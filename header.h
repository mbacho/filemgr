/* 
 * File:   header.h
 * Author: barbossa
 *
 * Created on April 18, 2014, 2:36 AM
 */

#ifndef HEADER_H
#define	HEADER_H

#ifdef	__cplusplus
extern "C" {
#endif


#include<string.h> //string manipulation
#include<stdio.h> //I/O functions
#include<stdlib.h> //Standard functions
#include<sys/stat.h> //File stat functions
#include<getopt.h> //Cmdline option handling
#include<errno.h> //error handling
#include<unistd.h> //chdir, close
#include<sys/types.h> //types e.g. uint
#include<sys/socket.h>  //accept,recv,socket,bind,connect,send...
#include<netinet/in.h> //sockaddr data structures(netinet/in.h)
#include<arpa/inet.h>//inet_pton,....
#include<signal.h> //signal handling

#define BUF_MAX 10000 //max size of buffer
#define CONN_MAX 10 
#define PORT 8280
#define TRUE 1
#define CHECK_FIND '1' //find file option sent to server
#define SAVE_FILE '2' //save file option sent to server
#define MAX_FILNAME_SIZE 255
#define FILE_SEPARATOR  '/'
#define UP_FOLDER ".."
#define ROOT_FOLDER  "filesystem"
#define FILE_NOT_FOUND "404"

    void cleanup(int); //does cleanup after catching signals
#ifdef	__cplusplus
}
#endif

#endif	/* HEADER_H */

