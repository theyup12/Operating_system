//
//  main.c
//  pingpong
//
//

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

//==========================================================================================
//  NOTE:
   sem_t semPing, semPong;           // on Linux, PC
//
//   #include <dispatch/dispatch.h>    // macOS ONLY
//   dispatch_semaphore_t semPing;     // use this code on macOS
//   dispatch_semaphore_t semPong;
//==========================================================================================

#define PINGS  5
#define PINGERS 2
#define PONGERS 2

//==========================================================================================
// NOTE:
//   sem_post(semPong);                   // on Linux, PC is the same as
//   dispatch_semaphore_signal(semPong);  // on macOS
//
//   sem_wait(&semPong);                                        // on Linux, PC
//   dispatch_semaphore_wait(semPong, DISPATCH_TIME_FOREVER);   // on macOS
//==========================================================================================


//---------------------------------------------------------------
//
void* ping(void* x) {
  int pings = PINGS;
  while (pings-- > 0) {
    // TODO: code to write ping here, and to alternate with PONG using semaphores
   
    sem_wait(&semPing);
    printf("ping\t");
    sem_post(&semPong);
    printf("\n");
    
    
  }
  return NULL;
}

//---------------------------------------------------------------
//
void* pong(void* x) {
  int pongs = PINGS;
  while (pongs-- > 0) {
    // TODO: code to write PONG here, and to alternate with ping
    sem_wait(&semPong);
    printf("pong\t");
    sem_post(&semPing);
    
    
  }
  return NULL;
}


//---------------------------------------------------------------
//
int main(int argc, const char * argv[]) {
  pthread_t pinger, ponger;
  pthread_attr_t attr;
  
  sem_init(&semPing, 0, 1);
  sem_init(&semPong, 0, 0);
//  semPing = dispatch_semaphore_create(0); // macOS ONLY
//  semPong = dispatch_semaphore_create(1);

  pthread_attr_init(&attr);
  pthread_create(&pinger, &attr, ping, NULL);
  pthread_create(&ponger, &attr, pong, NULL);
  
  pthread_join(pinger, NULL);
  printf("\njoined pinger\n");

  pthread_join(ponger, NULL);
  printf("joined ponger\n");
  
//  sem_destroy(semPing);
//  sem_destroy(semPong);
//  dispatch_release(semPing);    // macOS ONLY
//  dispatch_release(semPong);

  printf("\n\tdone...\n");
  return 0;
}




//==========================================================================================
//  EXPECTED OUTPUT
//==========================================================================================

//ping  PONG
//ping  PONG
//ping  PONG
//ping  PONG
//ping  PONG
//
//joined pinger
//joined ponger
//
//  done...
