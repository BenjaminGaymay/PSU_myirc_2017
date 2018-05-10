/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#include "server.h"
#include "macro.h"

static void set_fd(t_env *e)
{
	t_client *tmp = e->clients;

	FD_ZERO(e->fd_read);
	FD_SET(e->server, e->fd_read);
	while (tmp) {
		FD_SET(tmp->fd, e->fd_read);
		tmp = tmp->next;
	}
}


static int server_loop(t_env *e)
{
	while (1) {
		set_fd(e);
		if (read_all_fd(e) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

static int init_server(t_env *e, const char *argv[])
{
	// verifier argv1
	e->port = atoi(argv[1]);
	e->server = create_socket(e->port, INADDR_ANY, SERVER, VERBOSE);
	if (e->server == FD_ERROR)
		return (FD_ERROR);
	e->fd_max = e->server;
	return (SUCCESS);
}

int main(const int argc, const char *argv[])
{
	fd_set fd_read;
	t_env e = {NULL, NULL, &fd_read, 0, -1, -1};

	if (argc != 2)
		return (ERROR);
	if (init_server(&e, argv) == FD_ERROR)
		return (ERROR);
	server_loop(&e);
	//supprimer t_env e
	return (0);
}
