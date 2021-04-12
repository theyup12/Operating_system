//
//  main.c
//  mutexes
//

#include <stdio.h>
#include <pthread.h>


typedef struct counter counter;
struct counter {
  int count;
  pthread_mutex_t lock;
};

void counter_init(counter* c) {
  c->count = 0;
  pthread_mutex_init(&c->lock, NULL);
}

void counter_increment(counter* c, int id) {
  pthread_mutex_lock(&c->lock);
  ++c->count;
//  printf("%sthread %d inc to: %d\n", (id == 0 ? "" : "\t\t"), id, c->count);
  pthread_mutex_unlock(&c->lock);
}

int counter_get(counter* c) {
  pthread_mutex_lock(&c->lock);
  int val = c->count;
  pthread_mutex_unlock(&c->lock);
    return val;
}

#define MAXRUNS   20
counter c;

void* runner(void* param) {
  int id = (int)(long)(param), count = 0;
  int finished = 0;
  printf("======================================= Thread %d started...\n", id);

  while (!finished) {
    // TODO:  code to increment, get, and display the values
    //    of the count member of counter c
    //    using mutexes
    counter_increment(&c, id);
    int num = counter_get(&c);
    printf("%sT%d inc -> %d\n", (id ==0 ? "" : "\t\t"), id , num);
    if (++count >= MAXRUNS) { finished = 1; }
  }
  
  printf("======================================= Thread %d terminating...\n", id);
  pthread_exit(NULL);
}


int main(int argc, const char* argv[]) {
  pthread_t tidA, tidB;

  // TODO:  code to create threads and join them
  pthread_attr_t p_attr; 
  pthread_attr_init(&p_attr);

  counter_init(&c);
  
  printf("creating threads tidA and tidB..\n");
  pthread_create(&tidA, &p_attr, runner, (void*)1);
  pthread_create(&tidB, &p_attr, runner, (void*)0);
  
  pthread_join(tidA, NULL);
  pthread_join(tidB, NULL);
  printf("thread tidA and tidB joined.\n");

  printf("\n\t\t...done.\n");
  return 0;
}
