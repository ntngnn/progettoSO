#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  // do stuff :)

    int fd=running->syscall_args[0];


    SemDescriptor* desc=SemDescriptorList_byFd(&(running->sem_descriptors), fd);

    //check if process keep track of the semaphore descriptor(=has opened the semaphore)
    if(!desc){
        running->syscall_retvalue=DSOS_ESEMWAIT_DESC;
        printf("fd semaforo non valido\n");
        return;
    }
    Semaphore* sem=desc->semaphore;




     sem->count -= 1;
    if(sem->count <=0){ //true : calling process scheduling in  waiting list


	    //SCHEDULING:
		//1)running process (calling process) is queued
               //2)running=first process in ready list

            SemDescriptorPtr* waitingptr= desc->ptr;
            List_insert(&(sem->waiting_descriptors) , sem->waiting_descriptors.last , (ListItem*) waitingptr); //process descriptor is placed in waiting list

            running->status=Waiting;

	        List_insert(&waiting_list , waiting_list.last , (ListItem*) running);


            if(ready_list.first){

                running=(PCB*) List_detach(&ready_list , (ListItem*) ready_list.first);

            }


    }


    running->syscall_retvalue=0;
    return;
}

