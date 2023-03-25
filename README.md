# remote-procedure-calls
A client-server application connecting via TCP protocol. After connection, client is able to remotely call a procedure aquired from server.

Client application was created in C++ with Qt framework for Windows. Executable file is contained in *client* catalog. Application source files are contained in *source* catalog. Qt environment is required for compilation.

Server application is contained in *server* catalog. It was made for Linux. Compilation and execution steps:

Server compilation:

    g++ *.cpp -o 'server name'
    
    g++ -Wall -std=c++17 -pthread *.cpp -o 'server name'
    
Server execution:

    ./'server name'
