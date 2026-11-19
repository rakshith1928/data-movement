#include <atomic>     // std::atomic, fetch_add, memory_order_relaxed — the new one
#include <chrono>     // steady_clock, duration<double, milli> — timing
#include <cstdint>    // uint64_t
#include <iostream>   // std::cout
#include <thread>     // std::thread, hardware_concurrency
#include <vector>     // std::vector — thread list, private slots

const size_t   kThreads = 8;                
const uint64_t kIters    = 5ull << 20;       

uint64_t plain = 0;                           
std::atomic total{0};               

void add_racy() {
    for (uint64_t i = 0; i < kIters; ++i)
        plain += 1;                                       
}

void add_atomic() {
    for (uint64_t i = 0; i < kIters; ++i)
        total.fetch_add(1, std::memory_order_relaxed);    
}

void add_private(uint64_t* out) {
    uint64_t local = 0;
    for (uint64_t i = 0; i < kIters; ++i)
        local += 1;                                       
    *out = local;                                         
}

double ms_since(std::chrono::steady_clock::time_point t0) {  
  return std::chrono::duration(
        std::chrono::steady_clock::now() - t0).count();
}


int main() {
    const uint64_t expected = kThreads * kIters;
    std::cout << "expected: " << expected << "\n";

    std::vector<std::thread> ts;

    // Variant A: Racy
    auto t0 = std::chrono::steady_clock::now();

    for (size_t t = 0; t < kThreads; ++t)
        ts.emplace_back(add_racy);

    for (std::thread& th : ts)
        th.join();

    std::cout << "racy     total=" << plain
              << "  " << ms_since(t0) << " ms\n";

    ts.clear();

    // Variant B: Atomic
    total.store(0, std::memory_order_relaxed);
    t0 = std::chrono::steady_clock::now();

    for (size_t t = 0; t < kThreads; ++t)
        ts.emplace_back(add_atomic);

    for (std::thread& th : ts)
        th.join();

    std::cout << "atomic   total=" << total.load()
              << "  " << ms_since(t0) << " ms\n";

    ts.clear();

    // Variant C: Private
    std::vector<uint64_t> slots(kThreads, 0);

    t0 = std::chrono::steady_clock::now();

    for (size_t t = 0; t < kThreads; ++t)
        ts.emplace_back(add_private, &slots[t]);

    for (std::thread& th : ts)
        th.join();

    uint64_t sum = 0;

    for (uint64_t s : slots)
        sum += s;

    std::cout << "private  total=" << sum
              << "  " << ms_since(t0) << " ms\n";

    return 0;
}
