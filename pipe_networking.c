#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  remove("server");
  int fd;
  if (mkfifo("server", 0644) == -1) {
    printf("Error with mkfifo: %s\n", strerror(errno));
    exit(0);
  }
  printf("Well known pipe created!\n");

  printf("Wait for Client...\n");
  fd = open("server", O_RDONLY);
  printf("Well known pipe opened: %d\n", fd);
  char message[HANDSHAKE_BUFFER_SIZE];
  read(fd, message, sizeof(message));
  printf("Message from the Client: %s\n", message);

  int f = fork();
  // parent
  if(f == 0) {
    printf("Remove well known pipe...\n");
    remove("server");
  }
  // child server
  else {
    printf("Write back to Client...\n");
    *to_client = open(message, O_WRONLY);
    write(*to_client, ACK, sizeof(ACK));

    printf("Wait for Client...\n");
    printf("Private pipe opened: %d\n", fd);
    char response[HANDSHAKE_BUFFER_SIZE];
    read(fd, response, sizeof(response));
    printf("Response from the Client: %s\n", response);

    return fd;
  }
  return 0;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int fd;
  if (mkfifo("private", 0644) == -1) {
    printf("Error with mkfifo: %s\n", strerror(errno));
    exit(0);
  }
  printf("Private pipe created!\n");

  printf("Write to Server about Private Pipe...\n");
  *to_server = open("server", O_WRONLY);
  write(*to_server, "private", sizeof("private"));

  fd = open("private", O_RDONLY);
  printf("Wait for Server...\n");
  char message[HANDSHAKE_BUFFER_SIZE];
  read(fd, message, sizeof(message));
  printf("Message from the Server: %s\n", message);
  remove("private");

  printf("Write to Server to Acknowledge...\n");
  write(*to_server, ACK, sizeof(ACK));
  return fd;
}
