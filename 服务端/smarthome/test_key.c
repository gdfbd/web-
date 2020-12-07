#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "dev.h"
int ledAll=0;
int pwm=0;
static void sig_handler(int signo)
{
	//按键处理
	if(signo==SIGIO) 
	{
                read(btnFd,&keyID,sizeof(keyID));
		if(keyID == 2)//按键2实现第一三个灯和第二四个灯交替
		{
                        ledAll=!ledAll;
                        if(ledAll){
                			ledProcess(0,2);//灭
                            		ledProcess(1,1);//亮
					ledProcess(2,2);//灭
                            		ledProcess(3,1);//亮
                        }else{
					ledProcess(0,1);//亮
					ledProcess(1,2);//灭
					ledProcess(2,1);//亮
					ledProcess(3,2);//灭
                        }
		}
		else if(keyID == 3)//按键3实现流水灯 :按下一次执行一次流水灯
		{

                        //流水灯
                       
			while(1){
				 	int i;
                        		for(i = 0;i<4;i++)
                        		{
                            			ledProcess(i,1);
                            			sleep(1);
                        		}

                        		for(i = 0;i<=4;i++)
                        		{
                            			ledProcess(i,2);
                            			sleep(1);
                        		}
				}	
		}
		else if(keyID == 4)//按键4实现蜂鸣器:按下蜂鸣器响，再按下关闭
		{
                        pwm=!pwm;
                        if(pwm){
                            pwmProcess(1);
                        }else{
                            pwmProcess(2);
                        }
		}
	}
}

int keyProcess()
{
	//按键信号捕捉及设置异步等，挂起进程
	signal(SIGIO,sig_handler);
	fcntl(btnFd,F_SETOWN,getpid());
	fcntl(btnFd,F_SETFL,fcntl(btnFd,F_GETFL) | FASYNC);
	while(1);
	return 0;
}


