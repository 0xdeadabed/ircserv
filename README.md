# ircserv
An IRC (Internet Relay Chat) server in C++ 98.

## Execution order:
```sh
./ircserv <port> <password>
```
- port: the port on which the IRC server will be listening to for incoming IRC connections.
- password: the connection password, any IRC client should have one in order to connect to the server.

## NOTTODO
- Develop a client.
- Handle server-to-server communication.
- Forking and block operations.
- Writing spaghetti code

## TODO

- Develop an IRC server in c++ 98.
- Handle multiple clients at the same time.
- Use only 1 poll() (or equivalent) for handling all these operations (read, write, listen, and so forth).
- Choosing and IRC client as reference.
- Connecting te client with the server without encountering any error.
- Implement communication between client and server via TCP/IP (v4 or v6).
- The client must be able to connect to the server just like any other official IRC server.
- Implement the following features:
	- Authentication, nickname, username.
	- Joining a channel, send and recieve private message using the client.
	- Forwarding all the messages sent from one client to everyone else that joined the channel.
	- Implement operators and regular users.
	- Implement the commands that are specific to operators.


## MacOS only
- Since MacOS doesn't implement write() the same way as other Unix OSes, the fcntl() should be used.
- The implementation of fcntl should be like `fcntl(fd, F_SETFL, O_NONBLOCK)`, any other flag is forbidden.


## Tests
- Verify absolutely every possible error and issue (recieving partial data, low bandwidth, and so forth).
- To ensure that the server correctly processes everything that we send to it, the following test could be used.
```sh
$ nc 127.0.0.1 <port>
com^Dman^Dd
$
```
- Use ctrl+D to send the command in several parts: 'com', then 'man', then 'd\n'.
- In order to process a command, aggregate (COLLECT) the received packets in order to rebuild it.


## Bonus

- Handle file transfer.
- A bot.
