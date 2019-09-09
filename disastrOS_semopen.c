

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

     if(id < 0 ){
         running->syscall_retvalue=DSOS_ESEMOPEN_IDNEG;
         printf("error:Semaphore id must be positive");
         return;
     }


    // verifica semaforo preesistente e creazione semaforo in caso negativo
     Semaphore* new_sem=SemaphoreList_byId(&semaphoreList, id);
     if(!new_sem){
        new_sem=Semaphore_alloc(id , count);
        if(!new_sem){
            printf("errore creazione semaforo");
            running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
            return;

        }
        //inserimento del nuovo semaforo nella lista
        List_insert(&semaphoreList,semaphoreList.last,(ListItem*) new_sem);
     }






    //creazione descrittore semaforo e assegnazione al pcb del processo running
    running->last_sem_fd+=1;
    SemDescriptor* new_sem_desc=SemDescriptor_alloc(running->last_sem_fd, new_sem , running);

    if(!new_sem_desc){
        printf("errore creazione semaforo");
        running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
        return;
    }



    //puntatore al descrittore
    SemDescriptorPtr* new_sem_desc_ptr=SemDescriptorPtr_alloc(new_sem_desc);

    if(!new_sem_desc_ptr){
        running->syscall_retvalue=DSOS_ESEMOPEN_CREATION;
        printf("errore nella creazione del semaforo");
        return;
    }

    new_sem_desc->ptr=new_sem_desc_ptr;

    //inserimento descrittore e puntatore nelle rispettive liste
    List_insert(&(running->sem_descriptors),(running->sem_descriptors).last,(ListItem*)new_sem_desc);
    List_insert(&(new_sem->descriptors ), new_sem->descriptors.last , (ListItem*)new_sem_desc_ptr);

    //ritorniamo il descrittore del semaforo(risore identificate dai descrittori)

    running->syscall_retvalue=new_sem_desc->fd;

    return;

}
