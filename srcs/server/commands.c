/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
*/

#include "server.h"
#include "macro.h"

t_client *get_user_by_name(t_env *e, const char *name)
{
	t_client *tmp = e->clients;

	while (tmp) {
		if (strcmp(tmp->name, name) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int private_msg(t_env *e, t_client *c, const char *cmd)
{
	int i = 8;
	char *name;
	char *msg;
	t_client *cli_dest;
	t_channel *chan_dest;

	while (cmd[i] && cmd[i] != ' ')
		i += 1;
	name = strndup(&cmd[8], i - 8);
	chan_dest = find_channel(e, name);
	asprintf(&msg, ":%s PRIVMSG %s %s", c->name, name, &cmd[i + 1]);
	printf("privmsg: '%s'\r\n", msg);
	if (!chan_dest) {
		cli_dest = get_user_by_name(e, name);
		if (cli_dest)
			dprintf(cli_dest->fd, "%s\r\n", msg);
	} else
		server_message(e, chan_dest->id, msg);
	return (free(msg), free(name), SUCCESS);
}

int quit(t_env *e, t_client *client)
{
	char *msg;
	t_channel *chan = client->channel;

	if (chan) {
		client->channel->nb_users -= 1;
		if (client->channel->nb_users == 0)
				delete_channel(e, client->channel);
		if (is_chanop(chan, client) == SUCCESS)
			asprintf(&msg, "* @%s has disconnected", client->name);
		else
			asprintf(&msg, "* %s has disconnected", client->name);
		client->channel = NULL;
		client->channel_id = NONE;
		server_message(e, chan->id, msg);
		free(msg);
	}
	delete_client(e, client);
	return (SUCCESS);
}

void server_message(t_env *e, int channel_id, const char *msg)
{
	t_client *tmp = e->clients;

	if (channel_id == NONE)
		return;
	while (tmp) {
		if (tmp->channel_id == channel_id)
			dprintf(tmp->fd, "%s\r\n", msg);
		tmp = tmp->next;
	}
}

int send_message(t_env *e, t_client *client, const char *msg)
{
	t_client *tmp = e->clients;

	if (!client->channel)
		return (dprintf(client->fd, "You must join a channel.\n"),
			FAILURE);
	while (tmp) {
		if (tmp->channel_id == client->channel_id &&
				tmp->fd != client->fd)
			print_chanop(tmp, client, msg);
		tmp = tmp->next;
	}
	return (SUCCESS);
}
