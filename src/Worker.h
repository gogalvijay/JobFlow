#ifndef WORKER_H
#define WORKER_H

#include "JobQueue.h"
#include <thread>
#include <atomic>
#include <map>
#include <mutex>

class Worker {
private:
    JobQueue& queue;
    std::map<int, std::string>& results;
    std::mutex& results_mtx;
    std::thread th;
    std::atomic<bool> running{true};

    void run();
    std::string processTask(const std::string& task);

public:
    Worker(JobQueue& q, std::map<int, std::string>& res, std::mutex& res_mtx);
    ~Worker();
};

#endif
