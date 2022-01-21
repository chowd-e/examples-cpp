#!/bin/sh
g++ -pthread server.cpp Timer.cpp TcpSocket.cpp -o server
g++ client.cpp Timer.cpp TcpSocket.cpp -o client