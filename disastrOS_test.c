#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "disastrOS.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}




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

void child_test(void *args){

    printf("testing");
    /*int fd=*((int*)args);
	for (int j=0 ; j < 5 ; j++){

        disastrOS_semWait(fd);
         printf("figlio in sezione critica");
        disastrOS_semPost(fd);
    }*/
    printf("figlio termina");
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");

  //testing semopen and semclose

    //apriamo il semaforo 16
    printf("/n open semaforo16 /n");
    int nsem=16;
    int ret,pid;
    int retval;

    ret=disastrOS_semOpen(nsem,0);



    printf("open con ritorno(fd) : %d /n" , ret);
    disastrOS_printStatus();

    disastrOS_spawn(child_test , 0 );

    for (int i ; i < 5; i++){
    	disastrOS_semWait(ret);

    	printf("padre in sez critica");

    	disastrOS_semPost(ret);
    }

    pid=disastrOS_wait(0,&retval);
    disastrOS_semClose(ret);


    /*printf("SSSSSSS/n");
    //apriamo semaforo 17
    nsem=17;
    retval=disastrOS_semOpen(nsem,17);
    printf(" ritorno %d " , retval );
    disastrOS_printStatus();

    printf("chiusura di 16");
    retval=disastrOS_semClose(ret);
    printf("chiusura con ritorno %d" , retval);
    disastrOS_printStatus();
    printf("chiudiamo semaforo***")
	ret=disastrOS_semClose(nsem);
      printf("close con ritorno: %",ret);

   disastrOS_spawn(sleeperFunction, 0);


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
  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}

