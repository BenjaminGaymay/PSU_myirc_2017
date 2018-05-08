/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** basics
*/

#pragma once

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char *replace_char(char *, const char, const char);
int read_file(const char *, const int);
int create_file(const char *, const int);
char *get_ip_from_cmd(char *);
int get_port_from_cmd(char *, int);