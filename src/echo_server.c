#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error(char *msg)
{
  fprintf(stderr, "%s:%s\n", msg, strerror(errno));
  exit(1);
}

int main(int argc, char *argv[])
{
  int listener_d = socket(PF_INET, SOCK_STREAM, 0);
  if (listener_d == -1) {
    error("socket err");
  }

  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = (in_port_t)htons(30000);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(listener_d, (struct sockaddr *) &name, sizeof(name)) == -1) {
    error("bind err");
  }

  puts("wait...");
  while(1) {
  }
  return 0;
}

