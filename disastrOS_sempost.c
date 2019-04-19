#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  // do stuff :)



    int fd=syscall_args[0];

    SemDescriptor* desc=SemDescriptorList_byFd(&running->sem_descriptors , (ListItem*)fd);

    if(!desc){
        running->syscall_retvalue=DSOS_ESEMPAHOREDESC;
        prinf("descrittore non valido");

        return;
    }

    Semaphore sem=desc->semaphore;

    sem->count++;

    if(sem->count <= 0 &&  sem->waiting_descriptors != NULL ){
        SemDescriptor desc_ready= (SemDescriptor*) List_detach(&sem->waiting_descriptors ,(ListItem*) (sem->waiting_descriptors).first );




}
