/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
*/

#include "server.h"
#include "macro.h"

void join(t_env *e, int fd, char *cmd)
{
	if (!e->users[fd].name)
		return;

	//assossier channel name avec channel id
	// verifier si le chan existe deja (si non user OP)
	printf("%s join channel %s\n", e->users[fd].name, &cmd[5]);
	e->users[fd].channel_id = 1;
	e->users[fd].channel_name = strdup(&cmd[5]);
}

void nick(t_env *e, int fd, char *cmd)
{
	// Verifier que c'est pas deja pris
	e->users[fd].name = strdup(&cmd[5]);

}

void user(t_env *e, int fd, char *cmd)
{
	dprintf(fd, "\n");
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

static void send_message(t_env *e, int fd, const char *msg)
{
	if (e->users[fd].channel_id == NONE) {
		dprintf(fd, "You must join a channel.\n");
		return;
	}
	for (int i = 0 ; i < MAX_FD ; i++) {
		if (e->users[fd].channel_id == e->users[i].channel_id &&
				i != fd && e->fd_type[i] == FD_CLIENT) {
			dprintf(i, "%s\n", msg);
			printf("sending message\n");
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
