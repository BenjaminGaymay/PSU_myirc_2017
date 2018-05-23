/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** server
*/

#include <signal.h>
#include "server.h"
#include "macro.h"

void set_fd(t_env *e)
{
	t_client *tmp = e->clients;

	FD_ZERO(e->fd_read);
	FD_SET(e->server, e->fd_read);
	while (tmp) {
		FD_SET(tmp->fd, e->fd_read);
		tmp = tmp->next;
	}
}

int continue_loop(int new, int save)
{
	static int old = SUCCESS;

	if (save == 1)
		old = new;
	return (old);
}

void handle_sigint(int sign)
{
	(void)sign;
	continue_loop(FAILURE, 1);
}

void init_sigint_catch(void)
{
	struct sigaction action;

	action.sa_handler = handle_sigint;
	sigaction(SIGINT, &action, NULL);
}