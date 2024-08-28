# Multithreaded Web Server in C

## Overview

This project implements a multithreaded web server written in C. The server handles multiple client requests concurrently using threads, and it employs a queue system to manage incoming requests efficiently. The project demonstrates core concepts of operating systems such as process management, threading, and synchronization.

## File Descriptions

- **`client.c`**: Handles client-side operations, possibly for testing the server or as part of a client-server interaction model.
  
- **`server.c`**: The main server file that initiates the server, listens for incoming client connections, and delegates requests to worker threads for processing.

- **`request.c`** and **`request.h`**: Contains functions and definitions related to processing client requests, such as parsing HTTP requests, sending responses, and handling different types of requests.

- **`queue.c`** and **`queue.h`**: Implements a queue data structure used to store incoming client requests before they are processed by the server. This is crucial for managing multiple simultaneous requests.

- **`output.c`**: Likely contains functions related to output operations, possibly for logging or sending responses to clients.

- **`segel.c`** and **`segel.h`**: Typically, auxiliary files that might handle low-level system interactions (e.g., networking, threading) or provide utility functions for the server.

- **`Makefile`**: Contains instructions for compiling the project. It automates the build process, specifying how to compile the server and link the necessary files.

## Setup and Compilation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/Multithreaded-Web-Server.git
   ```

2. **Navigate to the Project Directory:**
   ```bash
   cd Multithreaded-Web-Server
   ```

3. **Compile the Project:**
   - Use the provided `Makefile` to compile the project:
     ```bash
     make
     ```

   - This will produce an executable named `server`.

4. **Run the Server:**
   - Start the server by running the executable:
     ```bash
     ./server <port_number>
     ```
   - Replace `<port_number>` with the desired port number.

5. **Client Interaction:**
   - Use `client.c` or any HTTP client (e.g., `curl`, `browser`) to send requests to the server.

## Features

- **Multithreading:** The server handles multiple client requests concurrently using threads, improving performance and responsiveness.
- **Request Queue:** Implements a queue to manage incoming requests efficiently.
- **HTTP Request Handling:** Processes basic HTTP requests and sends appropriate responses.
- **Logging:** Logs incoming requests and server actions for monitoring and debugging.

## Future Enhancements

- **Advanced HTTP Support:** Extend the server to handle more complex HTTP requests and responses.
- **Security Features:** Implement basic security measures such as HTTPS support and request validation.
- **Load Balancing:** Introduce load balancing for handling high traffic scenarios.

