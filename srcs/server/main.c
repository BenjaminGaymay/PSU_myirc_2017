/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** main
*/

#include "server.h"
#include "macro.h"

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

static void usage(const char *prog)
{
	printf("USAGE\n\t%s\tport\n\nDESCRIPTION"
		"\n\tport\tport of the server\n", prog);
}

int main(const int argc, const char *argv[])
{
	fd_set fd_read;
	t_env e = {NULL, NULL, &fd_read, -1, -1};

	if (argc != 2)
		return (usage(argv[0]), ERROR);
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		return (usage(argv[0]), SUCCESS);
	if (init_server(&e, argv) == FD_ERROR)
		return (ERROR);
	server_loop(&e);
	while (e.clients)
		delete_client(&e, e.clients);
	return (0);
}