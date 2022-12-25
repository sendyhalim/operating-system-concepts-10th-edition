#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MIN_PID 300
#define MAX_PID 500

typedef unsigned int bit_t;
int bit_width = 32;
int bit_field_count = ((MAX_PID - MIN_PID) / 32) + 1;

struct pid_state_t {
  bit_t *pid_bit_fields;
  int pid_counter;
  int pid_width;
};

struct pid_state_t pid_state;

int allocate_map() {
  pid_state.pid_bit_fields = calloc(bit_field_count, sizeof(bit_t));
  pid_state.pid_counter = 0;
  pid_state.pid_width = MAX_PID - MIN_PID + 1; // To include the MIN_PID as well

  return 1;
}

int count_set_bit(bit_t n) {
  int counter = 0;

  while (n > 0) {
    n = (n & (n - 1));
    counter++;
  }

  return counter;
}

int allocate_pid() {
  // Guard, it's full!
  if (pid_state.pid_counter == pid_state.pid_width) {
    return -1;
  }

  int i = 0;
  bit_t *current_bit_field;

  // Iterate and check which bit_field has empty slot
  while (i < bit_field_count) {
     current_bit_field = &pid_state.pid_bit_fields[i];

    // Stop when bit field still has empty slot
    if (~(*current_bit_field) != 0) {
      break;
    }

    i++;
  }

  int bit_position_to_set = 0;

  // Iterate until we find unset bit;
  while ((*current_bit_field & (1 << bit_position_to_set)) != 0) {
    bit_position_to_set++;
  }

  *current_bit_field = *current_bit_field | (1 << bit_position_to_set);
  bit_t pid = (MIN_PID + (i * bit_width) + bit_position_to_set) % MAX_PID;
  pid_state.pid_counter++;

  if (pid == 0) {
    return MAX_PID;
  } else if (pid < MIN_PID) {
    return pid + MIN_PID;
  } else {
    return pid;
  }
}

void release_pid(int pid) {
  // Guard
  if (! (pid >= MIN_PID && pid <= MAX_PID)) {
    return;
  }

  bit_t normalized_pid = pid - MIN_PID;
  int bit_field_position = (normalized_pid / bit_width); // No need to +1 because index is 0 based.
  int pid_position = normalized_pid % bit_width;

  bit_t *current_bit_field = &pid_state.pid_bit_fields[bit_field_position];

  // Turn off the bit at the given position
  *current_bit_field = *current_bit_field ^ (1 << pid_position);
  pid_state.pid_counter--;
}

int main() {
  allocate_map();

  int pid = 0;

  while (pid < MAX_PID && pid != -1) {
    pid = allocate_pid();
    printf("Hey %d\n",  pid);
  }

  release_pid(388);
  release_pid(332);
  release_pid(300);
  printf("LAST %d\n",  allocate_pid());
  printf("LAST %d\n",  allocate_pid());
  printf("LAST %d\n",  allocate_pid());

  return 0;
}

