/*
** EPITECH PROJECT, 2018
**PSU_myirc_2017
** File description:
** client
*/

#include "client.h"
#include "macro.h"

int connect_to_server(t_env *e, const char *ip)
{
	e->fd = create_socket(e->port, inet_addr(&ip[8]), CLIENT, VERBOSE);
	if (e->fd != FD_ERROR)
		printf("Connected to %s\n", &ip[8]);
	else
		printf("Connection failed\n");
	return (SUCCESS);
}

static t_ptr_fct *get_cmd_ptr(void)
{
	static t_ptr_fct commands[] = {
		{"/server ", connect_to_server}
	};

	return (commands);
}

void exec_cmd(t_env *e, char *cmd)
{
	t_ptr_fct *commands = get_cmd_ptr();
	t_ptr_fct tmp;

	// Faut mettre ton truc pour strcmp
	// la si tu fais un /listqsdqsd au lieu de /list ca marche =)))
	for (int i = 0 ; i < 1 ; i++) {
		tmp = commands[i];
		if (strncmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS) {
			tmp.cmd(e, cmd);
			return;
		}
	}
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
			dprintf(e->fd, "%s\n", buffer);
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
		tmp = strtok(buffer, "\n");
		while (tmp) {
			printf("%s\n", tmp);
			tmp = strtok(NULL, "\n");
		}
	} else {
		close(e->fd);
		e->fd = FD_ERROR;
		printf("Connection closed (server timed out)\n");
	}
}

void read_fd(t_env *e)
{
	static struct timeval tv;
	int fd_max = (e->fd != FD_ERROR ? e->fd : 0);

	tv.tv_sec = 1;
	tv.tv_usec = 0;
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

int main(void)
{
	client_loop();
	return (0);
}