/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
*/

#include "client.h"
#include "macro.h"

static t_ptr_fct *get_cmd_ptr(void)
{
	static t_ptr_fct commands[] = {
		{"/server ", connect_to_server}
	};

	return (commands);
}

char *transform_cmd(char *cmd)
{
	int end_cmd = FAILURE;

	for (int i = 0 ; cmd[i] ; i++) {
		if (cmd[i] == ' ')
			end_cmd = SUCCESS;
		cmd[i] = (end_cmd == SUCCESS ? cmd[i + 1] :
			toupper(cmd[i + 1]));
	}
	return (cmd);
}

void exec_cmd(t_env *e, char *cmd)
{
	t_ptr_fct *commands = get_cmd_ptr();
	t_ptr_fct tmp;

	for (int i = 0 ; i < 1 ; i++) {
		tmp = commands[i];
		if (strncmp(cmd, tmp.name, strlen(tmp.name)) == SUCCESS) {
			tmp.cmd(e, cmd);
			return;
		}
	}
	dprintf(e->fd, "%s\r\n", transform_cmd(cmd));
}
