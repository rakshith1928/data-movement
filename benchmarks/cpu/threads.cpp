#include <chrono>     // steady_clock, duration_cast — timing
#include <cstddef>    // size_t
#include <cstdint>    // uint8_t, uint64_t
#include <iostream>   // std::cout
#include <thread>     // std::thread, hardware_concurrency — the new one
#include <vector>     // std::vector — buffer, slots, thread list
void walk_slice(const uint8_t* first, const uint8_t*last, uint64_t* out){
    uint64_t sum = 0;
    for(const uint8_t* p =first;p<last;p+=64){
      sum+= *p;
    }
    *out = sum;
}

int main(){
    std::cout << "hardware threads:"
              <<std::thread::hardware_concurrency()<<"\n";
    
}

