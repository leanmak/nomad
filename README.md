<div align="center">
  <h1>
    <img width="300" alt="Nomad Logo" src="https://github.com/user-attachments/assets/4754b671-1fd1-429a-b3ae-0285e048ed23" />
  </h1>

  <p>Nomad is an HTTP server implemented entirely in C. Without using any external HTTP libraries, it handles TCP connections, parses raw HTTP requests, and serves responses using the Winsock2 API.</p>
</div>

## Architecture

The project is organized into a couple modular components:

```
src/
├── main.c              # Main server loop (Accepts incoming client sockets)
├── socket_server.c     # TCP socket creation and management (e.g. creating and binding server socket)
├── client_handler.c    # HTTP request handling and response generation
├── request_parser.c    # HTTP request parsing utilities
```

## Building and Running

### Prerequisites

- Windows operating system (multi-os support coming soon!)
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
   ./main.exe
   ```

The server will start on `localhost:3000` by default.

## Future Goals
- [ ] Make the server multi-threaded and non blocking to handle multiple client connections concurrently.
- [ ] Implement Multi-OS support
- [ ] Implement dynamic content generation (_this will take a long time..._)

## Note
This is basically a static file web server, it serves only two pages:
- ``index.html`` - if the incoming request is to the ``/`` path.
- ``404.html`` - if the incoming request is to any other path.<br>

<p>Once dynamic content generation is implemented, the ability to execute external programs or scripts will become available.</p>
