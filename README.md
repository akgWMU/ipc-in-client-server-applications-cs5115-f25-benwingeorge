# IPC in Client-Server Applications (C)

**Author:** Benwin George  
**Course:** CS5115 – Fall 2025  
**Assignment:** PA6 – Client–Server Architecture using IPC  
**Date:** November 2025

---

## 🎯 Objective

This project demonstrates **Interprocess Communication (IPC)** in C through a client-server architecture using two different UNIX mechanisms:

- **Named Pipes (FIFOs)**
- **UNIX Domain Sockets**

Clients send arithmetic operation requests (`add`, `sub`, `mul`, `div`) to the server, which processes them and returns results.

---

## 📂 Project Structure

```
IPC-IN-CLIENT-SERVER-APPLICATIONS-C/
│
├── fifo/
│   ├── client.c
│   ├── server.c
│   ├── makefile
│   └── README.md
│
└── socket/
    ├── client.c
    ├── server.c
    └── makefile
```

Each directory contains a self-contained client-server implementation with its own Makefile.

---

## 🔧 Implementation Details

### 1️⃣ FIFO (Named Pipes)

**Location:** `fifo/`

#### How It Works

- Two named pipes (FIFOs) handle bidirectional communication:
  - `fifo_request` – Client → Server
  - `fifo_response` – Server → Client
- The server reads requests, performs calculations, and writes results back
- Uses `open()`, `read()`, and `write()` system calls

#### Running the FIFO Implementation

```bash
cd fifo
make
./server      # Start server (run first)
./client      # Start client (run second)
```

#### Example Usage

```
Enter operation (add/sub/mul/div or exit): mul
Enter two integers: 6 9
Result from server: 54
```

#### Cleanup

FIFOs are automatically removed on server exit:

```c
unlink("fifo_request");
unlink("fifo_response");
```

---

### 2️⃣ UNIX Domain Sockets

**Location:** `socket/`

#### How It Works

- Uses `AF_UNIX` domain sockets for local communication
- Server creates socket file `/tmp/server_socket`, binds, and listens
- Clients connect, send messages, and receive computed results
- Supports sequential client connections (extensible to concurrent with `fork()`)

#### Running the Socket Implementation

```bash
cd socket
make
./server      # Start server
./client      # Start client (can run multiple instances)
```

#### Example Usage

```
Enter operation (add/sub/mul/div or exit): add
Enter two integers: 10 25
Result from server: 35
```

The socket file is automatically cleaned up on exit.

---

## 📦 Message Format

Both implementations use a common message structure:

```c
struct message {
    char operation[8];  // "add", "sub", "mul", "div"
    int operand1;       // First operand
    int operand2;       // Second operand
};
```

Messages are transmitted as binary data between processes.

---

## 🛠️ Compilation & Execution

### Build

```bash
make           # Builds both client and server
./server &     # Run server in background
./client       # Run client and interact
```

### Clean Build Artifacts

```bash
make clean
```

---

## ✅ Error Handling

| Error Type | Handling |
|------------|----------|
| **Invalid Operation** | Client validates input before sending to server |
| **Divide by Zero** | Server checks `operand2 == 0` and returns 0 with warning |
| **Broken Pipe/Socket** | Graceful termination with proper cleanup |
| **Exit Command** | Typing `exit` closes connection and ends loop |

---

## 🔍 System Calls Used

- **Process Control:** `fork()`, `exit()`, `unlink()`
- **File/FIFO I/O:** `open()`, `read()`, `write()`, `close()`
- **Socket Communication:** `socket()`, `bind()`, `connect()`, `listen()`, `accept()`
- **String Handling:** `strcmp()`, `strcpy()`, `memset()`

---

## 🏗️ Design Decisions

- **Separate Implementations:** FIFO and Socket versions are independent for clarity
- **Structured Communication:** Shared `struct message` ensures consistency
- **Synchronization:** Blocking read-write loop ensures clean request-response flow
- **Proper Cleanup:** All file descriptors and IPC resources are closed/unlinked on exit

---

## 🧪 Testing Summary

✓ Verified all four arithmetic operations with various inputs  
✓ Tested multiple sequential client requests  
✓ Validated error handling (invalid operations, divide-by-zero)  
✓ Confirmed proper cleanup of socket files and FIFOs  
✓ Tested abrupt client disconnections

---

## 📝 Conclusion

This project demonstrates fundamental client-server communication patterns using two different IPC mechanisms. It highlights key concepts in systems programming:

- Message passing and synchronization
- Error handling and validation
- Resource management and cleanup
- Process communication in UNIX environments

---

## 📚 Resources

- [Linux Programmer's Manual - pipe(7)](https://man7.org/linux/man-pages/man7/pipe.7.html)
- [Linux Programmer's Manual - unix(7)](https://man7.org/linux/man-pages/man7/unix.7.html)
- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/)

---

## 📄 License

This project is part of academic coursework for CS5115.