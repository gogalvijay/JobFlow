#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream> 

Server::Server(int p, JobQueue& q, std::map<int, std::string>& res, std::mutex& res_mtx)
    : port(p), queue(q), results(res), results_mtx(res_mtx), th(&Server::run, this) {}

Server::~Server() {
    running = false;
    if (th.joinable()) th.join();
    close(server_fd);
}

void Server::run() {
    sockaddr_in address{};
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return;
    }

    std::cout << "[Server] Listening on port " << port << std::endl;

    while (running) {
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);
        std::string command(buffer);

        std::string response = handleCommand(command);
        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }
}

std::string Server::handleCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::string method, path;
    iss >> method >> path;

    if (method == "POST" && path == "/job") {
        std::string task;
        std::getline(iss, task);
        if (!task.empty() && task[0] == ' ') task.erase(0, 1);

        int id = nextJobId++;
        Job job{id, task, ""};
        queue.push(job);

        return "Job " + std::to_string(id) + " received\n";
    } else if (method == "GET" && path == "/job") {
        int id;
        iss >> id;

        std::lock_guard<std::mutex> lock(results_mtx);
        if (results.find(id) != results.end()) {
            return "Result for job " + std::to_string(id) + ": " + results[id] + "\n";
        } else {
            return "Job " + std::to_string(id) + " is still processing\n";
        }
    }

    return "Invalid command\n";
}
