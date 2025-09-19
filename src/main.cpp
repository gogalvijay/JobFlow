
#include "JobQueue.h"
#include "Server.h"
#include "ThreadPool.h"
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <algorithm>
#include <chrono>

// Job processing logic (moved from Worker)
std::string processTask(const std::string& task) {
    std::istringstream iss(task);
    std::string cmd;
    iss >> cmd; // should be "TASK"
    if (cmd != "TASK") return "Invalid command";

    std::string op;
    iss >> op;

    if (op == "sum") {
        int num, sum = 0;
        while (iss >> num) sum += num;
        return "sum = " + std::to_string(sum);
    } else if (op == "multiply") {
        int num, product = 1;
        bool hasNum = false;
        while (iss >> num) { product *= num; hasNum = true; }
        if (!hasNum) return "multiply needs numbers";
        return "product = " + std::to_string(product);
    } else if (op == "reverse") {
        std::string rest, word;
        while (iss >> word) rest += word + " ";
        if (!rest.empty()) rest.pop_back();
        std::reverse(rest.begin(), rest.end());
        return rest;
    } else if (op == "upper") {
        std::string rest, word;
        while (iss >> word) rest += word + " ";
        if (!rest.empty()) rest.pop_back();
        std::transform(rest.begin(), rest.end(), rest.begin(), ::toupper);
        return rest;
    } else if (op == "sleep") {
        int sec;
        if (iss >> sec) {
            std::this_thread::sleep_for(std::chrono::seconds(sec));
            return "slept " + std::to_string(sec) + " seconds, done";
        }
        return "sleep needs a number";
    } else {
        return "Unknown task: " + op;
    }
}

int main() {
    JobQueue queue;
    std::map<int, std::string> results;
    std::mutex results_mtx;

    // Start thread pool
    ThreadPool pool(3);

    // Enqueue job processing tasks in the thread pool
    for (int i = 0; i < 3; ++i) {
        pool.enqueue([&queue, &results, &results_mtx]() {
            while (true) {
                Job job = queue.pop();
                std::string result = processTask(job.task);
                {
                    std::lock_guard<std::mutex> lock(results_mtx);
                    results[job.id] = result;
                }
                std::cout << "[ThreadPool] Job " << job.id << " processed: " << result << std::endl;
            }
        });
    }

    // Start server
    Server server(8080, queue, results, results_mtx);

    std::cout << "Server started. Use nc or telnet to send commands.\n";
    std::cout << "Examples:\n";
    std::cout << "  printf \"POST /job TASK sum 1 2 3 4\\n\" | nc localhost 8080\n";
    std::cout << "  printf \"GET /job 1\\n\" | nc localhost 8080\n";

    std::cin.get(); // wait for Enter key
    return 0;
}
