# TCP Client-Server Learning Exercise (C++)

## Overview

Dive into the world of TCP client-server connections with this hands-on learning exercise in C++. Explore the fundamental concepts of network programming by creating a simple Client-Server application using TCP sockets.

## Main Objectives

- [x] Create two files with the main function: one for the client (`client.cpp`) and one for the server (`server.cpp`).
- [x] Establish connectivity and communication.

## Server

- [x] Implement the code to create a socket and listen on a specified port (e.g., 7777).
- [x] Greet connecting clients with the message "Welcome."
- [x] Print received messages from clients to the console.

## Client

- [x] Implement the code to allow the client to connect to a socket on the server's port.
- [x] Receive and print the welcome message upon connecting to the server.
- [ ] Start sending messages with random integer values (e.g., "1," "2," "3," "4," ...) to the server.

## Bonus Objectives

- [ ] Save received messages from the server in the "history.txt" file with the format "dd-mm-yyyy client_x message."
- [ ] Connect two clients to the same server.

## Getting Started

1. Clone the repository.
2. Compile and run the `server.cpp` file, when running choose the port that you will be using. `gcc server.cpp -o server && ./server 7777`
3. Compile and run the `client.cpp` file, when running choose the IP and the port you will be using gcc `gcc client.cpp -o client && ./client 127.0.0.1 7777`

During testing, putty was also used to simulate the Client end of the server.

## Contribution

Feel free to contribute by providing enhancements, bug fixes, or additional features. Create a pull request, and let's make this learning exercise even better together!

Happy coding! 🚀
