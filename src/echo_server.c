#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(char *msg)
{
  fprintf(stderr, "%s:%s\n", msg, strerror(errno));
  exit(1);
}

int main(int argc, char *argv[])
{
  puts("wait...");
  while(1) {
  }
  return 0;
}

