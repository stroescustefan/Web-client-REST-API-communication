#include <stdlib.h> /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(const char *host, const char *url, const char *query_params,
                          char **cookies, int cookies_count,const char *JWT) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL && strcmp(query_params, "") != 0) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (JWT != NULL && strcmp(JWT, "") != 0) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    if (cookies != NULL)
    {
        sprintf(line, "Cookie: ");   
        for (int i = 0; i < cookies_count; i++) {
            strcat(line, cookies[i]);
        }
        compute_message(message, line);
    }
    // Step 4: add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(const char *host, const char *url, const char *content_type
                        , const char *body_data, char **cookies, int cookies_count, const char *JWT)
{
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */
    if (content_type != NULL && strcmp(content_type, "") != 0) {
        sprintf(line, "Content-Type: %s", content_type);
        compute_message(message, line);
    }

    if (body_data != NULL && strcmp(body_data, "") != 0) {
        sprintf(line, "Content-Length: %ld", strlen(body_data));
        compute_message(message, line);
    }

    if (JWT != NULL && strcmp(JWT, "") != 0) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    // Step 4 (optional): add cookies
    if (cookies != NULL)
    {
        for (int i = 0; i < cookies_count; i++)
        {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
        }
    }
    // Step 5: add new line at end of header
    compute_message(message, "");
    // Step 6: add the actual payload data
    if (body_data != NULL) {
        memset(line, 0, LINELEN);
        compute_message(message, body_data);
    }
    free(line);
    return message;
}

char *compute_delete_request(const char *host, const char *url, const char *query_params, const char *JWT) {
    char *message = (char *)calloc(BUFLEN, sizeof(char));
    char *line = (char *)calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type
    if (query_params != NULL && strcmp(query_params, "") != 0) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    // Step 3 (optional): add headers and/or cookies, according to the protocol format
    if (JWT != NULL && strcmp(JWT, "") != 0) {
        sprintf(line, "Authorization: Bearer %s", JWT);
        compute_message(message, line);
    }

    // Step 4: add final new line
    compute_message(message, "");
    return message;
}
