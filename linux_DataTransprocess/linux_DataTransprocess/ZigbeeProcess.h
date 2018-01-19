#ifndef _INCLUDE_ZIGBEE_PROCESS_H_
#define _INCLUDE_ZIGBEE_PROCESS_H_

#define ZIGBEE_DATA_BUF_MAX_LEN  4096 //���ݻ���ĳ���

char Net_Zigbee_IPAddr[20];	// �Զ�  �ͻ��˷���IP
int Net_Zigbee_Port;//���Ͷ˿�
 
 
int s_port_fd,s_port_fd_client;
int connect_counter;//���Ӽ���  ���ڳ�ʱ��û�б��ĺ� ������������
//int new_fd;
unsigned char RevBuf1[ZIGBEE_DATA_BUF_MAX_LEN];
unsigned char RevBuf2[ZIGBEE_DATA_BUF_MAX_LEN];
//int RecPointer;
//int socketConected;  //������������
bool OpenSerial_Flag;//���ڴ��Ƿ�ɹ�

void DataInit();
void ReadXMLCfgData();
bool OpenSerial(int iPort,int ibaud,int iparity);
void StartNetServer(int l_port);



#endif // _INCLUDE_ZIGBEE_PROCESS_H_