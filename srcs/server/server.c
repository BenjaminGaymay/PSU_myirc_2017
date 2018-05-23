/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#include <signal.h>
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

static int continue_loop(int new, int save)
{
	static int old = SUCCESS;

	if (save == 1)
		old = new;
	return (old);
}

static void handle_sigint(int sign)
{
	(void)sign;
	continue_loop(FAILURE, 1);
}

static void init_sigint_catch(void)
{
	struct sigaction action;

	action.sa_handler = handle_sigint;
	sigaction(SIGINT, &action, NULL);
}

static int server_loop(t_env *e)
{
	while (continue_loop(0, 0) == SUCCESS) {
		set_fd(e);
		if (read_all_fd(e) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

static int init_server(t_env *e, const char *argv[])
{
	init_sigint_catch();
	e->port = atoi(argv[1]);
	e->server = create_socket(e->port, INADDR_ANY, SERVER, VERBOSE);
	if (e->server == FD_ERROR)
		return (FD_ERROR);
	return (SUCCESS);
}

int main(const int argc, const char *argv[])
{
	fd_set fd_read;
	t_env e = {NULL, NULL, &fd_read, -1, -1};

	if (argc != 2)
		return (ERROR);
	if (init_server(&e, argv) == FD_ERROR)
		return (ERROR);
	server_loop(&e);
	while (e.clients)
		delete_client(&e, e.clients);
	return (0);
}
