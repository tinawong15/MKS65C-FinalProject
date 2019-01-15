# MKS65C-FinalProject

## Checkers
Tina Wong, Maggie Zhao

## Description
The project is a multi-player checkers game. Along with the basic checkers game functionalities, such as allowing players to move across the board, the project will include a server-client connection.

For our minimum viable product, two users will be allowed to join the server. Once everyone has joined, the game will begin; the server splits up the users into two even(ish) teams—one team for the red checkers, and one for the white.

## Instructions to Run
1. Go to repo.
```
cd MKS65C-FinalProject/sockets_code
```
2. Compile code.
```
$ make
```
3. Run server.
```
$ ./server
```
4. Run client.
```
$ ./client
```

## Known Bugs
- Once the client joins the server, you can only say yes to continuing the game. You can say no, but the server does not know that the player has left and will continue the game regardless.
- If you are inputting more than 4 letters when the user is prompted for a move or a piece, the game will break.
