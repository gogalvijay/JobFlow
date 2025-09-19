#include "JobQueue.h"
#include "Worker.h"
#include "Server.h"
#include <vector>
#include <iostream>
#include <map>
#include <mutex>

int main() {
    JobQueue queue;
    std::map<int, std::string> results;
    std::mutex results_mtx;

    // Start workers
    std::vector<Worker*> workers;
    for (int i = 0; i < 3; i++) {
        workers.push_back(new Worker(queue, results, results_mtx));
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
