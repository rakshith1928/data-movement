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


void link(std::vector<Node>& nodes , std::mt19937& rng){

    const size_t n = nodes.size();
    std::vector<std::size_t> perm(n);
    for(size_t i= 0 ;i<n; ++i) perm[i] = static_cast<std::size_t>(i);
    std::shuffle(perm.begin(),perm.end(),rng);
    for(size_t i =0;i<n; ++i)
        nodes[perm[i]].next = perm[(i+1)%n];
        

}

void chase(const std::vector<Node>& nodes){
  std::uint32_t p =0;
  for(std::size_t i =0;i<nodes.size(); ++i)
      p = nodes[p].next;
  
  sink+=p;
}

int main(){
  std::mt19937 rng(42);
  const size_t target_hops = 1ull << 24;

  for(size_t n = 256; n<=1ull<<22;n*=4){
    std::vector<Node> nodes(n);
    link(nodes,rng);
    chase(nodes);

    size_t reps = target_hops/n;
    if(reps<1) reps=1;

    auto t0 = std::chrono::steady_clock::now();
    for(size_t r=0;r<reps;++r) chase(nodes);

    auto t1 = std::chrono::steady_clock::now();
    double ns = static_cast<double>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
        t1 - t0
      ).count()
    );

    double per_hop = ns/target_hops;

    std::cout<< ((n*64)>>10) << "Kib" << per_hop <<"ns/hop\n";

    
  }
  std::cout<<"sink=" << sink << "\n";
}
                       
