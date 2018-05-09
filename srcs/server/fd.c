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
void client_read(t_env *e, int fd)
{
	int size;
	char buffer[4096];
	char *tmp = NULL;

	size = read(fd, buffer, 4096);
	if (size > 0) {
		buffer[size] = '\0';
		tmp = strtok(buffer, "\n");
		while (tmp) {
			exec_client_command(e, fd, tmp);
			tmp = strtok(NULL, "\n");
		}
	} else {
		//faire commande quit (avec message toussa toussa)
		//verifier si dernier user du chan -> si oui on reset tout)
		close(fd);
		free(e->users[fd].name);
		e->channels[e->users[fd].channel_id].nb_users -= 1;
		e->users[fd].name = 0;
		e->fd_type[fd] = FD_FREE;
	}
}

/*
** un client a join donc on l'ajoute à la liste
*/
void server_read(t_env *e, int fd)
{
	add_client(e, fd);
}

static int get_fd_max(t_env *e)
{
	int fd_max = 0;

	for (int i = 0; i < MAX_FD; i++)
		if (e->fd_type[i] != FD_FREE) {
			FD_SET(i, e->fd_read);
			fd_max = i;
		}
	return (fd_max);
}

/*
** On regarde si qqch a été écrit sur un fd
** On execute la fonction liée au fd (serveur ou client)
*/
void read_fd(t_env *e)
{
	static struct timeval tv = {20, 0};
	int fd_max = get_fd_max(e);

	if (select(fd_max + 1, e->fd_read, NULL, NULL, &tv) == -1)
		FCT_FAIL("select");
	for (int i = 0; i < MAX_FD; i++)
		if (FD_ISSET(i, e->fd_read))
			e->fct_read[i](e, i);
}