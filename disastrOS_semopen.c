

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
  // do stuff :)


     int id=running->syscall_args[0];
     int count=running->syscall_args[1];



    // check if semaphore already exists.
     Semaphore* new_sem=SemaphoreList_byId(&semaphoreList, id);
     if(!new_sem){     //true: we have to alloc new semaphore
        new_sem=Semaphore_alloc(id , count);
        if(!new_sem){
            printf("errore creazione semaforo");
            running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
            return;

        }

        List_insert(&semaphoreList,semaphoreList.last,(ListItem*) new_sem); //entry new semaphore in the global Semaphorelist
     }




    running->last_sem_fd+=1;


    //new semaphore descriptor is created and  associated with  runnning PCB
    SemDescriptor* new_sem_desc=SemDescriptor_alloc(running->last_sem_fd, new_sem , running);

    if(!new_sem_desc){
        printf("errore creazione semaforo");
        running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
        return;
    }


    //new semDescriptorPtr
    SemDescriptorPtr* new_sem_desc_ptr=SemDescriptorPtr_alloc(new_sem_desc);

    if(!new_sem_desc_ptr){
        running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
        printf("errore nella creazione del semaforo");
        return;
    }

    new_sem_desc->ptr=new_sem_desc_ptr;

    //adding descriptor and its pointer in the respective lists
    List_insert(&(running->sem_descriptors),(running->sem_descriptors).last,(ListItem*)new_sem_desc);
    List_insert(&(new_sem->descriptors ), new_sem->descriptors.last , (ListItem*)new_sem_desc_ptr);

    // return value= semaphore descriptor

    running->syscall_retvalue=new_sem_desc->fd;

    return;

}
