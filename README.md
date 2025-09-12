<div align="center">
  <h1>
    <img width="300" alt="nomad Logo" src="https://github.com/user-attachments/assets/4754b671-1fd1-429a-b3ae-0285e048ed23" />
  </h1>

  <p>nomad is an HTTP server implemented entirely in C. Without using any external HTTP libraries, it handles TCP connections, parses raw HTTP requests, and serves responses using the Winsock2 API and IOCP for non-blocking I/O operations.</p>
</div>

## Features
- Multi-threaded, non-blocking IOCP server
- Static file caching for fast response
- Minimal HTTP request parsing
- Serves `index.html` for `/` and `404.html` for all other paths

## Architecture

The project is organized into modular components:

```
src/
├── main.c                # Entry point, sets up IOCP, threads, and server context
├── utils.c/h             # Utility functions (WSA init, memory, file reading, etc.)
├── server/               # Server socket creation, AcceptEx, server context
│   ├── server.c/h
├── client/               # Client connection handling, HTTP request parsing
│   ├── client.c/h
│   ├── client_request.c/h
├── threads/              # Worker thread logic for IOCP
│   ├── threads.c/h
├── cache/                # Static file caching
│   ├── cache.c/h
```

## Building and Running

### Prerequisites

- Windows operating system
- GCC compiler (MinGW-w64 recommended)
- Make utility

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone https://github.com/leanmak/nomad.git
   cd nomad
   ```

2. **Build the project**:
   ```bash
   make
   ```

3. **Run the server**:
   ```bash
   ./nomad.exe
   ```

The server will start on `localhost:3000` by default (port can be changed in [the utils header file](https://github.com/leanmak/nomad/blob/main/src/utils.h#L10)).

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
