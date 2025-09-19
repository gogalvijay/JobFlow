#include "Worker.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <algorithm>

Worker::Worker(JobQueue& q, std::map<int, std::string>& res, std::mutex& res_mtx)
    : queue(q), results(res), results_mtx(res_mtx), th(&Worker::run, this) {}

Worker::~Worker() {
    running = false;
    if (th.joinable()) th.join();
}

void Worker::run() {
    while (running) {
        Job job = queue.pop();
        std::string result = processTask(job.task);

        {
            std::lock_guard<std::mutex> lock(results_mtx);
            results[job.id] = result;
        }

        std::cout << "[Worker] Job " << job.id << " processed: " << result << std::endl;
    }
}

std::string Worker::processTask(const std::string& task) {
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
