#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){


  //fd semaforo da chiudere passato come argomento

   int sem_to_close = running->syscall_args[0];

    if(sem_to_close < 0 ){

        running->syscall_retvalue=DSOS_ESEMCLOSE_IDNEG;
        printf("semaphore fd must be positive");
        return;
    }



    SemDescriptor* desc= SemDescriptorList_byFd(&running->sem_descriptors , sem_to_close);

    if(!desc){
        running->syscall_retvalue=DSOS_ESEMCLOSE_DESC;
        return;
    }

    Semaphore* sem=desc->semaphore;

    SemDescriptorPtr* ptr=(SemDescriptorPtr*) List_detach(&sem->descriptors,(ListItem*)desc->ptr);

    if(!ptr){
        running->syscall_retvalue=DSOS_ESEMPAHORECLOSE;
        return;
    }

    List_detach(&running->sem_descriptors,(ListItem*)desc);


    if(sem->waiting_descriptors.size==0){
        List_detach(&semaphoreList,(ListItem*) sem);
        Semaphore_free(sem);
        printf("semaforo deallocato");
    }

    SemDescriptorPtr_free(ptr);
    SemDescriptor_free(desc);

    running->syscall_retvalue=0;


}
