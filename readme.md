# MemDB

**MemDB is a memory based DB.**

## Run the code

```sh
  #Setup meson
  meson setup build/

  #Compile the code into
  ninja -C build/

  #Exec the program
  ./build/memdb

  #New Terminal and connect a client
  netcat localhost 8080
```

## How has it been coded?

This project **is not** a development one. I mean, it has been made just for learning a bit more about how does sockets works, and how does the kernel handle multiples connections to the same "server".

There are some libraries which has helped me to make this project posible. Some of them are:

- [sys/epoll](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [sys/socket](https://man7.org/linux/man-pages/man2/socket.2.html)
- [unistd](https://man7.org/linux/man-pages/man0/unistd.h.0p.html)
- [arpa/inet](https://man7.org/linux/man-pages/man3/inet.3.html)

Let me say, that if you are not familiar with `socket` and `epoll` programming, it might well be challenging for you to understand these code. That is why I'm going to lend you some resources, which would help.

These resorces are the followings:

- [Socket programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/)
- [Epoll in 3 easy steps](https://www.suchprogramming.com/epoll-in-3-easy-steps/)
- [Functional example with explanation](https://stackoverflow.com/questions/66916835/c-confused-by-epoll-and-socket-fd-on-linux-systems-and-async-threads)

##### <center>_Made with ❤️ by @Nimeavles using C++_</center>
