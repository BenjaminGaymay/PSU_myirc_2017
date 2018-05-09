/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#pragma once

#include "basics.h"
#include "socket.h"

#define FD_FREE 0
#define FD_CLIENT 1
#define FD_SERVER 2
#define MAX_FD 255

typedef void(*fct)();

typedef struct s_client {
	char *name;
	char *channel_name;
	int channel_id;
} t_client;

typedef struct s_ptr_fct {
	char *name;
	fct cmd;
} t_ptr_fct;

typedef struct s_env
{
	char fd_type[MAX_FD];
	fct fct_read[MAX_FD];
	fct fct_write[MAX_FD];
	t_client users[MAX_FD];
	fd_set *fd_read;
	int port;
} t_env;

void add_server(t_env *);
void add_client(t_env *, int);

void read_fd(t_env *);
void server_read(t_env *, int);
void client_read(t_env *, int);

void exec_client_command(t_env *, int, char *);