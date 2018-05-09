/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#include "server.h"
#include "macro.h"

static int server_loop(t_env *e)
{
	while (1) {
		FD_ZERO(e->fd_read);
		read_fd(e);
	}
	return (SUCCESS);
}

static void init_server(t_env *e, int port)
{
	static fd_set fd_read;

	memset(e->fd_type, FD_FREE, MAX_FD);
	memset(e->users, 0, sizeof(e->users));
	memset(e->channels, 0, sizeof(e->channels));
	e->port = port;
	e->fd_read = &fd_read;
	add_server(e);
}

int main(const int argc, const char *argv[])
{
	t_env e;

	if (argc != 2)
		return (ERROR);
	init_server(&e, atoi(argv[1]));
	server_loop(&e);
	return (0);
}
