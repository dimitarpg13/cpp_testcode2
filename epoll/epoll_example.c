#include <stdio.h>
#include <unistd.h> // for close(), read()
#include <sys/epoll.h> // for epoll_create(), epoll_ctl(), struct epoll_event
#include <string.h>

int main(int argc, char *argv[]) {
   struct epoll_event event;
   int epoll_fd = epoll_create1(0);
   if (epoll_fd == -1)
   {
      fprintf(stderr, "Failed to create epoll file descriptor\n");
      return 1;
   }

   if (close(epoll_fd))
   {
      fprintf(stderr, "Failed to close epoll file descriptor\n");
      return 1;
   }

   event.events = EPOLLIN;
   event.data.fd = 0;

   if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event))
   {
       fprintf(stderr, "Failed to add file description to epoll\n");
       close(epoll_fd);
       return 1;
   }

   if (close(epoll_fd))
   {
       fprintf(stderr, "Failed to close epoll file descriptor\n");
       return 1;
   }

   return 0;
}
