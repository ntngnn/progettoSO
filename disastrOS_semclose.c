#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){


  //fd semaforo da chiudere passato come argomento come per le altre risorse

   int sem_to_close = running->syscall_args[0];

    if(sem_to_close < 0 ){

        running->syscall_retvalue=DSOS_EIDNEG;
        printf("semaphore fd must be positive");
        return;
    }








}
