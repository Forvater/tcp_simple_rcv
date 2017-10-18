#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include <time.h>

#define BUFLEN 1024



void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(output, "[%d %d %d %d:%d:%d]",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

int main( int argc, char *argv[] )
{
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   char time_char[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   int i;
    FILE* fp;
    fp = fopen ("tcp_received.log", "a+");
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 20050;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   



    while(1)
    {
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
   
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("ERROR on acceptT");
            exit(1);
        }
   
        /* If connection is established then start communicating */
        bzero(buffer,256);
        n = read( newsockfd,buffer,255 );
   
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
   
        
        bzero(time_char,256);
        format_time(time_char);

        fprintf(fp,"%s : Received packet from %s:%d [Data ASCII: %s] ", time_char, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port),buffer);

        fprintf(fp,"Data hex: ");
        for (i=0;i<n;i++)
        {
            fprintf(fp,"0x%02x ", buffer[i]);
        }
        fprintf(fp,"\n");
//        memset(buffer, 0, BUFLEN);
        fflush(fp);



   
        /* Write a response to the client */
//        n = write(newsockfd,"I got your message",18);
   
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }
    }
      
   return 0;
}
