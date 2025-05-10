A minimalist yet robust concurrent HTTP/1.0 web server written in C built from the ground up using OS-level system calls and POSIX threads.


- **Thread-per-request concurrency**  
  Every connection is handled in an isolated thread with proper resource cleanup.

- **Static file serving**  
  Supports `GET` requests for HTML, images, and more. Uses `mmap()` for file I/O.

- **Thread-safe logging**  
  Requests are logged to a centralized file (`logs/access.log`) using a mutex to prevent race conditions.

- **Modular Design**  
  Components are split cleanly across modules:
  - `http_handler`: request parsing and response formatting
  - `server`: socket setup and request dispatching
  - `logger`: logging system
  - `utils`: helpers for string/path sanitization and validation

---

## Concepts in Action

- POSIX sockets (`socket()`, `bind()`, `accept()`)
- Threading (`pthread_create()`, `pthread_mutex_t`)
- Memory mapping (`mmap()`)
- File and directory handling (`open()`, `stat()`, `read()`, `write()`)
- Manual HTTP response crafting (no libraries, no frameworks)

---

## Build & Run

```bash
make
./server <port> <directory>
````

Example:

```bash
./server 8080 www/
```

Test with:

```bash
curl http://localhost:8080/index.html
```

---

## Logs

All access logs are written to:

```
logs/access.log
```

Each request is recorded with the timestamp, client IP, and requested path.

---

## Implemented Extensions

* ✅ `mmap()`-based file reads
* ✅ Secure path resolution
* ✅ Thread-safe centralized logging

---

## Benchmarks & Analysis

This server works well for small projects and learning purposes.
For heavier traffic, using a thread pool or switching to event-driven models like epoll would scale better. Current performance is mainly limited by:

 * the cost of creating a new thread for every request
 * how the OS schedules these threads
 * blocking I/O while reading and sending files
