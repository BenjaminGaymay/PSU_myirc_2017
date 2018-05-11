/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** manage_lists
*/

#include "server.h"
#include "macro.h"

void delete_channel(t_env *e, t_channel *channel)
{
	t_channel *tmp = e->channels;

	if (!tmp || !channel)
		return;
	if (channel->id == tmp->id)
		e->channels = channel->next;
	else {
		while (tmp->next && tmp->next->id != channel->id)
			tmp = tmp->next;
		tmp->next = channel->next;
	}
	free(channel->name);
	free(channel->chanop);
	free(channel);
}

int delete_client(t_env *e, t_client *client)
{
	t_client *tmp = e->clients;

	if (!tmp || !client)
		return (FAILURE);
	close(client->fd);
	free(client->name);
	if (client->id == tmp->id)
		e->clients = client->next;
	else {
		while (tmp->next && tmp->next->id != client->id)
			tmp = tmp->next;
		tmp->next = client->next;
	}
	if (client->channel_id != NONE && client->channel) {
		client->channel->nb_users -= 1;
		if (client->channel->nb_users == 0)
			delete_channel(e, client->channel);
	}
	free(client);
	return (DELETE);
}