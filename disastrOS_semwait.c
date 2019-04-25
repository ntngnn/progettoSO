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

    SemDescriptor* desc=SemDescriptorList_byFd(&running->sem_descriptors , fd);

    if(!desc){
        running->syscall_retvalue=DSOS_ESEMWAIT_DESC;
        printf("id semaforo non valido");
        return;

    Semaphore* sem=desc->semaphore;

    sem->count -= 1;



    if(sem->count <= 0){

            SemDescriptorPtr* waitingptr= desc->ptr;
            List_insert(&(sem->waiting_descriptors) , sem->waiting_descriptors.last , (ListItem*) waitingptr); //descrittore processo nei waiting descriptors

            //inseriamo running(processo chiamante) nella coda dei processi in wait

            running->status=Waiting;
            List_insert(&waiting_list , waiting_list.last , (ListItem*) running);

            // primo processo in ready --> running
            if(ready_list != NULL){

                running=(PCB*) List_detach(&ready_list , (ListItem*) ready_list.first);

            }else{
                printf("no process ready");
            }

    }
    running->syscall_retvalue=0;
    return;
}

