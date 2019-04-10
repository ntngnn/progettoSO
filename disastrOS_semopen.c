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

   //creazione semaforo e inserimento in lista semafori(allocazione descrittore per tenerne traccia)
    Semaphore* new_sem=Semaphore_alloc(id , count);

    int fd = running->last_sem_fd;
    SemDescriptor* new_sem_desc=SemDescriptor_alloc(fd++, new_sem , running);

    if(!new_sem || !new_sem_desc){
        printf("errore creazione semaforo");
        running->syscall_retvalue=DSOS_ESEM_CREATION;
        return;
    }

    List_insert(&SemaphoreList,NULL,(ListItem*) new_sem);
    List_insert(&running->sem_descriptors,NULL,(ListItem*)new_sem_desc);

    SemDescriptorPtr_alloc* new_sem_desc_ptr=SemDescriptorPtr_alloc(new_sem_desc);

    List_insert(&new_sem->descriptors ,NULL , (ListItem*)new_sem_desc_ptr);

}
