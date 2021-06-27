#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include <iostream>
#include <string>
#include <json.hpp>


using json = nlohmann::json;
using namespace std;

/**
 * Function that checks if a string is a number or not.
 * @param str - string to be checked.
 * @return - true(if the given string is a number), false(otherwise).
 */ 
bool isNumber(string str)
{
    for (auto chr : str) {
        if (!isdigit(chr)) {
            return false;
        }
    }
    return true;
}


int main(int argc, char *argv[]) {
    char *message;
    char *response;
    int sockfd;

    // saved cookie and token
    char cookie[500];
    char token[500];
    memset(cookie, 0, 500);
    memset(token, 0, 500);

    while (1) {
        string command;

        getline(cin, command);
        if (command == "register") {
            json credentials;
            string username;
            string password;
            char data[1000];

            // read username and password
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password); 

            // create json object.
            credentials["username"] = username;
            credentials["password"] = password;
            strcpy(data, credentials.dump().c_str());

            // post request with json object as string(data)
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/register", "application/json"
                                            , data, NULL, 0, NULL);

            // printing messsage                                
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            // print response from server
            puts(response);
            close_connection(sockfd);

        } else if (command == "login") {
            json credentials;
            string username;
            string password;
            char data[1000];

            // if you were previously logged in on another account
            memset(cookie, 0, 500);
            memset(token, 0, 500);

            // read username and password
            cout << "username=";
            getline(cin, username);
            cout << "password=";
            getline(cin, password);

            // create json object. 
            credentials["username"] = username;
            credentials["password"] = password;
            strcpy(data, credentials.dump().c_str());

            // post request with json object as string(data)
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/login", "application/json"
                                            , data, NULL, 0, NULL);

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            // parsing the cookie
            char *p = strstr(response, "connect.sid");
            if (p != NULL) {
                int index = 0;
                int size = 0;
                memset(cookie, 0, 500);
                while (p[index] != ';') {
                    cookie[size++] = p[index++];
                }
                cookie[size] = p[index];
            }

            close_connection(sockfd);

        } else if (command == "enter_library") {

            // checking for cookie existence
            // send post request with cookie(if exists)
            if (cookie != NULL && strcmp(cookie, "") != 0) {
                char **cookies = (char **)malloc(sizeof(char *));
                cookies[0] = (char *)malloc(500 * sizeof(char));
                strcpy(cookies[0], cookie);
                sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access", NULL
                                            , cookies, 1, NULL);
            } else {
                sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access", NULL
                                            , NULL, 0, NULL);
            }

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            // token parsing
            if (cookie != NULL && strcmp(cookie, "") != 0) {
                string aux(strstr(response, "{"));
                json j = json::parse(aux);
                string token_aux = j["token"];
                strcpy(token, token_aux.c_str());
            }

            close_connection(sockfd);
            
        } else if (command == "get_books") {

            // get request with JWT
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_get_request("34.118.48.238", "/api/v1/tema/library/books", NULL
                                            , NULL, 0, token);

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
            close_connection(sockfd);
      
        } else if (command == "get_book") {
            string id, path;
            char path_array[200];

            // correctness check id
            cout << "id=";
            getline(cin, id);
            if (!isNumber(id)) {
                while (!isNumber(id)) {
                    cout << "id=";
                    getline(cin, id);
                }
            }

            // build url + get request with JWT.
            path = "/api/v1/tema/library/books/" + id;
            strcpy(path_array, path.c_str());
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_get_request("34.118.48.238", path_array, NULL
                                            , NULL, 0, token);

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
            close_connection(sockfd);

        } else if (command == "add_book") {
            string title, author, genre, publisher, page_count;
            json book;
            char data[1000];

            // read title, author, genre, publisher, page_count
            cout << "title=";
            getline(cin, title);
            cout << "author=";
            getline(cin, author);
            cout << "genre=";
            getline(cin, genre);
            cout << "publisher=";
            getline(cin, publisher);
            cout << "page_count=";
            getline(cin, page_count);

            // check for page_count correctness
            if (!isNumber(page_count)) {
                while (!isNumber(page_count)) {
                    cout << "page_count=";
                    getline(cin, page_count);
                }
            }

            // build json object(book)
            book["title"] = title;
            book["author"] = author;
            book["genre"] = genre;
            book["publisher"] = publisher;
            book["page_count"] = page_count;
            strcpy(data, book.dump().c_str());

            // post request with json object as string and JWT.
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/library/books", "application/json"
                                            , data, NULL, 0, token);

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            close_connection(sockfd);

        } else if (command == "delete_book") {
            string id, path;
            char path_array[200];

            // check for id correctness
            cout << "id=";
            getline(cin, id);
            if (!isNumber(id)) {
                while (!isNumber(id)) {
                    cout << "id=";
                    getline(cin, id);
                }
            }
            path = "/api/v1/tema/library/books/" + id;
            strcpy(path_array, path.c_str());


            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_delete_request("34.118.48.238", path_array, NULL, token);

           // print sent message and response from server 
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            close_connection(sockfd);

        } else if (command == "logout") {

            // check for cookie existence + get request with cookie
            if (cookie != NULL && strcmp(cookie, "") != 0) {
                char **cookies = (char **)malloc(sizeof(char *));
                cookies[0] = (char *)malloc(500 * sizeof(char));
                strcpy(cookies[0], cookie);
                sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout", NULL
                                            , cookies, 1, NULL);
            } else {
                sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout", NULL
                                            , NULL, 0, NULL);
            }

            // print sent message and response from server
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            // delete cookie
            strcpy(cookie, "");
            //delete JWT
            strcpy(token, "");

            close_connection(sockfd);

        } else if (command == "exit") {
            close_connection(sockfd);
            exit(0);
        }
    }

    return 0;
}