#include <chrono>     // steady_clock, duration_cast — timing
#include <cstddef>    // size_t
#include <cstdint>    // uint64_t, uint8_t
#include <iostream>   // std::cout
#include <vector>     // std::vector — your heap buffer


uint64_t sink =0;

void walk(const std::vector<uint8_t>& buf){
  uint64_t sum=0;
  for(size_t i =0;i<buf.size();i+=64){
      sum+=buf[i];
  }
  sink+=sum;
}

int main(){
  const size_t target = 256ull << 20; //256mib in bytes left shift
  for(size_t bytes= 16ull << 10;bytes<= 512ull <<20;bytes*=4){ // 16kib till 512 Mib
    std::vector<uint8_t> buf(bytes,1);

    walk(buf); //warmup to avoid pagefault
    
    size_t reps = target/bytes; //small bytes gets repeated to get acc res
    if(reps<1) reps=1;
    
    //start timer to end timer
    auto t0 = std::chrono::steady_clock::now();
    for(size_t r=0;r<reps;r++) walk(buf);
    auto t1 = std::chrono::steady_clock::now();

    double ns = static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count());

    double gbs = static_cast<double>(bytes)* reps/ns;
    
    std::cout << (bytes >> 10) <<"Kib"<<gbs<<"GB/s\n";

  }
  std::cout << "sink=" << sink << "\n";
}
