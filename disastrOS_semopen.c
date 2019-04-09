#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"

#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
  // do stuff :)


     int id=running->syscall_args[0];

     if(id < 0 ){
         syscall_retvalue=DSOS_EIDNEG;
         printf("error:Semaphore id must be positive");
         return;
     }

     if(SemaphoreList_byId(&SemaphoreList, id){
        running->syscall_retvalue=DSOS_EXISTINGID;
        printf("error: Semaphore Id already exists");
        return;
     }

    int count=running->syscall_args[1];

}
