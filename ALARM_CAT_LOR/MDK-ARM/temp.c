





typedef struct    
{
	unsigned char IMEI[20];//IMEI    ����Ǳ仯��
	unsigned char pubtopic[25];//      ���Ҳ�Ǳ仯��
	unsigned char subtopic[25];//����Ǳ仯��      
	unsigned char username[12];//�̶���
	unsigned char passwore[12];//�̶���
	unsigned char upIMEI[47];  //����Ǳ仯��
}EC600u_init; 


EC600u_init  EC600uinit; 




case 0x0C:
								if (fat_send_cmd("AT+GSN\r\n", 25))
										
									{
													
										if(ec600_RxBuffer[2]>='0'&&ec600_RxBuffer[2]<='9')//��ȡģ��IMEI��ֵ
										{
												 //(const char *)(cFatUartRecvBuf)
											
											
											    for(i=0;i<25;i++)
													{
														
														EC600uinit.pubtopic[i]=0;
														EC600uinit.subtopic[i]=0;
													}	
											
													
													
													  for(i=0;i<20;i++)
													{
														EC600uinit.IMEI[i]=0;
														
													}	
													
											
												for(i=0;i<15;i++)
													{
														 EC600uinit.IMEI[i]=ec600_RxBuffer[i+2];
														 EC600uinit.IMEI[15]=0;
														 EC600uinit.IMEI[16]=0;
														 EC600uinit.IMEI[17]=0;
														 EC600uinit.IMEI[18]=0;
													}	
													EC600uinit.pubtopic[0]='a';
													EC600uinit.pubtopic[1]='p';
													EC600uinit.pubtopic[2]='p';
													EC600uinit.pubtopic[3]='/';
													strcat((char *)EC600uinit.pubtopic,( char *)EC600uinit.IMEI); 
													EC600uinit.subtopic[0]='d';
													EC600uinit.subtopic[1]='e';
													EC600uinit.subtopic[2]='v';
													EC600uinit.subtopic[3]='/';
													strcat((char *)EC600uinit.subtopic,( char *)EC600uinit.IMEI); 
//													Uart1_SendStr("��ȡ���豸IMEI���룺\r\n");													
												   //Uart1_SendDATA((char *)EC600uinit.subtopic,19);
//													Uart1_SendStr((char *)EC600uinit.subtopic);						
//											        Uart1_SendStr("\r\n");	
											        ucErrorTimes = 0;
											        ucStateNum=0x0D;
													
										}
										else
											{
												//����3�εò�����ȷӦ��
												if (ucErrorTimes++ > 3)
												{
													ucStateNum = 0x00;
												}
											}
									}
									break;	


									
									
									
									
									

					 case 0x0E:
 

					     for(i=0;i<47;i++)
													{
														
														 EC600uinit.upIMEI[i]=0; 
														
													}	
				 
					 
strcat(( char *)EC600uinit.upIMEI,(char*)EC600uinit.IMEI); 
					 
	            
					 EC600uinit.username[0]='"';
					 EC600uinit.username[1]=',';
					 EC600uinit.username[2]='"';
					 EC600uinit.username[3]='d';
					 EC600uinit.username[4]='e';
					 EC600uinit.username[5]='v';
					 EC600uinit.username[6]='i';
					 EC600uinit.username[7]='c';
					 EC600uinit.username[8]='e';
					 EC600uinit.username[9]='"';
           EC600uinit.username[10]=0;
					 EC600uinit.username[11]=0;							
					 strcat(( char *)EC600uinit.upIMEI,(char*)EC600uinit.username); 
					 
					 EC600uinit.passwore[0]=',';
					 EC600uinit.passwore[1]='"';
					 EC600uinit.passwore[2]='1';
					 EC600uinit.passwore[3]='2';
					 EC600uinit.passwore[4]='3';
					 EC600uinit.passwore[5]='4';
					 EC600uinit.passwore[6]='5';
					 EC600uinit.passwore[7]='6';
					 EC600uinit.passwore[8]='7';
					 EC600uinit.passwore[9]='8';
					 EC600uinit.passwore[10]=0;
					 EC600uinit.passwore[11]=0;


//					 EC600uinit.passwore[0]=',';				 
//					 EC600uinit.passwore[1]='1';
//					 EC600uinit.passwore[2]='2';
//					 EC600uinit.passwore[3]='3';
//					 EC600uinit.passwore[4]='4';
//					 EC600uinit.passwore[5]='5';
//					 EC600uinit.passwore[6]='6';
//					 EC600uinit.passwore[7]='7';
//					 EC600uinit.passwore[8]='8';
//					 EC600uinit.passwore[9]=0;
				 strcat(( char *)EC600uinit.upIMEI,(char*)EC600uinit.passwore); 
					 
						//	sprintf(buf,"AT+QMTCONN=0,\"%s\"\r\n",EC600uinit.IMEI);
						sprintf(buf,"AT+QMTCONN=0,\"%s\"\r\n",EC600uinit.upIMEI);
							if (fat_send_cmd(buf,250))
							{
											//�յ�+QMTCONN: 0,0,0
								if (fat_cmdres_keyword_matching("+QMTCONN: 0,0,0"))
									{
									  ucErrorTimes = 0;
									  ucStateNum=0x0F;
									 }
								else
									{
														//����10�εò�����ȷӦ��
										if (ucErrorTimes++ > 3)
										{
											ucStateNum = 0x00;
										}
									}
							}
							
							break;						



//�̶��Ķ���������Sprintf���� ƴ��
							
							
							
//��������� memset(a,'\0',sizeof(a));

//memset(recvbuf, 0, SOCKET_BUF_SIZE);
// memcpy(recvbuf, p + 1, length);
							
							
							
int arr[]={1,2,3};
for(int i=0;i<(sizeof(arr)/sizeof(int));i++){
    printf("%d,",arr[i]);
}
 							
							
							