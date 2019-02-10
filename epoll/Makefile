all: epoll_example simple_tcp_server simple_tcp_client #epoll_sample

epoll_example: epoll_example.c
	gcc -Wall -Werror -o $@ epoll_example.c

epoll_sample: epoll_sample.c
	gcc -Wall -Werror -o $@ epoll_sample.c

simple_tcp_server: simple_tcp_server.c
	gcc -Wall -Werror -o $@ simple_tcp_server.c

simple_tcp_client: simple_tcp_client.c
	gcc -Wall -Werror -o $@ simple_tcp_client.c

clean:
	@rm -v epoll_example
	@rm -v epoll_sample
	@rm -v simple_tcp_server
	@rm -v simple_tcp_client

