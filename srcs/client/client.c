/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** client
*/

#include "client.h"
#include "macro.h"

int connect_to_server(t_env *e, const char *ip)
{
	e->fd = create_socket(e->port, inet_addr(&ip[8]), CLIENT, VERBOSE);
	if (e->fd != FD_ERROR)
		printf(CONNECTED_TO, &ip[8]);
	else
		printf(CONNECTION_FAILED);
	return (SUCCESS);
}

void read_client(t_env *e)
{
	int size;
	char buffer[4096];

	size = read(0, buffer, 4096);
	if (size > 0) {
		buffer[size - 1] = '\0';
		if (buffer[0] == '/')
			exec_cmd(e, buffer);
		else if (e->fd != FD_ERROR)
			dprintf(e->fd, "%s\r\n", buffer);
	}
}

void receive_message(t_env *e)
{
	int size;
	char buffer[4096];
	char *tmp = NULL;

	size = read(e->fd, buffer, 4096);
	if (size > 0) {
		buffer[size - 1] = '\0';
		tmp = strtok(buffer, "\r\n");
		while (tmp) {
			printf("%s\r\n", tmp);
			tmp = strtok(NULL, "\r\n");
		}
	} else {
		close(e->fd);
		e->fd = FD_ERROR;
		printf(CONNECT_CLOSE_TIMEOUT);
	}
}

void read_fd(t_env *e)
{
	struct timeval tv = {1, 0};
	int fd_max = (e->fd != FD_ERROR ? e->fd : 0);

	FD_SET(0, e->fd_read);
	if (e->fd != FD_ERROR)
		FD_SET(e->fd, e->fd_read);
	if (select(fd_max + 1, e->fd_read, NULL, NULL, &tv) == -1)
		FCT_FAIL("select");
	if (FD_ISSET(0, e->fd_read))
		read_client(e);
	if (e->fd != FD_ERROR && FD_ISSET(e->fd, e->fd_read))
		receive_message(e);
}

void client_loop(void)
{
	fd_set fd_read;
	t_env e = {&fd_read, NULL, 6697, FD_ERROR};
	int quit = FAILURE;

	while (quit == FAILURE) {
		FD_ZERO(&fd_read);
		read_fd(&e);
	}
}