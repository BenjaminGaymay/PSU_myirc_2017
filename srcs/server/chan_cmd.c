/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** chan_cmd
*/

#include "server.h"
#include "macro.h"

/*
** Afficher les channels du serveur
*/
int list_chan(t_env *e, t_client *client, const char *cmd)
{
	t_channel *tmp = e->channels;

	dprintf(client->fd, " > Available channels:\r\n");
	while (tmp) {
		if (strstr(tmp->name, &cmd[5]))
			dprintf(client->fd, "\t- %s\r\n", tmp->name);
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int leave_chan(t_env *e, t_client *client, const char *cmd)
{
	char *msg;
	t_channel *chan = client->channel;

	if (strlen(&cmd[5]) == 0)
		return (dprintf(client->fd, NOPARAM), FAILURE);
	if (strcmp(chan->name, &cmd[5]) != 0)
		return (dprintf(client->fd, NOTONCHAN, &cmd[5]),
			FAILURE);
	client->channel->nb_users -= 1;
	if (client->channel->nb_users == 0)
		delete_channel(e, client->channel);
	asprintf(&msg, ":%s PART %s", client->name, &cmd[5]);
	server_message(e, chan->id, msg);
	client->channel = NULL;
	client->channel_id = NONE;
	free(msg);
	return (SUCCESS);
}

int users_in_chan(t_env *e, t_client *client, const char *cmd)
{
	t_channel *channel = find_channel(e, &cmd[6]);

	if (!channel)
		return (dprintf(client->fd,
		" > Channel '%s' does not exist\r\n", &cmd[6]), FAILURE);
	dprintf(client->fd, " > Users connected on '%s' channel:\r\n",
		&cmd[6]);
	for (t_client *tmp = e->clients; tmp; tmp = tmp->next)
		if (tmp->channel_id == channel->id)
			dprintf(client->fd, "\t- %s\r\n", tmp->name);
	return (SUCCESS);
}
