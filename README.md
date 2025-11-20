<div align="center">
  <h1>
    <img width="300" alt="nomad Logo" src="https://github.com/user-attachments/assets/4754b671-1fd1-429a-b3ae-0285e048ed23" />
  </h1>

  <p>nomad is an HTTP server built from scratch in C. Without using any external HTTP libraries, it handles TCP connections, parses raw HTTP requests, and serves responses using the <a href="https://learn.microsoft.com/en-us/windows/win32/api/winsock2/">Winsock2 API</a> and <a href="https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports">IOCP</a> for non-blocking I/O operations.</p>
</div>

## Project Structure

nomad is organized into modular components:

```
src/
├── main.c                # Entry point, sets up IOCP, worker threads, and server context
├── utils.c/h             # Useful functions (memory, file reading, etc.)
├── server/               # Server socket and context creation
│   ├── server.c/h
├── client/               # Client connection handling (accept, send, recv.), HTTP request parsing
│   ├── client.c/h
│   ├── client_request.c/h
├── threads/              # Worker thread logic for IOCP
│   ├── threads.c/h
├── cache/                # Static file caching (uses a linked list type structure)
│   ├── cache.c/h
```

## Building and Running

### Prerequisites

- Windows operating system
- GCC compiler (MinGW-w64 recommended) + Make utility

### Build Instructions

1. **Clone & navigate to the repository (Windows CMD)**:
   ```bash
   git clone https://github.com/leanmak/nomad.git
   cd nomad
   ```

2. **Build the project & start the server (in your GCC compiler)**:
   ```bash
   make && ./nomad
   ```

The server will start on `localhost:3000` by default (port can be changed in [the utils header file](https://github.com/leanmak/nomad/blob/main/src/utils.h#L7)).

## Future Goals
- [X] Multi-threaded, non-blocking server (IOCP)
- [X] Static file caching
- [ ] Dynamic content generation (_this will take a long time..._)

## Note
This is basically a static file web server, it serves only two pages:
- ``index.html`` - if the incoming request is to the ``/`` path.
- ``404.html`` - if the incoming request is to any other path.<br>

<p>Once dynamic content generation is implemented, the ability to execute external programs or scripts will become available.</p>

Lastly, there are other concurrency models (old versions) of this HTTP Server stored in other branches:
- [Single-threaded (blocking)](https://github.com/leanmak/nomad/tree/single-threaded)
- [Thread-per-connection (multithreaded + blocking)](https://github.com/leanmak/nomad/tree/thread-per-connection)
