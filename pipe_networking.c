#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
  int server_handshake(int *to_client) {
    printf("Creating wkp...\n");
    mkfifo("wkp", 0644);
    printf("Waiting to receive from client...\n");
    int from_client = open("wkp", O_RDONLY);
    char pid[HANDSHAKE_BUFFER_SIZE];
    read(from_client, pid, sizeof(pid));
    printf("Message received! %s\n", pid);
    *to_client = open(pid, O_WRONLY);
    printf("Removing wkp...\n");
    remove("wkp");
    printf("Writing to client via private pipe...\n");
    write(*to_client, ACK, sizeof(ACK));
    printf("Waiting to receive from client...\n");
    char buf[HANDSHAKE_BUFFER_SIZE];
    read(from_client, buf, sizeof(buf));
    printf("Message received: %s\n", buf);
    return from_client;
  }


  /*=========================
    client_handshake
    args: int * to_server
    Performs the client side pipe 3 way handshake.
    Sets *to_server to the file descriptor for the upstream pipe.
    returns the file descriptor for the downstream pipe.
    =========================*/

  int client_handshake(int *to_server) {
    printf("Creating private pipe...\n");
    char pid[HANDSHAKE_BUFFER_SIZE];
    sprintf(pid, "%d", getpid());
    mkfifo(pid, 0644);

    printf("Writing PID...\n");
    *to_server = open("wkp", O_WRONLY);
    write(*to_server, pid, sizeof(pid));

    printf("Waiting to receive via private pipe...\n");
    int response = open(pid, O_RDONLY);
    char buf[HANDSHAKE_BUFFER_SIZE];
    read(response, buf, sizeof(buf));
    printf("Message received: %s\n", buf);

    printf("Removing private pipe...\n");
    remove(pid);

    printf("Writing to server...\n");
    write(*to_server, ACK, sizeof(ACK));

    return response;
  }
