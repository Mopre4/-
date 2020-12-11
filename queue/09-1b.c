#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int msqid, i; /* IPC дескриптор для очереди сообщений */
    char pathname[] = "09-1a.c";  
    int len, maxlen; 
    key_t key;
    struct mymsgbuf
    { 
        long mtype;
	struct {
		char end_mess[81];
		char mtext[81];
	} info;
    } mybuf;

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    printf("wait to run r1...\n"); 
    while(1){
        maxlen = 250;
        if( len = msgrcv(msqid, 
            (struct msgbuf *) &mybuf, maxlen, 1, 0) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
	if ((strcmp(mybuf.info.end_mess,"end"))== 0){
		printf("end_message_ done!\n");
		break;
        	}
        sleep(1);
	printf("message type = %ld, info = %s\n", 
        mybuf.mtype, mybuf.info.mtext);
	
    }
	strcpy(mybuf.info.end_mess, "begin");
   	for(i = 1; i <= 5; i++){
		mybuf.mtype = 2;
		strcpy(mybuf.info.mtext, "Sended_message_to_pr1!");
		len = strlen(mybuf.info.mtext)+ sizeof(mybuf.info.end_mess);
			if(msgsnd(msqid, (struct msgbuf *) &mybuf,
				len, 0 ) < 0) {
				printf("cant send message to queue\n");
				msgctl(msqid, IPC_RMID,
				(struct msqid_ds *) NULL);
				exit(-1);
			}
		}
	strcpy(mybuf.info.end_mess, "end");
	if (msgsnd(msqid, (struct msgbuf *) &mybuf,
		len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID,
		(struct msqid_ds *) NULL);
                 exit(-1);
          }
	printf("I sended all the messages to queue\n");
 return 0; 
}
