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

     if(SemaphoreList_byId(&semaphores_list , id){

}
