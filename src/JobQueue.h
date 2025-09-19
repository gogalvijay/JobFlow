#ifndef JOBQUEUE_H
#define JOBQUEUE_H

#include "Job.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class JobQueue {
private:
    std::queue<Job> jobs;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(const Job& job);
    Job pop();
};

#endif
