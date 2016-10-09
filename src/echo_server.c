#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>

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
  puts("wait...");
  while(1) {
  }
  return 0;
}

