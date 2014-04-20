
/*
 * File:   server.c
 * Author: erico
 *
 * Created on March 27, 2013, 11:54 AM
 *
 * Purpose : tcp echo server
 */

#include"header.h"
#include<netdb.h> //gethostbyname

int sock_fd = -1, conn_fd = -1;

void putfile(const char*filepath, const char*outputfile);
void find(const char*filename, char*buf);
char*fileType(const struct stat*s);
void saveFile(const char*data, const char*filename, char*buf);

int main(int argc, char** argv) {
    struct sockaddr_in srv_socket, client_socket;
    signal(SIGINT, cleanup);

    if ((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket creation error");
        return EXIT_FAILURE;
    }

    srv_socket.sin_family = AF_INET;
    srv_socket.sin_port = htons(PORT);
    srv_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_fd, (struct sockaddr *) &srv_socket, sizeof (srv_socket)) < 0) {
        perror("binding error");
        return EXIT_FAILURE;
    }
    while (TRUE) {
        listen(sock_fd, CONN_MAX);
        char msg[BUF_MAX];
        socklen_t*size = malloc(sizeof (socklen_t));
        *size = sizeof (client_socket);

        if ((conn_fd = accept(sock_fd, (struct sockaddr*) &client_socket, size)) < 0) {
            perror("accept error");
            return EXIT_FAILURE;
        }
        free(size); //free mem

        fprintf(stdout, "Accepted %s...\n", inet_ntoa(client_socket.sin_addr));
        while (TRUE) {
            if (recv(conn_fd, msg, BUF_MAX - 1, 0) > 0) {
                fprintf(stdout, "\nReceiving transmission from %s...\n", inet_ntoa(client_socket.sin_addr));
                char ans[BUF_MAX];
                char*msg_ptr = msg;
                if (msg[0] == CHECK_FIND) {
                    find(++msg_ptr, ans);
                } else if (msg[0] == SAVE_FILE) {
                    char filename[MAX_FILNAME_SIZE];
                    memset(filename, '\0', sizeof (filename));
                    while ((*(++msg_ptr) != '\n') && strlen(filename) > MAX_FILNAME_SIZE - 2) {
                        filename[strlen(filename)-1] = *msg_ptr;
                    }
                    saveFile(msg_ptr, filename, ans);
                } else {
                    strcpy(ans, "action not specified");
                    printf(ans);
                }
                fprintf(stdout, "Sending transmission to %s...\n", inet_ntoa(client_socket.sin_addr));
                send(conn_fd, ans, strlen(ans), 0);
                memset(ans, '\0', sizeof (ans));
                memset(msg, '\0', sizeof (msg));
                sleep(1);
                break;
            }
        }
        fprintf(stdout, "Shutting down connection : %s\n", ((shutdown(conn_fd, SHUT_RDWR) != -1) ? "OK" : "FAIL"));
    }
    return EXIT_SUCCESS;
}

void cleanup(int sig) {
    printf("\n^C caught. Cleaning up...\n");
    if (conn_fd > 0)close(conn_fd);
    if (sock_fd > 0)close(sock_fd);
    exit(EXIT_SUCCESS);
}

void saveFile(const char*data, const char*filename, char*buf) {
    printf("writing to file %s\n", data);
    chdir(ROOT_FOLDER);
    FILE*fd = fopen(filename, "wb");
    fputs(data, fd);
    fclose(fd);
    chdir("..");
}

void find(const char*filename, char*buf) {
    printf("Finding file : %s\n", filename);
    struct stat *result = malloc(sizeof (struct stat));
    if (filename == NULL) {
        strcpy(buf, "filename not given\0");
        return;
    }
    chdir(ROOT_FOLDER);
    int status = stat(filename, result);
    if (status == -1) {
        fprintf(stdout, "File %s not found\n", filename);
        strcpy(buf, FILE_NOT_FOUND);
        return;
    }
    fprintf(stdout, "File %s found\n", filename);
    char created[20], last_modified[20];

    snprintf(created, sizeof (created) - 1, "%i", (int) result->st_ctim.tv_sec);
    snprintf(last_modified, sizeof (created) - 1, "%i", (int) result->st_mtim.tv_sec);
    memset(buf, '\0', BUF_MAX);
    snprintf(buf, BUF_MAX - 1, "filepath: /%s\nsize : %i bytes\ncreated : %s\nlast modified : %s\ntype : %s\n",
            filename, (int) result->st_size, created, last_modified, fileType(result));
    strcat(buf, "File Permissions: ");
    strcat(buf, (S_ISDIR(result->st_mode)) ? "d" : "-");
    strcat(buf, (result->st_mode & S_IRUSR) ? "r" : "-");
    strcat(buf, (result->st_mode & S_IWUSR) ? "w" : "-");
    strcat(buf, (result->st_mode & S_IXUSR) ? "x" : "-");
    strcat(buf, (result->st_mode & S_IRGRP) ? "r" : "-");
    strcat(buf, (result->st_mode & S_IWGRP) ? "w" : "-");
    strcat(buf, (result->st_mode & S_IXGRP) ? "x" : "-");
    strcat(buf, (result->st_mode & S_IROTH) ? "r" : "-");
    strcat(buf, (result->st_mode & S_IWOTH) ? "w" : "-");
    strcat(buf, (result->st_mode & S_IXOTH) ? "x" : "-");
    strcat(buf, "\n");

    printf(buf);
    chdir("..");
    free(result);
}

char*fileType(const struct stat*s) {
    return "folder/file";
}

