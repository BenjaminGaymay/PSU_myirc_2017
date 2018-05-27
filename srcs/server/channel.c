/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** channel
*/

#include "server.h"
#include "macro.h"

/*
** Recherche de l'ID du channel
** Création du channel s'il n'existe pas
*/
t_channel *find_channel(t_env *e, const char *chan_name)
{
	t_channel *tmp = e->channels;

	while (tmp) {
		if (strcmp(tmp->name, chan_name) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_channel *create_channel(t_env *e, t_client *client, const char *chan_name)
{
	static int id = 0;
	t_channel *new = calloc(1, sizeof(*new));

	if (!new)
		return (FCT_FAIL("malloc"), NULL);
	new->id = id++;
	new->name = strdup(chan_name);
	new->chanop = strdup(client->name);
	new->nb_users = 0;
	new->next = e->channels;
	e->channels = new;
	dprintf(client->fd, ":127.0.0.1 331 RPL_NOTOPIC %s :\r\n", chan_name);
	return (new);
}

void print_chanop(t_client *tmp, t_client *client, const char *msg)
{
	if (strcmp(client->channel->chanop, client->name) == 0)
		dprintf(tmp->fd, "@%s: %s\r\n",	client->name, msg);
	else
		dprintf(tmp->fd, "%s: %s\r\n", client->name, msg);
}