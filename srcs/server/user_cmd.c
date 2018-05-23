/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** user_cmd
*/

#include "server.h"
#include "macro.h"

int is_chanop(t_channel *chan, t_client *client)
{
	if (strcmp(chan->chanop, client->name) == 0)
		return (SUCCESS);
	return (FAILURE);
}

/*
** Rejoindre un channel
*/
int join(t_env *e, t_client *client, char *cmd)
{
	t_channel *channel;
	char *msg;

	if (client->channel) {
		client->channel->nb_users -= 1;
		if (client->channel->nb_users == 0)
			delete_channel(e, client->channel);
	}
	channel = (find_channel(e, &cmd[5]) ? find_channel(e, &cmd[5]) :
		create_channel(e, client, &cmd[5]));
	if (!channel)
		return (FAILURE);
	if (is_chanop(channel, client) == SUCCESS)
		asprintf(&msg, "* @%s has joined", client->name);
	else
		asprintf(&msg, "* %s has joined", client->name);
	server_message(e, channel->id, msg);
	free(msg);
	channel->nb_users += 1;
	client->channel = channel;
	client->channel_id = channel->id;
	return (SUCCESS);
}

/*
** Changer de pseudo
*/
int nick(t_env *e, t_client *c, char *cmd)
{
	t_client *tmp = e->clients;
	char *msg;

	while (tmp) {
		if (strcmp(tmp->name, &cmd[5]) == 0 && tmp->fd != c->fd)
			return (dprintf(c->fd,
			"Nickname '%s' already used\r\n", &cmd[5]), FAILURE);
		tmp = tmp->next;
	}
	if (c->channel) {
		if (is_chanop(c->channel, c) == SUCCESS)
			asprintf(&msg, "@%s is now known as @%s",
				c->name, &cmd[5]);
		else
			asprintf(&msg, "%s is now known as %s", c->name,
				&cmd[5]);
		server_message(e, c->channel->id, msg);
		free(msg);
	}
	if (c->channel && strcmp(c->channel->chanop, c->name) == 0) {
		free(c->channel->chanop);
		c->channel->chanop = strdup(&cmd[5]);
	}
	free(c->name);
	c->name = strdup(&cmd[5]);
	return (SUCCESS);
}

int user(t_env *e, t_client *client, const char *cmd)
{
	(void)e;
	(void)cmd;
	dprintf(client->fd, USER_MSG, client->name, client->name);
	return (SUCCESS);
}

/*
** Afficher les users du serveur
*/
int show_users(t_env *e, t_client *client)
{
	t_client *tmp = e->clients;

	dprintf(client->fd, " > Connected users:\r\n");
	while (tmp) {
		dprintf(client->fd, "\t- %s\r\n", tmp->name);
		tmp = tmp->next;
	}
	return (SUCCESS);
}