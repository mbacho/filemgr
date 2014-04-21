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


#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>
#include <errno.h>
#include<unistd.h>
#include <sys/types.h> //types e.g. uint
#include <sys/socket.h>  //accept,recv,socket,bind,connect,send...
#include<netinet/in.h> //sockaddr data structures(netinet/in.h)
#include<arpa/inet.h>//inet_pton,....
#include<signal.h>

#define BUF_MAX 10000
#define CONN_MAX 10
#define PORT 8280
#define TRUE 1
#define CHECK_FIND '1'
#define SAVE_FILE '2'
#define MAX_FILNAME_SIZE 255
#define FILE_SEPARATOR  '/'
#define ROOT_FOLDER  "filesystem"
#define FILE_NOT_FOUND "404"

void cleanup(int); //does cleanup after catching signals
#ifdef	__cplusplus
}
#endif

#endif	/* HEADER_H */

