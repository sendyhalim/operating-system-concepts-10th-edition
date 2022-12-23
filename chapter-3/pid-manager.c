#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MIN_PID 300
#define MAX_PID 5000

int bit_field_count = (MAX_PID / 63) + 1;
uint64_t *pid_bit_fields;

int allocate_map() {
  pid_bit_fields = calloc(bit_field_count, sizeof(uint64_t));

  return 1;
}

int count_set_bit(uint64_t n) {
  int counter = 0;

  while (n > 0) {
    n = (n & (n - 1));
    counter++;
  }

  return counter;
}

int allocate_pid() {
  // Iterate and check which bit_fields are available
  int i = 0;
  uint64_t *current_bit_field;

  while (i < bit_field_count) {
     current_bit_field = &pid_bit_fields[i];

    // Adding 1 to a full set bits will circular back to 0.
    // (~(uint64_t) 0) + 1 == 0 <--- all bits are set.
    if (pid_bit_fields[i] + 1 != 0)  {
      break;
    }

    i++;
  }

  uint64_t __current_bit_field = *current_bit_field;
  uint64_t bit_value_to_set = 1;

  // Iterate while the current bit field right most bit is 1 (unavailable)
  // then shift right it 1 by 1;
  while ((__current_bit_field & 1) == 1) {
    bit_value_to_set = bit_value_to_set << 1;
    __current_bit_field = __current_bit_field >> 1;
  }

  *current_bit_field = *current_bit_field | bit_value_to_set;

  return (i * 64) + count_set_bit(*current_bit_field);
}

void release_pid(int pid) {
}

int main() {
  allocate_map();
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  printf("Hey %d\n",  allocate_pid());
  return 0;
}

