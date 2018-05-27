/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands_manager
*/

#include "server.h"
#include "macro.h"
#include "commands.h"

static t_ptr_fct *get_cmd_ptr(void)
{
	static t_ptr_fct commands[] = {
		{"NICK", nick},
		{"USER ", user},
		{"JOIN", join},
		{"USERS", show_users},
		{"LIST", list_chan},
		{"PART", leave_chan},
		{"NAMES ", users_in_chan},
		{"PRIVMSG ", private_msg},
		{"QUIT", quit}
	};

	return (commands);
}

int exec_client_command(t_env *e, t_client *client, char *cmd)
{
	t_ptr_fct *commands = get_cmd_ptr();
	t_ptr_fct tmp;

	if (cmd[strlen(cmd) - 1] == '\r')
		cmd[strlen(cmd) - 1] = '\0';
	printf("CMD: %s\n", cmd);
	for (int i = 0 ; i < 9 ; i++) {
		tmp = commands[i];
		if (strncmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS)
			return (tmp.cmd(e, client, cmd));
	}
	return (SUCCESS);
}
