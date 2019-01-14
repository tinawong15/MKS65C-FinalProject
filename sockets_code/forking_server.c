#include "networking.h"
#include "game.h"

int total_players;
void process(char *s);
void subserver(int from_client);
void to_all_clients(struct client clients[], int num_players, char* msg);

static void sighandler(int signo){
  if (signo == SIGINT){
    printf("Closing pipes...\n");
    if(unlink("to_client") != -1){
      printf("Pipe to_client deleted.\n" );
    }
    if(unlink("to_parent") != -1){
      printf("Pipe to_parent deleted.\n" );
    }
    exit(0);
  }
}

int main() {

  int listen_socket;
  int joined = 0;
  int i;
  char buffer[BUFFER_SIZE];
  listen_socket = server_setup();

  struct client player;
  struct client clients[MAX];
  char msg[BUFFER_SIZE];
  char total[2];
  while (total_players < MAX) {
    printf("Waiting for players to connect...\n");
    int client_socket = server_connect(listen_socket);

    strcpy(buffer, "[server] Please type your name.");
    write(client_socket, buffer, sizeof(buffer));

    write(client_socket, "1", sizeof("1"));

    read(client_socket, buffer, sizeof(buffer));

    strcpy(player.name, buffer);
    player.client_socket = client_socket;
    clients[total_players] = player;
    total_players++;

    strcpy(msg, player.name);
    strcat(msg, " has joined the game.");
    printf("%s\n", msg);

    if(total_players > 1) {
      strcpy(msg, "\n");
      sprintf(total, "%d", total_players);
      strcat(msg, total);
      strcat(msg, " players have joined the game. Continue? (y/n)?");
      joined = 0;

      for(i = 0; i < total_players; i++) {
        write(clients[i].client_socket, msg, sizeof(msg));
        write(clients[i].client_socket, "1", sizeof("1"));
        read(clients[i].client_socket, buffer, sizeof(buffer));

        if(!strcmp(buffer, "y")) {
          joined++;
        }
        else {
          strcpy(msg, "Player ");
          strcat(msg, clients[i].name);
          strcat(msg, " has left.");
          to_all_clients(clients, total_players, msg);
          close(clients[i].client_socket);
        }
      }

      if (joined == total_players){
        break;
      }
    }
  }

  to_all_clients(clients, total_players, "\nWelcome to Checkers!\n");
  write(clients[i].client_socket, "0", sizeof("0"));
  
  char board[65];
  strcpy(board, init_board());
  // printf("%s\n", board);
  for(i = 0; i < total_players; i++) {
    if(i % 2 == 0) {
      clients[i].team = 1;
    }
    else {
      clients[i].team = 2;
    }
  }
  // for (i = 0; i < player_num; i++){
  //   printf("%d\n", clients[i].team);
  // }
  play_game(clients, total_players, board);
}

void to_all_clients(struct client clients[], int num_players, char* msg){
   int i;
   char buffer[BUFFER_SIZE];
   strcpy(buffer, msg);
   for (i = 0; i < num_players; i++){
     write(clients[i].client_socket, buffer, sizeof(buffer));
     write(clients[i].client_socket, "n", sizeof("n"));
   }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
