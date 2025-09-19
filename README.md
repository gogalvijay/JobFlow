
# JobFlow 🚀
A **multi-threaded job queue and minimal text-based API server** in **C++17**.

This project demonstrates **system-level programming and backend concepts** like concurrency, synchronization, job scheduling, and socket-based communication. Now featuring a modern **ThreadPool** for efficient job execution.

---

## ✨ Features
- ✅ Multi-threaded **ThreadPool** to process jobs concurrently
- ✅ **Thread-safe job queue** using `mutex` and `condition_variable`
- ✅ Minimal **TCP text API** (via `nc`)
- ✅ Supports:
    - **Job submission** → `POST /job TASK sum 1 2 3 4`
    - **Result retrieval** → `GET /job <id>`
- ✅ Built-in jobs: sum, multiply, reverse, upper, sleep
- ✅ Graceful shutdown and robust concurrency

---

## 🛠️ Build & Run
Clone the repository and build with `make`:

```bash
git clone https://github.com/yourusername/JobFlow.git
cd JobFlow
make
./job_server
```

📡 Example Usage
1. Submit a Job

        printf "POST /job TASK sum 1 2 3 4\n" | nc localhost 8080
        # -> Job 1 received

2. Fetch the Result

        printf "GET /job 1\n" | nc localhost 8080
        # -> Result for job 1 = 10

⚙️ Tech Stack

        C++17 STL → std::thread, std::mutex, std::unique_lock, std::condition_variable
        ThreadPool + JobQueue pattern
        POSIX Sockets for networking
        Makefile build system

🎯 Why this project?


This project is designed to demonstrate backend systems knowledge:

    - Writing thread-safe concurrent code
    - Designing a producer-consumer job queue
    - Building a minimal API server in C++ without frameworks
    - Clear separation into modules (JobQueue, ThreadPool, Server)

It showcases skills relevant for SDE, backend, and system programming roles.

📌 What I Learned

    - Implementing concurrency in C++17 using threads, thread pools, and synchronization primitives
    - Designing a job scheduling system with a modern thread pool
    - Handling thread-safe shared state with mutex and unique_lock
    - Building a lightweight API server using sockets and custom protocol parsing
    - Clean modular project structure with Makefile for compilation

🚀 Future Work

    - Add more job types (factorial, sorting, string manipulation)
    - Extend server to REST API using Boost.Beast or HTTP libraries
    - Store results in SQLite / file-based persistence
    - Add a web client for job submission & result fetching
    - Dockerize for easy deployment
    - Add unit tests and interview question examples