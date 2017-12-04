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

#include <ev++.h>
#include <iostream>

#define PORT (5000)
#define MAX_EVENTS (4096)
#define BACKLOG (1024)

class client
{
  ev::io watcher;
  int fd;

public:
  explicit client(int _fd) : fd(_fd)
  {
    // non-blocking
    ::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    watcher.set<client, &client::echo>(this);
    watcher.set(fd, ev::READ);

    std::cout << "client start " << fd << std::endl;
    watcher.start();
  }
  ~client()
  {
    watcher.stop();
    ::close(fd);
    std::cout << "client close " << fd << std::endl;
  }

  void echo(ev::io &w, int)
  {
    uint8_t buf[1024];
    int n = ::read(fd, buf, 1024);
    if (n <= 0)
    {
      printf("read close client %d\n", fd);
      delete this;
    }
    else
    {
      n = ::write(fd, buf, n);
      if (n < 0)
      {
        printf("write close client %d\n", fd);
        delete this;
      }
    }
  }
};

class server
{
  int fd_;

public:
  server()
  {
    sockaddr_in sin;
    fd_ = ::socket(PF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
      throw std::runtime_error("create socket failed");
    }

    int yes = 1;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);

    if (::bind(fd_, (sockaddr *)&sin, sizeof(sin)) < 0)
    {
      ::close(fd_);
      throw std::runtime_error("socket bind failed");
    }

    if (::listen(fd_, BACKLOG) < 0)
    {
      ::close(fd_);
      throw std::runtime_error("socket listen failed");
    }
  }

  ~server()
  {
    ::close(fd_);
  }

  void accept(ev::io &w, int)
  {
    sockaddr_in ca;
    socklen_t calen = sizeof(ca);
    int cfd = ::accept(fd_, (sockaddr *)&ca, &calen);
    if (cfd < 0)
    {
      throw std::runtime_error("accept failed");
    }

    new client(cfd);
  }

  int fd() { return fd_; }
};

int main()
{
  server s;

  // ev初期化
  ev::default_loop loop;
  ev::io watcher(loop);

  watcher.set<server, &server::accept>(&s);
  watcher.start(s.fd(), ev::READ);

  loop.run(0);
  return 0;
}