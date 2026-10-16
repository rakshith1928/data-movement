#include <algorithm>   // std::shuffle
#include <chrono>      // steady_clock, duration_cast — timing
#include <cstddef>     // size_t
#include <cstdint>     // uint32_t, uint64_t
#include <iostream>    // std::cout
#include <random>      // std::mt19937 — the RNG for shuffling
#include <vector>      // std::vector — the node list
                      
uint64_t sink =0;

struct Node{
    std::uint32_t next;
    char pad[60];
};
static_assert(sizeof(Node)== 64);

void link(std::vector& nodes , std::mt19937& rng){

    const size_t n = nodes.size();
    std::vector perm(n);
    for(size_t i= 0 ;i<n; ++i) perm[i] = static_cast(i);
    std::shuffle(perm.begin(),perm.end(),rng);
    for(size_t i =0;i<n; ++i)
        nodes[perm[i]].next = perm[(i+1)%n];
        

}

void chase(){
  std::unit32_t p =0;
  for(size_t i =0;i<nodes.size(); ++i)
      p = nodes[p.next];
  
  sink+=p;
}
                       
