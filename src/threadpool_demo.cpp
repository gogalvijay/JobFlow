// Example usage of ThreadPool in main.cpp
#include "ThreadPool.h"
#include <iostream>
#include <chrono>

int main() {
    ThreadPool pool(4);
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for tasks to finish
    return 0;
}
