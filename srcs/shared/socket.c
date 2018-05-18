/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** socket
*/

#include "socket.h"
#include "macro.h"

int safe_close(const int fd, const int ret)
{
	if (close(fd) == -1)
		return (fprintf(stderr, "Error: close failed\n"), ERROR);
	return (ret);
}

/*
** Socket serveur
*/
int s_socket(int fd, struct sockaddr_in *s_in, int quiet)
{
	if (bind(fd, (const struct sockaddr *)s_in, sizeof(*s_in)) == -1) {
		if (quiet == VERBOSE)
			FCT_FAIL("bind");
		return (safe_close(fd, FD_ERROR));
	}
	if (listen(fd, 42) == -1)
		return (FCT_FAIL("listen"), safe_close(fd, FD_ERROR));
	return (fd);
}

/*
** Socket client
*/
int c_socket(int fd, struct sockaddr_in *s_in)
{
	if (connect(fd, (struct sockaddr *)s_in, sizeof(*s_in)) == -1)
		return (FCT_FAIL("connect"), safe_close(fd, FD_ERROR));
	return (fd);
}

/*
** Base commune de création des sockets
*/
int create_socket(const int port, const in_addr_t addr,
			e_socket_type s_t, int quiet)
{
	int fd;
	struct protoent *pe;
	struct sockaddr_in s_in;

	pe = getprotobyname("TCP");
	if (! pe)
		return (FCT_FAIL("getprotobyname"), FD_ERROR);
	fd = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
		sizeof(int)) < 0)
		return (FCT_FAIL("setsockopt"), FD_ERROR);
	if (fd == -1)
		return (FCT_FAIL("socket"), FD_ERROR);
	s_in.sin_family = AF_INET;
	s_in.sin_port = htons(port);
	s_in.sin_addr.s_addr = addr;
	return (s_t == SERVER ? s_socket(fd, &s_in, quiet)
				: c_socket(fd, &s_in));
}