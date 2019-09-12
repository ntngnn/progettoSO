#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  // do stuff :)

    int fd = running->syscall_args[0];

    SemDescriptor* desc=SemDescriptorList_byFd(&(running->sem_descriptors ), fd);

    //check if process keep track of the semaphore descriptor(=has opened the semaphore)
    if(!desc){
        running->syscall_retvalue=DSOS_ESEMPOST_DESC;
        printf("descrittore non valido");

        return;
    }

    Semaphore* sem=desc->semaphore;

    sem->count++;

    if(sem->count <= 0 &&  sem->waiting_descriptors.first != NULL ){

        // //SCHEDULNG:
                   //1)take PCB selected by the waiting list
                   //2)and place in  the ready list.



        SemDescriptorPtr* desc_ready= (SemDescriptorPtr*) List_detach(&(sem->waiting_descriptors) ,(ListItem*) (sem->waiting_descriptors).first );	    //drop first descriptor of waiting descriptors


        PCB* ready=desc_ready->descriptor->pcb;  //correspoding PCB


        List_detach(&waiting_list , (ListItem*) ready);
        List_insert(&ready_list , ready_list.last , (ListItem*) ready);
        ready->status=Ready;     //update process status

        //Cambiamo stato del processo chiamante che viene messo in reaDY
        running->status=Ready;
        List_insert(&ready_list , ready_list.last, (ListItem*) running);

    }

    running->syscall_retvalue=0;

}
