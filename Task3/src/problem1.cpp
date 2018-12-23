#include <atomic>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main() {
    size_t count = 0;
    std::atomic<bool> done(false);
    std::mutex mut;
    std::queue<int> items;

    std::thread producer([&]() {
        for (int i = 0; i < 10000; ++i) {
            std::lock_guard<std::mutex> guard(mut);
            items.push(i);
            count++;
        }
        done = true;
    });

    std::thread consumer([&]() {
        do {
            std::lock_guard<std::mutex> guard(mut);
            while (!items.empty()) {
                items.pop();
                count--;
            }
            if (done) { break; } // Check while guard is on
        } while (true);
    });

    producer.join();
    consumer.join();
    std::cout << count << std::endl;
}
