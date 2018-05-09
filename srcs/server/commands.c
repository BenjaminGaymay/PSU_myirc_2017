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
int find_channel(t_env *e, const char *chan_name)
{
	for (int i = 1 ; i < MAX_FD + 1 ; i++)
		if (e->channels[i].name &&
		strcmp(chan_name, e->channels[i].name) == 0)
			return (i);
	return (FD_ERROR);
}

int create_channel(t_env *e, int fd, const char *chan_name)
{
	for (int i = 1 ; i < MAX_FD + 1; i++)
		if (e->channels[i].nb_users < 1) {
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
	e->channels[e->users[fd].channel_id].nb_users -= 1;
	channel_id = (find_channel(e, &cmd[5]) != FD_ERROR ?
		find_channel(e, &cmd[5]) : create_channel(e, fd, &cmd[5]));
	if (channel_id == FD_ERROR)
		return;
	if (e->channels[channel_id].chanop == fd)
		asprintf(&msg, "* @%s has joined", e->users[fd].name);
	else
		asprintf(&msg, "* %s has joined", e->users[fd].name);
	server_message(e, channel_id, msg);
	free(msg);
	e->channels[channel_id].nb_users += 1;
	e->users[fd].channel_id = channel_id;
}

void nick(t_env *e, int fd, char *cmd)
{
	// Verifier que c'est pas deja pris
	e->users[fd].name = strdup(&cmd[5]);

}

void user()
{
}

void show_users(t_env *e, int fd)
{
	dprintf(fd, " > Connected users:\n");
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (e->users[i].name && e->fd_type[i] == FD_CLIENT)
			dprintf(fd, "\t- %s\n", e->users[i].name);
	}
}

void list_chan(t_env *e, int fd, const char *cmd)
{
	dprintf(fd, " > Available channels:\n");
	for (int i = 1 ; i < MAX_FD ; i++) {
		if (e->channels[i].nb_users > 0 && strstr(e->channels[i].name, &cmd[5]))
			dprintf(fd, "\t- %s\n", e->channels[i].name);
	}
}

void leave_chan(t_env *e, int fd, const char *cmd)
{
	char *msg;
	int channel_id = e->users[fd].channel_id;

	// verifier si c'est le dernier user du chan
	// si oui rm le name toussa toussa
	if (strcmp(e->channels[channel_id].name, &cmd[5]) != 0)
		return;
	e->channels[channel_id].nb_users -= 1;
	e->users[fd].channel_id = NONE;
	if (e->channels[channel_id].chanop == fd)
		asprintf(&msg, "* @%s has disconnected", e->users[fd].name);
	else
		asprintf(&msg, "* %s has disconnected", e->users[fd].name);
	server_message(e, channel_id, msg);
	free(msg);
}

void users_in_chan(t_env *e, int fd, const char *cmd)
{
	int channel_id = find_channel(e, &cmd[6]);

	if (channel_id == FD_ERROR) {
		dprintf(fd, " > Channel '%s' does not exist\n", &cmd[6]);
		return;
	}
	dprintf(fd, " > Users connected on '%s' channel:\n", &cmd[6]);
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (e->users[i].name && e->fd_type[i] == FD_CLIENT &&
		e->users[i].channel_id == channel_id)
			dprintf(fd, "\t- %s\n", e->users[i].name);
	}
}

int get_user_id_by_name(t_env *e, const char *name)
{
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (e->users[i].name && e->fd_type[i] == FD_CLIENT &&
		strcmp(e->users[i].name, name) == 0)
			return (i);
	}
	return (FD_ERROR);
}

void private_msg(t_env *e, int fd, const char *cmd)
{
	int i = 8;
	char *name;
	char *msg;
	int dest_fd;

	while (cmd[i] && cmd[i] != ' ')
		i += 1;
	name = strndup(&cmd[8], i - 8);
	dest_fd = get_user_id_by_name(e, name);
	if (dest_fd == FD_ERROR)
		dprintf(fd, " > User '%s' does not exist\n", name);
	else {
		asprintf(&msg, " -> from %s: %s\n", e->users[fd].name, &cmd[i + 1]);
		dprintf(dest_fd, msg);
		free(msg);
	}
	free(name);
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

	// Faut mettre ton truc pour strcmp
	// la si tu fais un /listqsdqsd au lieu de /list ca marche =)))
	for (int i = 0 ; i < 8 ; i++) {
		tmp = commands[i];
		if (strncmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS) {
			tmp.cmd(e, fd, cmd);
			return;
		}
	}
	send_message(e, fd, cmd);
}
