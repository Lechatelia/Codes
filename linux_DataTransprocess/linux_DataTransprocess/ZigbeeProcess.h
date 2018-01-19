#ifndef _INCLUDE_ZIGBEE_PROCESS_H_
#define _INCLUDE_ZIGBEE_PROCESS_H_

#define ZIGBEE_DATA_BUF_MAX_LEN  4096 //数据缓冲的长度

char Net_Zigbee_IPAddr[20];	// 对端  客户端访问IP
int Net_Zigbee_Port;//上送端口
 
 
int s_port_fd,s_port_fd_client;
int connect_counter;//连接计数  用于长时间没有报文后 进行重新连接
//int new_fd;
unsigned char RevBuf1[ZIGBEE_DATA_BUF_MAX_LEN];
unsigned char RevBuf2[ZIGBEE_DATA_BUF_MAX_LEN];
//int RecPointer;
//int socketConected;  //服务端连接情况
bool OpenSerial_Flag;//串口打开是否成功

void DataInit();
void ReadXMLCfgData();
bool OpenSerial(int iPort,int ibaud,int iparity);
void StartNetServer(int l_port);



#endif // _INCLUDE_ZIGBEE_PROCESS_H_