#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int msqid; /* IPC дескриптор для очереди сообщений */
	char pathname[] = "09-1a.c";
   	key_t key; 
    	int i,len, maxlen;
    	
	struct mymsgbuf
    	{ 
        	long mtype;
		struct {
			char end_mess[81];
        		char mtext[81]; 
		} info; 
    	} mybuf;
    	
	strcpy(mybuf.info.end_mess,"begin");

    	if((key = ftok(pathname,0)) < 0){
        	printf("Can\'t generate key\n");
        	exit(-1);
    	}
    	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        	printf("Can\'t get msqid\n");
        	exit(-1);
    	}
	printf("wait to run r2...\n"); 
    	for (i = 1; i <= 5; i++){
        	mybuf.mtype = 1;
        	strcpy(mybuf.info.mtext, "This is text message");
        	len = strlen(mybuf.info.mtext)+sizeof(mybuf.info.end_mess);
        		if (msgsnd(msqid, (struct msgbuf *) &mybuf, 
            			len, 0) < 0){
            			printf("Can\'t send message to queue\n");
            			msgctl(msqid, IPC_RMID, 
                		(struct msqid_ds *) NULL);
            			exit(-1);
        		}
    		}	
    	strcpy(mybuf.info.end_mess,"end");
    	if (msgsnd(msqid, (struct msgbuf *) &mybuf, 
        	len, 0) < 0){
        	printf("Can\'t send message to queue\n");
        	msgctl(msqid, IPC_RMID, 
            	(struct msqid_ds *) NULL);
        	exit(-1);
    	}
	while(1){
		maxlen = 350;
		if(len = msgrcv(msqid,
			(struct msgbuf *) &mybuf, maxlen, 2, 0) < 0){
			printf("cant recieve message from queue\n");
			exit(-1);	
		}
		if ((strcmp(mybuf.info.end_mess,"end"))== 0){
			 printf("end_message_ done!\n");
 	        	 msgctl(msqid, IPC_RMID, 
 	         	(struct msqid_ds *) NULL);
 	         	exit(0);
			}
 	    	sleep(1);
 	    	printf("message type = %ld, info = %s\n",
 	    	mybuf.mtype, mybuf.info.mtext);
	}	
    	return 0;
}
