/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** add_fd
*/

#include "server.h"
#include "macro.h"

/*
** Création d'un client
*/
void add_client(t_env *e, int s)
{
	int client;
	struct sockaddr_in client_sin;
	socklen_t client_sin_len;

	client_sin_len = sizeof(client_sin);
	client = accept(s, (struct sockaddr *)&client_sin, &client_sin_len);
	e->fd_type[client] = FD_CLIENT;
	e->fct_read[client] = client_read;
	e->fct_write[client] = NULL;
}

/*
** Création du serveur
*/
void add_server(t_env *e)
{
	int serv;

	serv = create_socket(e->port, INADDR_ANY, SERVER, VERBOSE);
	e->fd_type[serv] = FD_SERVER;
	e->fct_read[serv] = server_read;
	e->fct_write[serv] = NULL;
}
