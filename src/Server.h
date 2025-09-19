#ifndef SERVER_H
#define SERVER_H

#include "JobQueue.h"
#include "Job.h"
#include <thread>
#include <atomic>
#include <netinet/in.h>
#include <map>
#include <mutex>

class Server {
private:
    int port;
    int server_fd;
    std::atomic<bool> running{true};
    JobQueue& queue;
    std::map<int, std::string>& results;
    std::mutex& results_mtx;
    std::thread th;
    int nextJobId = 1;

    void run();
    std::string handleCommand(const std::string& cmd);

public:
    Server(int p, JobQueue& q, std::map<int, std::string>& res, std::mutex& res_mtx);
    ~Server();
};

#endif
