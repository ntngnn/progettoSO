#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){


    int ret;

  //fd semaforo da chiudere passato come argomento

   int sem_to_close = running->syscall_args[0];



    if(sem_to_close < 0 ){

        running->syscall_retvalue=DSOS_ESEMCLOSE_IDNEG;
        printf("semaphore fd must be positive");
        return;
    }


    //'recuperiamo' il descrittore del semaforo
    SemDescriptor* desc= SemDescriptorList_byFd(&running->sem_descriptors , sem_to_close);

    if(!desc){
        running->syscall_retvalue=DSOS_ESEMCLOSE_DESC;
        return;
    }

    Semaphore* sem=desc->semaphore;
    SemDescriptorPtr* ptr=desc->ptr;

    desc=List_detach(&(running->sem_descriptors),(ListItem*)desc);
    ret= SemDescriptor_free(desc);
    //verifica operazione riuscita(ret=0)
    if(ret){
        running->syscall_retvalue=ret;
        return;
    }

    ptr=(SemDescriptorPtr*) List_detach(&(sem->descriptors),(ListItem*)desc->ptr);
    ret= SemDescriptorPtr_free(ptr);
    //verifica operazione riuscita(ret=0)
    if(ret){
        running->syscall_retvalue=ret;
        return;
    }




    if((sem->descriptors).size==0){
        sem=List_detach(&semaphoreList,(ListItem*) sem);
        ret=Semaphore_free(sem);
        //verifica operazione riuscita(ret=0)
        if(ret){
            running->syscall_retvalue=ret;
            return;
        }


    }


    running->syscall_retvalue=0;



}
