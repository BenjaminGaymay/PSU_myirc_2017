/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** client
*/

#pragma once

#include "basics.h"
#include "socket.h"

typedef struct s_env
{
	fd_set *fd_read;
	char *name;
	int port;
	int fd;
} t_env;