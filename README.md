This is a chatroom in C using TCP connections (only in UNIX systems, as it uses POSIX sockets).

To use it, execute the server in ./out/server and then as many clients as you want with out/client \[address\] \[port\].
By default, the server is linked to 127.0.0.1:34197, to use it in local network as an example, but it can be configured to use any available port.
To connect to a server from another network, use ./client \[public ip\]:34197. Remember to open your port in your router if you want to try so!

It was done following the excellent tutorial in Chapter 3 of the Book [Hands-On Network Programming with C](https://www.amazon.com/exec/obidos/ASIN/1789349869/aoeu-20) by Lewis Van Winkle. Absolute gem of a book if you are into Netwroking and C, definetely recommend reading it. 

There is a make command to test it localy: make exec. It opens a terminal window with the server and 3 terminal windows with clients to test it out. It uses gnome-terminal as command for the terminal, so it might not work if you don't have it installed (or you are not using Linux altogether).
