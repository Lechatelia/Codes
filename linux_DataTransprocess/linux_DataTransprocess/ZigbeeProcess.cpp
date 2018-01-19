
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>   //struct ifreq
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include "Mtu_def.h"
#include "OsBase.h"
#include "SysLog.h"
#include "ZigbeeProcess.h"



static void SigProc1(int sig)
{
	printf("enter SigProc sig = %d",sig);
 
	//exit(0);
}

void DataInit()
{
	memset(Net_Zigbee_IPAddr,'\0',sizeof(Net_Zigbee_IPAddr));
	Net_Zigbee_Port = 6679;
	s_port_fd=-1;
	s_port_fd_client=-1;
	connect_counter=0;
}
bool OpenSerial(int iPort,int ibaud,int iparity)//打开串口
{
	 tcflag_t baudRate,parity=0;
	struct termios opt; 
	char cSerialName[20];

	if(s_port_fd!=0)
	{ 
		close(s_port_fd);
		s_port_fd = 0;
	}
	//sprintf(cSerialName, "/dev/ttySP%d", iPort - 1);
	sprintf(cSerialName, "/dev/ttymxc%d", iPort - 1);
	printf("open serila name:%s \n", cSerialName);
	CSysLog::Info("open serila name:%s ", cSerialName);
	 
	s_port_fd = open(cSerialName, O_RDWR | O_NOCTTY  | O_NDELAY);                        
	if(s_port_fd < 0) 
	{
		CSysLog::Info("open serila Error:%s ", cSerialName);
		return 0;
	}
	
	tcgetattr(s_port_fd, &opt);      //获取选项

	switch(ibaud)
	{
		case 115200:
			baudRate=B115200; break;
		case 38400:
			baudRate=B38400; break;
		case 19200:
			baudRate=B19200; break;
		case 9600:
			baudRate=B9600; break;
		case 4800:
			baudRate=B4800; break;
		case 2400:
			baudRate=B2400; break;
		default:
			baudRate=B19200;
	}
	cfsetispeed(&opt, baudRate);//设置波特率
	cfsetospeed(&opt, baudRate);

	/*
     * raw mode
     */
    opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG);
    opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
    opt.c_oflag   &=   ~(OPOST);
	switch(iparity)
	{
		case 0://无校验
			opt.c_cflag &= ~PARENB;
			opt.c_cflag &= ~CSTOPB;
			opt.c_cflag &= ~CSIZE;
			//opt.c_cflag |= CS8;
			break;
		case 2://偶校验
			opt.c_cflag |= PARENB;
			opt.c_cflag &= ~PARODD;
			opt.c_cflag &= ~CSTOPB;
			opt.c_cflag &= ~CSIZE;
			break;           
		case 1://奇校验
			opt.c_cflag |= PARENB;
			opt.c_cflag |= PARODD;
			opt.c_cflag &= ~CSTOPB;
			opt.c_cflag &= ~CSIZE;
			break;
		default:
			opt.c_cflag &= ~PARENB;
			opt.c_cflag &= ~CSTOPB;
			opt.c_cflag &= ~CSIZE;
			opt.c_cflag |= CS8;//默认无校验
	}
	opt.c_cflag   |=   CS8;//8位
	if (tcsetattr(s_port_fd,   TCSANOW,   &opt)<0) 
	{
		return   0;
	}
	tcflush(s_port_fd,TCIOFLUSH);
	CSysLog::Info("open serila cSerialName=%s,ibaud = %d,iparity=%d,s_port_fd=%d", cSerialName,ibaud,iparity,s_port_fd);
	return 1;
	
}
//从XML读配置数据
void ReadXMLCfgData()
{ 
	char tmp[128];
	tm t;
	// 加载xml文档
	char xmlfile[260];
	sprintf(xmlfile, "%s",XML_DATA_CONFIG);
	rapidxml::xml_document<> doc;
	rapidxml::file<> fxml(xmlfile);
	doc.parse<0>(fxml.data());

	rapidxml::xml_node<> *ConfigData =  doc.first_node("Config")->first_node("ConfigData");//查找 ComConf 节点
	 
	if(ConfigData)
	{
		rapidxml::xml_node<>*m_ConfigItem;//  查找 ConfigItem 节点
		for(m_ConfigItem= ConfigData->first_node("ConfigItem");m_ConfigItem;m_ConfigItem = m_ConfigItem->next_sibling("ConfigItem"))
		{
			int m_Inaddr = atoi(m_ConfigItem->first_attribute("Inaddr")->value());//内部地址

			if(m_Inaddr == 17)
			{ 
				strcpy(Net_Zigbee_IPAddr,m_ConfigItem->first_attribute("Value")->value()); 
			}
			if(m_Inaddr == 18)
			{
				Net_Zigbee_Port = atoi(m_ConfigItem->first_attribute("Value")->value());
				break;//只有一项
			}
			
		}
		printf("Net_Zigbee_IPAddr = %s,Net_Zigbee_Port=%d\n",Net_Zigbee_IPAddr,Net_Zigbee_Port);
		CSysLog::Info("Net_Zigbee_IPAddr=%s,Net_Zigbee_Port=%d",Net_Zigbee_IPAddr,Net_Zigbee_Port);
		
	}
} 

