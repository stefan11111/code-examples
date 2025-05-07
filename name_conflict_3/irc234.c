#include <netdb.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER "irc.libera.chat"
#define PORT "6697"

int main(void) {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error creating socket");
    exit(1);
  }

  struct addrinfo hints, *res, *addr;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(SERVER, PORT, &hints, &res);
  if (status != 0) {
    (void)fprintf(stderr, "error: %s\n", gai_strerror(status));
    exit(1);
  }

  for (addr = res; addr != NULL; addr = addr->ai_next) {
    sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sockfd == -1) {
      perror("Error creating socket");
      continue;
    }

    if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) == -1) {
      (void)close(sockfd);
      perror("Error connecting to server");
      continue;
    }

    break;
  }

  if (addr == NULL) {
    (void)fprintf(stderr, "Error connecting to server\n");
    freeaddrinfo(res);
    exit(1);
  }

  freeaddrinfo(res);

  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  const SSL_METHOD *method = TLS_client_method();
  SSL_CTX *ctx = SSL_CTX_new(method);
  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    exit(1);
  }

  SSL *ssl = SSL_new(ctx);
  if (ssl == NULL) {
    ERR_print_errors_fp(stderr);
    SSL_CTX_free(ctx);
    exit(1);
  }

  SSL_set_fd(ssl, sockfd);

  if (SSL_connect(ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    SSL_free(ssl);
    (void)close(sockfd);
    SSL_CTX_free(ctx);
    exit(1);
  } else {
    printf("Here\n");
  }

  SSL_free(ssl);
  (void)close(sockfd);
  SSL_CTX_free(ctx);
  EVP_cleanup();
  return 0;
}
