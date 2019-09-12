#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "disastrOS_semaphore.h"
#include "disastrOS.h"
#include "disastrOS_globals.h"






// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}



/*
void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid());

  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((20-disastrOS_getpid())*5);
  }
  disastrOS_exit(disastrOS_getpid()+1);
}
*/

void child_test(void* args){

    printf("   hello , I'M CHILD ,PID: %d \n",running->pid);

    int app;
    int ret = disastrOS_semOpen(16, 1);
    if(ret < 0 ){
          disastrOS_exit(disastrOS_getpid()+1);
    }



    printf("     **semopen child return value(fd) = %d **\n",ret);

    printf("   Semaphore list : \n");
    SemaphoreList_print(&semaphoreList);
    int j = 0 ;
    while(j < 5 ){

        	app= disastrOS_semWait(ret);
            if(app != 0){
                disastrOS_exit(disastrOS_getpid()+1);
            }
         	printf(" [son in critical section] \n");
        	app=disastrOS_semPost(ret);

            if(app != 0){
                disastrOS_exit(disastrOS_getpid()+1);
            }
            j++;
    }

    app=disastrOS_semClose(ret);
    if(app!=0){
        disastrOS_exit(disastrOS_getpid()+1);
    }


    printf("  !!!son job done!!!\n");
    disastrOS_exit(disastrOS_getpid()+1);
}

void dad_test(void* args){

    printf("   HELLO I'M DAD ,PID: %d \n",running->pid);

    // int nsem;
    //printf("inserire numero  semaforo \n");
    //scanf("%d",&nsem);
    int ret,app;
    int i = 0;

    ret=disastrOS_semOpen(16,1);   //opening  semaphore 16

    if(ret < 0 ){
          disastrOS_exit(disastrOS_getpid()+1);
    }

    printf("  **semopen dad return value (fd) = %d ** \n " , ret);


    printf("  Semaphore list : \n");
    SemaphoreList_print(&semaphoreList);



    while(i < 5){

        app= disastrOS_semWait(ret);
            if(app != 0){
                disastrOS_exit(disastrOS_getpid()+1);
            }

    	printf(" [dad in critical section] \n");

    	app=disastrOS_semPost(ret);
        if(app != 0){
                disastrOS_exit(disastrOS_getpid()+1);
        }
        i++;
    }
    app=disastrOS_semClose(ret);
    if(app!=0){
        disastrOS_exit(disastrOS_getpid()+1);
    }


    printf(" !!!dad job done!!! \n");
    disastrOS_exit(disastrOS_getpid()+1);

}




void initFunction(void* args) {


    printf("    hello, I am init and I just started\n");
    printf("    we are testing the problem with 1 dad and  1 son , that 'access' in critical section alternativaly .\n");
    printf("    info: we use semaphore 16 for the mutual exclusion. \n");


    //"testing with 1 producer 1 consumer"



    disastrOS_spawn(dad_test,0);
    disastrOS_spawn(child_test,0);
    disastrOS_wait(0,NULL);
    disastrOS_wait(0,NULL);


    printf("  Semaphore List  :");
    SemaphoreList_print(&semaphoreList);

    printf("shutdown!\n");
    disastrOS_shutdown();

/*

  printf("I feel like to spawn 1 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }*/

}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
 // printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}