//ipaddr对端IP
//r_port 对端端口
void OpenNetClient(char *ipaddr,int r_port)
{
	struct sockaddr_in remote_addr;  /* remote address information */
	//struct sockaddr_in my_addr;
	//struct ifreq ifr;
	//int i,n;
		
	if ((s_port_fd_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		CSysLog::Info("s_port_fd_client socket Error!");
		//exit(1); 
	}

	bzero(&remote_addr,sizeof(remote_addr)); /* zero the rest of the struct */ 	
	remote_addr.sin_family = AF_INET; /* host byte order */ 
	remote_addr.sin_port=htons(r_port);  //s_port_num=
	if(inet_aton(ipaddr,&remote_addr.sin_addr)==0) //Error = 0
	{
		perror("connect remote server Address Error"); 
		CSysLog::Info("s_port_fd_client connect remote server Address Error");
		close(s_port_fd_client);
		s_port_fd_client=-1;
		//exit(1); 
	}
	if(connect(s_port_fd_client,(struct sockaddr *)&remote_addr, sizeof(struct sockaddr))== -1) 
	{
		//perror("connect remote server Error"); 
		CSysLog::Info("s_port_fd_client connect remote server Error");
		//fprintf(stderr," s_port_fd_client=%d\n",s_port_fd_client);
		//printf("errno=%d \n",errno);
		close(s_port_fd_client);
		s_port_fd_client=-1;
		//exit(1); 
	}
	//fprintf(stderr," s_port_fd_client=%i\n",s_port_fd_client);
	CSysLog::Info("s_port_fd_client=%d",s_port_fd_client);
}

void TaskTimer(int sig)
{	
	connect_counter++;
	if(connect_counter>4*30)//半个小时
	{
		connect_counter=0;
		if(s_port_fd_client!=-1)
		{
			close(s_port_fd_client);
		}
		s_port_fd_client=-1;
	}
	if((s_port_fd_client==-1)||(s_port_fd_client==0))
	{		
		CSysLog::Info("s_port_fd_client will connect ");
		OpenNetClient(Net_Zigbee_IPAddr,Net_Zigbee_Port);
	}	
	
	alarm(15);	
	return;
}

int main(int argc, char *argv[])
{
	fd_set rfds;
	struct timeval tv;
	int i;
	int retval;
	int tmp=0;
	//socklen_t addrlen;
	//struct sockaddr_in their_addr; /* connector's address information */
	//socklen_t sin_size; 
	int numbytesclient1,numbytesclient2;
	//sigset_t set;

	if( GetProcessCount("ZigbeeProcess") > 1 )
	{
		printf("a same ZigbeeProcess instance exists!\n");
		return 0;
	}
	//signal(SIGHUP, SigProc1);
	// 配置日志
	char fullname[128];
	char logpath[128];
	char cmd[128];
	GetCurrentProcCmd(cmd);
	char *procname = GetBaseName(cmd);
	//GetLogPath(logpath);
	sprintf(fullname, "%s/%s", LOG_PATH, procname);

	CSysLog::SetIniCfg(fullname);
	CSysLog::Info("enter process ZigbeeProcess");


	
	DataInit();
	ReadXMLCfgData();
	
	//打开客户端
	OpenNetClient(Net_Zigbee_IPAddr,Net_Zigbee_Port);
	//定时器
	signal(SIGALRM,TaskTimer);	
	alarm(15);
	
	  
	while(1) // Read device data
	{		
		if(OpenSerial_Flag==0)//串口打开错误
		{
			if(OpenSerial(3,19200,0))
			{
				OpenSerial_Flag = 1;
			} 
			else
			{
				OpenSerial_Flag = 0;
			}
			usleep(1000);
			continue; 
		}
		
		FD_ZERO(&rfds);
		FD_SET(s_port_fd_client, &rfds);
		tmp = s_port_fd_client;
		FD_SET(s_port_fd, &rfds);
		tmp=tmp > s_port_fd ? tmp : s_port_fd;

		/* Wait up to five seconds. */
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(tmp+1, &rfds, NULL, NULL, &tv);
		/* Don't rely on the value of tv now! */
		if (retval == -1)
		{
			//perror("select()");
		}
		else if (retval ==0)
		{
			//perror("Zigbee: time out");
			//CSysLog::Info("Zigbee: time out");
			//continue;
		}
		else if (retval)
		{	       
			if(FD_ISSET(s_port_fd_client, &rfds))
			{                            
				if ((numbytesclient1=read(s_port_fd_client, &RevBuf1[0], (ZIGBEE_DATA_BUF_MAX_LEN-10))) == -1)
				{ 	
					perror("ZB ");                                   
					FD_CLR(s_port_fd_client,&rfds); 
					if(s_port_fd_client!=-1)close(s_port_fd_client);
					s_port_fd_client=-1;
					//exit(1); 
				}
				else if( numbytesclient1==0)
				{
					//perror("Transit");
					printf("Zigbee s_port_fd_client Disconnect\n");
					CSysLog::Info("Zigbee s_port_fd_client Disconnect");
					//socketConected=0;                                    
					FD_CLR(s_port_fd_client,&rfds); 
					if(s_port_fd_client!=-1)close(s_port_fd_client);
					s_port_fd_client=-1;
				}
				else
				{
					connect_counter=0;
					fprintf(stderr,"Zigbee s_port_fd_client recv numbytesclient1 = %d\n",numbytesclient1);
					CSysLog::Info("Zigbee s_port_fd_client recv numbytesclient1 = %d",numbytesclient1);
					//for(i=0;i<numbytesclient;i++)
					//{
					//	fprintf(stderr,"0x%x ",RevBuff[i]); 
					//}
					//fprintf(stderr,"\n");
					//fprintf(stderr,"NetSvrTDU numbytes=%d \n",numbytes);
					if(s_port_fd>0)
					{
						if (write(s_port_fd,&RevBuf1[0],numbytesclient1)<=0)	
						{
							fprintf(stderr,"Zigbee s_port_fd serialport failed!\n");
							CSysLog::Info("Zigbee s_port_fd serialport failed!");
						}
					}
				} 
			}
			
			if(FD_ISSET(s_port_fd, &rfds))
			{                            
				if ((numbytesclient2=read(s_port_fd, &RevBuf2[0], (ZIGBEE_DATA_BUF_MAX_LEN-10))) == -1)
				{ 	
					perror("Zigbee ");                                   
					FD_CLR(s_port_fd,&rfds); 
					if(s_port_fd!=-1)close(s_port_fd);
					s_port_fd=-1;
					//exit(1); 
				}
				else if( numbytesclient2==0)
				{
					//perror("Transit"); 
					printf("Zigbee s_port_fd Disconnect\n");
					CSysLog::Info("Zigbee s_port_fd Disconnect");
					//socketConected=0;                                    
					FD_CLR(s_port_fd,&rfds); 
					if(s_port_fd!=-1)close(s_port_fd);
					s_port_fd=-1; 
				}
				else
				{ 
					fprintf(stderr,"Zigbee s_port_fd recv numbytesclient2 = %d\n",numbytesclient2);
					CSysLog::Info("Zigbee s_port_fd recv numbytesclient2 = %d",numbytesclient2);
					//for(i=0;i<numbytesclient2;i++)
					//{
					//	fprintf(stderr,"0x%x ",RevBuff[i]); 
					//}
					//fprintf(stderr,"\n");
					//fprintf(stderr,"NetSvrTDU numbytes=%d \n",numbytes);
					if(s_port_fd_client>0)
					{
						if (send(s_port_fd_client, &RevBuf2[0],numbytesclient2, 0) == -1)			
						{
							fprintf(stderr,"Zigbee s_port_fd_client write port failed!\n");
							CSysLog::Info("Zigbee s_port_fd_client write port failed!");
						}    
					}
				} 
			}

		}
		else
		{
        		//fprintf(stderr,"No data within five seconds.\n");
		}
	}        
	//return 0; //never run here.
}  



 
