#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>

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

int main()
{
  epoll_event ev;
  epoll_event events[MAX_EVENTS];

  int listener = create_listener();
  int epfd = epoll_create(MAX_EVENTS + 1);
  if (epfd < 0)
  {
    perror("epoll_create failed");
    exit(-1);
  }

  // listenerをepoll eventに追加
  memset(&ev, 0, sizeof(ev));
  ev.events = EPOLLIN;
  ev.data.fd = listener;
  epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev);

  printf("listen port %d\n", PORT);

  while (true)
  {
    int nfd = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < nfd; i++)
    {
      if (events[i].data.fd == listener)
      {
        sockaddr_in ca;
        socklen_t calen = sizeof(ca);
        int cfd = accept(listener, (sockaddr *)&ca, &calen);
        if (cfd < 0)
          continue;

        // non-blocking
        fcntl(cfd, F_SETFL, fcntl(cfd, F_GETFL, 0) | O_NONBLOCK);

        // clientをepoll eventへ追加
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN | EPOLLET; // 書き込み不能はほぼないので、EPOLLOUTはいらない
        ev.data.fd = cfd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);

        printf("client accepted %d\n", cfd);
      }
      else
      {
        int cfd = events[i].data.fd;
        uint8_t buf[1024];
        int n = read(cfd, buf, 1024);
        if (n <= 0)
        {
          printf("read close client %d\n", cfd);
          epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &ev);
          close(cfd);
        }
        else
        {
          n = write(cfd, buf, n);
          if (n < 0)
          {
            printf("write close client %d\n", cfd);
            epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, &ev);
            close(cfd);
          }
        }
      }
    }
  }
  return 0;
}