#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
int main(){
  	int semid; 
	key_t key; /* IPC ключ */ 
	char pathname[] = "ps.c";
	struct sembuf mybuf; 
	if((key = ftok(pathname,0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}
	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get semid\n");
		exit(-1);
	}
	
	int fd[2], result;
   	size_t size;
	int i = 0;
   	char resstring[24]; 
   	if(pipe(fd) < 0){
        	printf("Can\'t create pipe\n");
        	exit(-1); 
 	} 
	result = fork(); 
	sleep(5);
    	if(result<0){ 
        	printf("Can\'t fork child\n");
        	exit(-1);
    	} else if (result > 0) {
        	size = write(fd[1], "Hello, world!FromParent", 24);
        	if(size != 24){
            		printf("Can\'t write all string\n"); 
            		exit(-1); 
        	} 
        	close(fd[1]);
        	printf("Parent wait\n");
		mybuf.sem_op = -1;
		mybuf.sem_flg = 0;
		mybuf.sem_num = 0;
		if(semop(semid, &mybuf, 1) < 0){
			printf("Can\'t wait for condition\n");
			exit(-1);
		}
		printf("Perent continue work\n");
		size = read(fd[0], resstring, 24);
		if (size < 0){
			printf("cant read string\n");
			exit(-1);
		}
		printf("%s\n", resstring);
		close(fd[0]);
		if(semctl(semid, 0,IPC_RMID, 0) < 0){
                  	printf("err del semaphore\n");
                 	 exit(-1);
		}  
		
    	} else if(result == 0){
        	size = read(fd[0], resstring, 24);
 	   	if(size < 0){
        	    printf("Can\'t read string\n"); 
            		exit(-1); 
        	}	
        	printf("%s\n",resstring);
        	close(fd[0]);
		size = write(fd[1], "Hello, world!FromChildr",24);
		if(size != 24){printf("cant write all string\n"); exit(-1);}
		close(fd[1]);
		mybuf.sem_op = 1;
		mybuf.sem_flg = 0;
		mybuf.sem_num = 0;
		if(semop(semid, &mybuf, 1) < 0){
			printf("Can\'t wait for condition\n");
			exit(-1);
		}
    	}
	
    return 0; 
}
