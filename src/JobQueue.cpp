#include "JobQueue.h"

void JobQueue::push(const Job& job) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        jobs.push(job);
    }
    cv.notify_one();
}

Job JobQueue::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !jobs.empty(); });

    Job job = jobs.front();
    jobs.pop();
    return job;
}
