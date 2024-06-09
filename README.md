# Socket Connections C++ Library
Simple Unix-style socket connection library for inter-process connections  

### Files
* **sockConn.h** - Main header file
* **client.cpp** - Implementation file for client functions
* **server.cpp** - Implementation file for server functions
* **serializer.cpp** - Implementation file for serialization functions

### Limitations
* Objects like std::string don't work when serialized in a struct
* Windows functionality is not fully implemented