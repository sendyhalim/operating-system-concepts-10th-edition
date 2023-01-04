#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int max_num = 0;
int min_num = 0;
double avg = 0;

void* avg_fn(void *numbers);
void* max_fn(void *numbers);
void* min_fn(void *numbers);

struct program_arg {
  int argc;
  char** argv;
};

int main(int argc, char** argv) {
  pthread_attr_t thread_attrs[3];
  pthread_t thread_ids[3];

  pthread_attr_init(&thread_attrs[0]);
  pthread_attr_init(&thread_attrs[1]);
  pthread_attr_init(&thread_attrs[2]);

  struct program_arg p_arg;
  p_arg.argc = argc;
  p_arg.argv = argv;

  pthread_create(&thread_ids[0], &thread_attrs[0], avg_fn, (void*) &p_arg);
  pthread_create(&thread_ids[1], &thread_attrs[1], max_fn, (void*) &p_arg);
  pthread_create(&thread_ids[2], &thread_attrs[2], min_fn, (void*) &p_arg);
  pthread_join(thread_ids[0], NULL);
  pthread_join(thread_ids[1], NULL);
  pthread_join(thread_ids[2], NULL);

  printf("Avg %f \n", avg);
  printf("Max %d \n", max_num);
  printf("Min %d \n", min_num);

  return 0;
}

void* avg_fn(void *p_arg_p) {
  struct program_arg* p_arg = (struct program_arg*) p_arg_p;
  double sum = 0;

  for (int i = 1; i < p_arg->argc; i++) {
    sum += atoi(p_arg->argv[i]);
  }

  avg = sum / (p_arg->argc - 1);

  pthread_exit(0);
}

void* max_fn(void *p_arg_p) {
  struct program_arg* p_arg = (struct program_arg*) p_arg_p;

  for (int i = 1; i < p_arg->argc; i++) {
    int current_num = atoi(p_arg->argv[i]);
    max_num = max_num > current_num ? max_num : current_num;
  }

  pthread_exit(0);
}

void* min_fn(void *p_arg_p) {
  struct program_arg* p_arg = (struct program_arg*) p_arg_p;

  for (int i = 1; i < p_arg->argc; i++) {
    int current_num = atoi(p_arg->argv[i]);
    min_num = min_num < current_num ? min_num : current_num;
  }

  pthread_exit(0);
}
