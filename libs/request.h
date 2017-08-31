#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

void error(const char *msg) { perror(msg); exit(0); }

char *http_get(char *host,
                char *path,
                char *data,
                char *headers) {
    int i;
    
    int portno = 80;
    char *method = "GET";

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total, message_size;
    char *message, response[4096];
    
    /* 计算需要多少空间 */
    message_size = 0;
    message_size += strlen("%s %s%s%s HTTP/1.0\r\n");
    message_size += strlen(path);
    if (strlen(data)) {
        message_size += strlen(data);
    }
    if (strlen(headers)) {
        message_size += strlen(headers);
    }
    message_size += strlen("\r\n");

    /* 申请空间 */
    message = malloc(message_size);

    /* 拼接请求 */
    sprintf(message, "%s %s?%s HTTP/1.0\r\n", method, path, data);
    if (headers) {
        strcat(message, headers);
        strcat(message, "\r\n");
    }
    strcat(message, "\r\n");

    /* 创建socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    
    /* 检测host */
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    /* 填充地址信息 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    /* 创建socket连接 */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* 发送请求 */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd, message + sent, total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    } while (sent < total);

    /* 接收回复 */
    memset(response, 0, sizeof(response));
    total = sizeof(response) - 1;
    received = 0;
    do {
        bytes = read(sockfd, response + received, total - received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    if (received == total)
        error("ERROR storing complete response from socket");

    /* 关闭socket */
    close(sockfd);
    free(message);

    return response;
}