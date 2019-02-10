#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
   perror(msg);
   exit(0);
}

int main(int argc, char *argv[])
{
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   /*
    * struct hostent
    * {
    *    char   *h_name;        // official name of host
    *    char   **h_aliases;    // alias list
    *    int    h_addrtype;     // host address type 
    *    char   h_length;       // length of address
    *    char   **h_addr_list;  // list of addresses from name server
    *    #define h_addr h_addr_list[0]  // address for backward compatibility
    * };
    *
    * it defines a host computer on the Internet. The members are:
    * h_name        official name of the host
    * h_aliases     a zero terminated array of alternate names for the host
    * h_addrtype    the type of address being returned
    * h_length      the length in bytes of the address
    * h_addr_list   a pointer to a list of network addresses for the named
    *               host. Host addresses are return in network byte order
    * Note that h_addr is an alias for the first address in the array of network addresses
    */

    char buffer[256];
    if (argc < 3)
    {
       fprintf(stderr, "usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    /* the variable argv[1] contains the name of the host on the internet e.g. cs.rpi.edu.
     * The function
     * struct hostent *gethostbyname(char *name)
     * takes such a name as an argument and returns a pointer to a hostent containing information
     * about the host. The field char * h_addr contains the IP address.
     * If the strucutre is NULL, the system could not locate a host with this name.
     * In the old days this function worked by searching a system file called /etc/hosts but with the
     * explosive growth of internet the function involes querying large dbs around the country
     */
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    const struct sockaddr * pserv_addr = (struct sockaddr *) &serv_addr;
    if (connect(sockfd, pserv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter a message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n", buffer);
    return 0;
    
}
