/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
*/

#include "server.h"
#include "macro.h"


/*
** Recherche de l'ID du channel
** Création du channel s'il n'existe pas
*/
int find_channel(t_env *e, int fd, const char *chan_name)
{
	for (int i = 1 ; i < MAX_FD + 1 ; i++)
		if (e->channels[i].name &&
		strcmp(chan_name, e->channels[i].name) == 0)
			return (i);
	for (int i = 1 ; i < MAX_FD + 1; i++)
		if (!e->channels[i].name) {
			e->channels[i].chanop = fd;
			e->channels[i].name = strdup(chan_name);
			return (i);
		}
	return (FD_ERROR);
}

void join(t_env *e, int fd, char *cmd)
{
	int channel_id;
	char *msg;

	if (!e->users[fd].name)
		return;
	channel_id = find_channel(e, fd, &cmd[5]);
	if (channel_id == FD_ERROR)
		return;
	asprintf(&msg, "* %s has joined", e->users[fd].name);
	server_message(e, channel_id, msg);
	free(msg);
	e->users[fd].channel_id = channel_id;
	e->users[fd].channel_name = e->channels[channel_id].name;
}

void nick(t_env *e, int fd, char *cmd)
{
	// Verifier que c'est pas deja pris
	e->users[fd].name = strdup(&cmd[5]);

}

void user()
{
}

static t_ptr_fct *get_cmd_ptr(void)
{
	static t_ptr_fct commands[] = {
		{"NICK ", nick},
		{"USER ", user},
		{"/join ", join}
	};

	return (commands);
}

void server_message(t_env *e, int channel_id, const char *msg)
{
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (channel_id == e->users[i].channel_id &&
				e->fd_type[i] == FD_CLIENT) {
			dprintf(i, "%s\n", msg);
		}
	}
}

static void send_message(t_env *e, int fd, const char *msg)
{
	if (e->users[fd].channel_id == NONE) {
		dprintf(fd, "You must join a channel.\n");
		return;
	}
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (e->users[fd].channel_id == e->users[i].channel_id &&
				i != fd && e->fd_type[i] == FD_CLIENT) {
			if (e->channels[e->users[fd].channel_id].chanop == fd)
				dprintf(i, "@%s: %s\n", e->users[fd].name, msg);
			else
				dprintf(i, "%s: %s\n", e->users[fd].name, msg);
		}
	}
}

void exec_client_command(t_env *e, int fd, char *cmd)
{
	t_ptr_fct *commands = get_cmd_ptr();
	t_ptr_fct tmp;

	for (int i = 0 ; i < 3; i++) {
		tmp = commands[i];
		if (strncasecmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS) {
			tmp.cmd(e, fd, cmd);
			return;
		}
	}
	send_message(e, fd, cmd);
}
