#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sockfd = 0;
  char recv[1024];
  struct sockaddr_in serverAddress;
  memset(&recv, 0, sizeof(recv));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Error : Could not create socket \n");
    exit(1);
  }
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(31337);
  inet_pton(AF_INET, "128.186.120.190", &serverAddress.sin_addr); // Change this to the address given
  if(connect(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    printf("Couldn't connect...\n");
    exit(1);
  }
 
  write(sockfd, "-E", 2);     // Encryption of the original message
  read(sockfd, &recv, 1024);
  printf("%s\n", recv);

  write(sockfd, "-E abcdef0123456789", 19);     // Encryption of abcdef0123456789 || message
  memset(&recv, 0, 1024);
  read(sockfd, &recv, 1024);
  printf("%s\n", recv);

  write(sockfd, "-V 6a9215b64a244b92b84dbe7cc58a5d02e8f029f432f8e931c8add86808118c5c8af29756f6d6ff85ae16ad77dc0b8221816c1dda9825f9a407b03d3f9a817160715f0b89abf9f1213bf45f464730b810 659a4a7cf4273befce277e5336202d69", 196); // Valid verification
  memset(&recv, 0, 1024);
  read(sockfd, &recv, 1024);
  printf("%s\n", recv);

  write(sockfd, "-V 59ab959fb85957208a5d705e9c8a89752d593c0036b5006a96aec8dbcebc46a52d990503387e347104c79e87015bc58f42f3caf0e4eabfe896f940b85ccd5fd1 22301cf11d01ff6dc2d3860484189b17", 164); // Invalid verification (last bit flipped)
  memset(&recv, 0, 1024);
  read(sockfd, &recv, 1024);
  printf("%s\n", recv);

  close(sockfd);
  return 0;
}
