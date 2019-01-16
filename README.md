# MKS65C-FinalProject

## Checkers
Tina Wong, Maggie Zhao

## Description
The project is a multi-player checkers game. Along with the basic checkers game functionalities, such as allowing players to move across the board, the project will include a server-client connection.

For our minimum viable product, two users will be allowed to join the server. Once everyone has joined, the game will begin; the server splits up the users into two even(ish) teams—one team for the red checkers, and one for the white.

## Instructions to Run
1. Go to repo.
```
cd MKS65C-FinalProject/
```
2. Change directory into final project.
```
cd sockets_code
```
3. Compile code.
```
$ make
```
4. Run server.
```
$ ./server
```
5. Run client.
```
$ ./client
```

## Known Bugs
- Once the client joins the server, you can only say yes to continuing the game. You can say no, but the server does not know that the player has left and will continue the game regardless.
- Regular checkers can kill king checkers.
