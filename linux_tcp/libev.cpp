#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <ev.h>

#define PORT (5000)
#define MAX_EVENTS (4096)
#define BACKLOG (1024)

int create_listener()
{
  sockaddr_in sin;
  int sock = ::socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("create socket failed");
    exit(-1);
  }

  int yes = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(PORT);

  if (bind(sock, (sockaddr *)&sin, sizeof(sin)) < 0)
  {
    close(sock);
    perror("socket bind failed");
    exit(-1);
  }

  if (listen(sock, BACKLOG) < 0)
  {
    close(sock);
    perror("socket listen failed");
    exit(-1);
  }

  return sock;
}

void ev_client(EV_P_ struct ev_io *w, int revents)
{
  int cfd = w->fd;
  uint8_t buf[1024];
  int n = read(cfd, buf, 1024);
  if (n <= 0)
  {
    printf("read close client %d\n", cfd);
    goto close;
  }
  else
  {
    n = write(cfd, buf, n);
    if (n < 0)
    {
      printf("write close client %d\n", cfd);
      goto close;
    }
  }

  return;
close:
  ev_io_stop(EV_A_ w);
  close(cfd);
  free(w);
}

void ev_accept(EV_P_ struct ev_io *w, int revents)
{
  sockaddr_in ca;
  socklen_t calen = sizeof(ca);
  int cfd = accept(w->fd, (sockaddr *)&ca, &calen);
  if (cfd < 0)
  {
    perror("accept failed");
    exit(-1);
  }

  // non-blocking
  fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL, 0) | O_NONBLOCK);

  ev_io *client_watcher = (ev_io *)malloc(sizeof(ev_io));

  // client ev開始
  ev_io_init(client_watcher, ev_client, cfd, EV_READ);
  ev_io_start((struct ev_loop *)w->data, client_watcher);
}

int main()
{
  int listener = create_listener();
  printf("listen port %d\n", PORT);

  // ev初期化
  ev_io io_watcher;
  struct ev_loop *loop = ev_loop_new(0);
  io_watcher.data = loop;

  // ev_io初期化+開始
  ev_io_init(&io_watcher, ev_accept, listener, EV_READ);
  ev_io_start(loop, &io_watcher);

  // ev ループ開始
  ev_loop(loop, 0);

  ev_loop_destroy(loop);
  close(listener);
  return 0;
}