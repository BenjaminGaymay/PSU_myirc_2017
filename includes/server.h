/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#pragma once

#include "basics.h"
#include "socket.h"

typedef struct s_channel {
	int id;
	char *name;
	char *chanop;
	int nb_users;
	struct s_channel *next;
} t_channel;

typedef struct s_client {
	int id;
	char *name;
	int fd;
	t_channel *channel;
	int channel_id;
	struct s_client *next;
} t_client;

typedef struct s_env
{
	t_client *clients;
	t_channel *channels;
	fd_set *fd_read;
	int server;
	int port;
} t_env;

void add_server(t_env *);
int add_client(t_env *);

int read_all_fd(t_env *);
void server_read(t_env *, int);
void client_read(t_env *, int);

void server_message(t_env *, int, const char *);
int exec_client_command(t_env *, t_client *, char *);

int delete_client(t_env *, t_client *);
void delete_channel(t_env *, t_channel *);