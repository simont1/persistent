#include "pipe_networking.h"


static void sighandler(int signo){
  if (signo == SIGINT){
    remove("wkp");
    exit(0);
  }
}

int main() {

  int to_client;
  int from_client;
  int size_read;
  char str[BUFFER_SIZE];

  signal(SIGINT, sighandler);

  while (1){

    from_client = server_handshake( &to_client );

    while (1){

      size_read = read(from_client, str, sizeof(str));
      if ( !size_read )
      break;
      printf("Message Received: %s\n", str);
      char* dummy = str;
      while (*dummy){
        *dummy += 1;
        dummy ++;
      }
      write(to_client, str, sizeof(str));

    }
  }

  return 0;
}
