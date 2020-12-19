#include <iostream>

void compute_hash(uint16_t* curr_hash, const uint16_t time, const uint16_t student_id) {
  uint32_t prev_hash = *curr_hash;
  prev_hash = ((time ^ student_id) - prev_hash);
  std::cout << "DEBUG " << prev_hash << std::endl;
  prev_hash *= prev_hash;
  std::cout << "DEBUG " << prev_hash << std::endl;
  prev_hash = ((prev_hash >> 8) & UINT16_MAX);
  std::cout << "DEBUG " << prev_hash << std::endl;
  *curr_hash = prev_hash;
}

int main(void) {
  uint16_t curr_hash = 0;
  uint16_t time = 0;

  compute_hash(&curr_hash, time, 34351);

  return 0;
}
