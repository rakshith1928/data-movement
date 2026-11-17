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

    const size_t bytes = 512ull << 20;
    std::vector buf(bytes,1);

    std::vector sums(64,0);
    walk_slice(buf.data(),buf.data()+bytes,&sum[0]);//warmup

    for(size_t threads:{1ull, 2ull, 4ull, 8ull}){
        const size_t slice = bytes/threads;

        auto t0 = std::chrono::steady_clock::now();
        std::vector<std::thread> ts;
        for(size_t t=0;t<threads;++t){
            ts.emplace_back(walk_slice,
                            buf.data()+t*slice,
                            buf.data()+(t+1)*slice,
                            &sums[t]);


        }
        for(std::thread& th :ts){
            th.join();
        }

        auto t1= std::chrono::steady_clock::now();

        double ns = static_cast(
            std::chrono::duration_cast(t1-t0).count());
        double gbs = static_cast(bytes)/n;

        std::cout << threads<< "thread(s) " <<gbs<<"GB/s\n";


    }
    uint64_t total =0;
    for (uint64_t s : sums) total += s;
    std::cout<< "sink=" << total << "\n";
}

