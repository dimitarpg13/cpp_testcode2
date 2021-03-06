#include <stdio.h>
#include <stdlib.h> /* for exit() */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> /* for accept() */
#include <sys/epoll.h>
#include <string.h>

#define MAX_EVENTS 10
#define EXIT_FAILURE -1


int main(int argc, char *argv[]) {
    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd, n;

    /* code to set up listening socket, 'listen_sock',
     * (socket(), bind(), listen()) omitted */

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
       perror("epoll_create1");
       exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
       perror("epoll_ctl: listen_sock");
       exit(EXIT_FAILURE);
    }

    for (n = 0; n < nfds; ++n) {
       if (events[n].data.fd == listen_sock) {
           conn_sock = accept(listen_sock, 
                               (struct sockaddr *) &addr, &addrlen);
           if (conn_sock == -1) {
              perror("accept");
              exit(EXIT_FAILURE);
           }
           setnonblocking(conn_sock);
           ev.events = EPOLLIN | EPOLLET;
           ev.data.fd = conn_sock;
           if (epoll_ctl(epolllfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
               perror("epoll_ctl: conn_sock");
               exit(EXIT_FAILURE);
           }
       } else {
          do_use_fd(events[n].data.fd);
       }

    }



    return 0;
}
