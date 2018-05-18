/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** fd
*/

#include "server.h"
#include "macro.h"

/*
** lecture de la commande du client
*/
int read_on_client(t_env *e, t_client *client)
{
	static char buffer[4096];
	int size;
	char *tmp = NULL;

	size = read(client->fd, buffer, 4096);
	if (size > 0) {
		buffer[size] = '\0';
		tmp = strtok(buffer, "\n");
		while (tmp) {
			if (exec_client_command(e, client, tmp) == DELETE)
				return (DELETE);
			tmp = strtok(NULL, "\n");
		}
	}
	else
		return (delete_client(e, client));
	return (SUCCESS);
}

/*
** Création d'un client
*/
int add_client(t_env *e)
{
	static int id = 0;
	struct sockaddr_in client_sin;
	socklen_t client_sin_len;
	t_client *new = calloc(1, sizeof(*new));

	if (!new)
		return (FCT_FAIL("malloc"), ERROR);
	client_sin_len = sizeof(client_sin);
	asprintf(&(new->name), "anonymous%d", id);
	new->id = id++;
	new->fd = accept(e->server, (struct sockaddr *)&client_sin,
		&client_sin_len);
	new->channel_id = NONE;
	new->next = e->clients;
	new->channel = NULL;
	e->clients = new;
	return (SUCCESS);
}

int get_max_fd(t_env *e)
{
	int max = e->server;
	t_client *tmp = e->clients;

	while (tmp) {
		max = (tmp->fd > max ? tmp->fd : max);
		tmp = tmp->next;
	}
	return (max);
}

/*
** On regarde si qqch a été écrit sur un fd
** On execute la fonction liée au fd (serveur ou client)
*/
int read_all_fd(t_env *e)
{
	struct timeval tv = {1, 0};
	t_client *tmp;

	if (select(get_max_fd(e) + 1, e->fd_read, NULL, NULL, &tv) == -1)
		return (FCT_FAIL("select"), ERROR);
	if (FD_ISSET(e->server, e->fd_read) && add_client(e) == ERROR)
		return (ERROR);
	tmp = e->clients;
	while (tmp) {
		if (FD_ISSET(tmp->fd, e->fd_read))
			tmp = (read_on_client(e, tmp) == DELETE ?
				e->clients : tmp->next);
		else
			tmp = tmp->next;
	}
	return (SUCCESS);
}