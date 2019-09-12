#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){


    int ret;



   int sem_to_close = running->syscall_args[0];  //semaphore fd


    SemDescriptor* desc= SemDescriptorList_byFd(&running->sem_descriptors , sem_to_close);

    //check if process keep track of the semaphore descriptor(=has opened the semaphore).
    if(!desc){
        running->syscall_retvalue=DSOS_ESEMCLOSE_DESC;
        return;
    }

    Semaphore* sem=desc->semaphore;
    SemDescriptorPtr* ptr=desc->ptr;

    desc=(SemDescriptor*)List_detach(&(running->sem_descriptors),(ListItem*)desc);  //drop descriptor by running  descriptor list
    ret= SemDescriptor_free(desc);                                  //  descriptor free

    if(ret){
        running->syscall_retvalue=ret;
        printf("semclose error (free) ");
        return;
    }

    ptr=(SemDescriptorPtr*) List_detach(&(sem->descriptors),(ListItem*)desc->ptr);    //drop descriptor ptr from SemDescriptorPtr's list
    ret= SemDescriptorPtr_free(ptr);                                                  //descriptor free

    if(ret){
        running->syscall_retvalue=ret;
        printf("semclose error (free) ");
        return;
    }



    //if no other descriptorptr in the list , we can unlink the semaphore.
    if((sem->descriptors).size==0){
        sem=(Semaphore*)List_detach(&semaphoreList,(ListItem*) sem);
        ret=Semaphore_free(sem);

        if(ret){
            running->syscall_retvalue=ret;
            printf("semclose error (free)");
            return;
        }


    }


    running->syscall_retvalue=0;



}
