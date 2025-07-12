# nomad

An HTTP server built from scratch in C using raw sockets on Windows (Winsock2).

## How It Works

1. Sets up a TCP server socket on `localhost:5000`
2. Waits for a single client connection
3. Parses the incoming HTTP request manually (no libraries)
4. Sends an appropriate HTTP response
5. Closes the connection

## Endpoints

### `GET /`

**Returns:**
```http
HTTP/1.1 200 OK
```

---

### `GET /echo/{message}`

**Returns:** Echoes the ``{message}`` value back in the response body.
**Example:**
```bash
curl http://localhost:5000/echo/hello
```

**Response:**
```http
HTTP/1.1 200 OK
Content-Type: text/plain
Content-Length: 5

hello
```

---

### `GET /user-agent`

**Returns:** The value of the ``User-Agent`` header from the request.
**Example:**
```bash
curl --header "User-Agent: foobar/1.2.3" http://localhost:5000/user-agent
```

**Response:**
```http
HTTP/1.1 200 OK
Content-Type: text/plain
Content-Length: 13

foobar/1.2.3
```