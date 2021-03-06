#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>

void error(char *msg)
{
  fprintf(stderr, "%s:%s\n", msg, strerror(errno));
  exit(1);
}

int read_line(int socket, char *buf, int len)
{
  char *s = buf;
  int slen = len;
  int c = read(socket, s, slen);
  while ((c > 0) && (s[c - 1] != '\n')) {
    s += c;
    slen = -c;
    c = read(socket, s, slen);
  }
  if (c < 0) {
    return c;
  }
  return len - slen;
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

  if (listen(listener_d, 1) == -1) {
    error("listen err");
  }

  puts("wait...");

  struct sockaddr_storage client_addr;
  unsigned int address_size = sizeof(client_addr);

  // Listenソケットをノンブロッキングにする
  int val = 1;
  if (ioctl(listener_d, FIONBIO, &val) == -1) {
    error("ioctl err");
  }

  // 接続済みソケットを管理するための配列
  int fds[255];
  // 接続済みソケットの数
  int n = 0;

  char buf[255];
  while(1) {
    int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if (connect_d < 0) {
      // クライアントの接続がない場合は EWOULDBLOCK
      if (errno != EWOULDBLOCK) {
        error("accept err");
      }
    } else {
      // 接続済みソケットをノンブロッキングにする
      if (ioctl(connect_d, FIONBIO, &val) == -1) {
        error("ioctl err");
      }
      fds[n] = connect_d;
      n++;

      char *msg = "Hello World!\r\n";
      write(connect_d, msg, strlen(msg));
    }

    int i = 0;
    while (i < n) {
      // 接続済みソケットを順番に処理
      int conn = fds[i];
      if (read_line(conn, buf, sizeof(buf)) < 0) {
        // データが届いてない場合は EAGAIN
        if (errno != EAGAIN) {
          error("read err");
        }
        i++;
      } else {
        // データが届いてる場合はechoしてclose
        write(conn, buf, strlen(buf));
        close(conn);
        n--;
      }
    }
  }
  return 0;
}

