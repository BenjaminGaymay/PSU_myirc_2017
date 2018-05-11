/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
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
	return (new);
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
	channel = (find_channel(e, &cmd[5]) ? find_channel(e, &cmd[5]) : create_channel(e, client, &cmd[5]));
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
int  nick(t_env *e, t_client *client, char *cmd)
{
	t_client *tmp = e->clients;
	char *msg;

	while (tmp) {
		if (strcmp(tmp->name, &cmd[5]) == 0 && tmp->fd != client->fd)
			return (dprintf(client->fd, "Nickname '%s' already used\n", &cmd[5]), FAILURE);
		tmp = tmp->next;
	}
	if (client->channel) {
		if (is_chanop(client->channel, client) == SUCCESS)
			asprintf(&msg, "@%s is now known as @%s", client->name, &cmd[5]);
		else
			asprintf(&msg, "%s is now known as %s", client->name, &cmd[5]);
		server_message(e, client->channel->id, msg);
		free(msg);
	}
	if  (client->channel && strcmp(client->channel->chanop, client->name) == 0) {
		free(client->channel->chanop);
		client->channel->chanop = strdup(&cmd[5]);
	}
	free(client->name);
	client->name = strdup(&cmd[5]);
	return (SUCCESS);
}

int user(void)
{
	return (SUCCESS);
}

/*
** Afficher les users du serveur
*/
int show_users(t_env *e, t_client *client)
{
	t_client *tmp = e->clients;

	dprintf(client->fd, " > Connected users:\n");
	while (tmp) {
		dprintf(client->fd, "\t- %s\n", tmp->name);
		tmp = tmp->next;
	}
	return (SUCCESS);
}

/*
** Afficher les channels du serveur
*/
int list_chan(t_env *e, t_client *client, const char *cmd)
{
	t_channel *tmp = e->channels;

	dprintf(client->fd, " > Available channels:\n");
	while (tmp) {
		if (strstr(tmp->name, &cmd[5]))
			dprintf(client->fd, "\t- %s\n", tmp->name);
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int leave_chan(t_env *e, t_client *client, const char *cmd)
{
	char *msg;
	t_channel *chan = client->channel;

	if (strcmp(chan->name, &cmd[5]) != 0)
		return (dprintf(client->fd, "You are not connected on this channel\n"), FAILURE);
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
	return (SUCCESS);
}

int users_in_chan(t_env *e, t_client *client, const char *cmd)
{
	t_channel *channel = find_channel(e, &cmd[6]);
	t_client *tmp = e->clients;

	if (!channel)
		return (dprintf(client->fd, " > Channel '%s' does not exist\n", &cmd[6]), FAILURE);
	dprintf(client->fd, " > Users connected on '%s' channel:\n", &cmd[6]);
	while (tmp) {
		if (tmp->channel_id == channel->id)
			dprintf(client->fd, "\t- %s\n", tmp->name);
		tmp = tmp->next;
	}
	return (SUCCESS);
}

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

int private_msg(t_env *e, t_client *client, const char *cmd)
{
	int i = 8;
	char *name;
	char *msg;
	t_client *dest;

	while (cmd[i] && cmd[i] != ' ')
		i += 1;
	name = strndup(&cmd[8], i - 8);
	dest = get_user_by_name(e, name);
	if (!dest)
		dprintf(client->fd, " > User '%s' does not exist\n", name);
	else {
		asprintf(&msg, " -> from %s: %s\n", client->name, &cmd[i + 1]);
		dprintf(dest->fd, msg);
		free(msg);
	}
	free(name);
	return (SUCCESS);
}

static t_ptr_fct *get_cmd_ptr(void)
{
	static t_ptr_fct commands[] = {
		{"NICK ", nick},
		{"USER ", user},
		{"JOIN ", join},
		{"USERS", show_users},
		{"LIST", list_chan},
		{"PART ", leave_chan},
		{"NAMES ", users_in_chan},
		{"PRIVMSG ", private_msg}
	};

	return (commands);
}

void server_message(t_env *e, int channel_id, const char *msg)
{
	t_client *tmp = e->clients;

	if (channel_id == NONE)
		return;
	while (tmp) {
		if (tmp->channel_id == channel_id)
			dprintf(tmp->fd, "%s\n", msg);
		tmp = tmp->next;
	}
}

static int send_message(t_env *e, t_client *client, const char *msg)
{
	t_client *tmp = e->clients;

	if (!client->channel)
		return (dprintf(client->fd, "You must join a channel.\n"), FAILURE);
	while (tmp) {
		if (tmp->channel_id == client->channel_id &&
				tmp->fd != client->fd) {
			if (strcmp(client->channel->chanop, client->name) == 0)
				dprintf(tmp->fd, "@%s: %s\n", client->name, msg);
			else
				dprintf(tmp->fd, "%s: %s\n", client->name, msg);
		}
		tmp = tmp->next;
	}
	return (SUCCESS);
}

int exec_client_command(t_env *e, t_client *client, char *cmd)
{
	t_ptr_fct *commands = get_cmd_ptr();
	t_ptr_fct tmp;

	// Faut mettre ton truc pour strcmp
	// la si tu fais un /listqsdqsd au lieu de /list ca marche =)))
	for (int i = 0 ; i < 8 ; i++) {
		tmp = commands[i];
		if (strncmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS)
		//faire commande quit (avec message toussa toussa)
			return (tmp.cmd(e, client, cmd));
	}
	send_message(e, client, cmd);
	return (SUCCESS);
}
