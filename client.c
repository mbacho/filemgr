
#include"header.h"

int sock_fd = -1;

void usage(char*prog_name);
void sendFind(const char*hostname, const char*filename);
void sendFile(const char*hostname, const char*filepath);
void checkOpts(int argc, char**argv);
void getFilename(const char*filepath, char*buf);

int main(int argc, char** argv) {
    signal(SIGINT, cleanup);
    checkOpts(argc, argv);
    return (EXIT_SUCCESS);
}

void sendFile(const char*hostname, const char*filepath) {
    char srv[BUF_MAX], msg[BUF_MAX];
    memset(msg, '\0', sizeof (msg));
    msg[0] = SAVE_FILE;
    FILE*fd = fopen(filepath, "rb");
    if (fd == NULL) {
        fprintf(stderr, "file open error : ");
        return;
    }
    char filename[FILENAME_MAX];
    getFilename(filepath, filename);
    strncat(msg, filename, FILENAME_MAX);
    strcat(msg, "\n");
    int rc = 0;
    while (((rc = fgetc(fd)) != EOF) && (strlen(msg) < BUF_MAX - 3)) {
        msg[(int) strlen(msg)] = (char) rc;
    }
    fclose(fd);
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket creation error");
        return;
    }
    struct sockaddr_in inet_socket;

    inet_socket.sin_family = AF_INET;
    inet_socket.sin_port = htons(PORT);
    inet_pton(AF_INET, hostname, &inet_socket.sin_addr);

    if (connect(sock_fd, (struct sockaddr*) &inet_socket, sizeof (inet_socket)) != 0) {
        perror("Error connecting to server");
        return;
    }
    if (send(sock_fd, msg, strlen(msg), 0) < 0) {
        perror("Send error");
    }
    memset(msg, '\0', sizeof (msg));
    if (recv(sock_fd, srv, sizeof (srv) - 1, 0) <= 0) {
        perror("Receive error");
    } else {
        printf("%s\n", srv);
        memset(srv, '\0', sizeof (srv));
    }
}

void sendFind(const char*hostname, const char*filename) {
    if ((sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket creation error");
        return;
    }
    struct sockaddr_in inet_socket;

    inet_socket.sin_family = AF_INET;
    inet_socket.sin_port = htons(PORT);
    inet_pton(AF_INET, hostname, &inet_socket.sin_addr);

    if (connect(sock_fd, (struct sockaddr*) &inet_socket, sizeof (inet_socket)) != 0) {
        perror("Error connecting to server");
        return;
    }
    char srv[BUF_MAX], msg[BUF_MAX];
    msg[0] = CHECK_FIND;
    strcat(msg, filename);
    if (send(sock_fd, msg, strlen(msg), 0) < 0) {
        perror("Send error");
    }
    memset(msg, '\0', sizeof (msg));
    if (recv(sock_fd, srv, sizeof (srv) - 1, 0) <= 0) {
        perror("Receive error");
    } else {
        if (strcasecmp(srv, FILE_NOT_FOUND) == 0) {
            printf("%s: file not found\n", srv);
        } else {
            printf("%s\n", srv);
        }

        memset(srv, '\0', sizeof (srv));
    }
}

void cleanup(int sig) {
    printf("\n^C caught. Cleaning up...\n");
    if (sock_fd > 0)close(sock_fd);
    exit(EXIT_SUCCESS);
}

void checkOpts(int argc, char**argv) {
    char*hostname = NULL;
    char*find = NULL;
    char*put = NULL;

    while (TRUE) {
        const char*short_options = "h:f:p:";
        struct option long_options[] = {
            {"hostname", required_argument, 0, 'h'},
            {"find", required_argument, 0, 'f'},
            {"put", required_argument, 0, 'p'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        int c = getopt_long(argc, argv, short_options, long_options, &option_index);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                hostname = optarg;
                break;
            case 'f':
                find = optarg;
                break;
            case 'p':
                put = optarg;
                break;
            default:
                printf("Invalid option\n");
                usage(argv[0]);
        }
    }

    if (hostname == NULL) {
        printf("server host not set\n");
        usage(argv[0]);
    }
    if (find == NULL && put == NULL) {
        printf("find / put not set\n");
        usage(argv[0]);
    }
    if (find != NULL) {
        sendFind(hostname, find);
    } else if (put != NULL) {
        sendFile(hostname, put);
    }

}

void usage(char*prog_name) {
    printf("Usage : %s hostname command [option]\n", prog_name);
    printf("command : find => find shit\n");
    printf("command : put => put shit\n");
    exit(0);
}

void getFilename(const char*filepath, char*buf) {
    char*last_sep = strrchr(filepath, FILE_SEPARATOR);
    if (last_sep == NULL) {
        strncpy(buf, filepath, MAX_FILNAME_SIZE - 2);
    } else {
        strncpy(buf, ++last_sep, MAX_FILNAME_SIZE - 2);
    }
}

