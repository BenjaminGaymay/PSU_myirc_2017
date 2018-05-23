/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** main
*/

#include "client.h"
#include "macro.h"

static void usage(const char *prog)
{
	printf("USAGE\n\t%s\n", prog);
}

int main(int ac, char **av)
{
	(void)ac;
	if (strcmp(av[1], "-h") == 0 || strcmp(av[1], "--help") == 0)
		return (usage(av[0]), SUCCESS);
	client_loop();
	return (SUCCESS);
}