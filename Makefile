##
## EPITECH PROJECT, 2018
## PSU_myirc_2017
## File description:
## Makefile
##

NAME_SERVER	= server
NAME_CLIENT	= client

CC	= gcc

RM	= rm -f

SRCS_SHARED	= ./srcs/shared/basics.c	\
		  ./srcs/shared/socket.c

SRCS_CLIENT	= ./srcs/client/client.c

SRCS_SERVER	= ./srcs/server/server.c	\
		  ./srcs/server/commands.c	\
		  ./srcs/server/fd.c		\
		  ./srcs/server/manage_lists.c

OBJS_SHARED	= $(SRCS_SHARED:.c=.o)
OBJS_CLIENT	= $(SRCS_CLIENT:.c=.o)
OBJS_SERVER	= $(SRCS_SERVER:.c=.o)

CFLAGS = -I ./includes/
CFLAGS += -W -Wall -Wextra

all: $(NAME_CLIENT) $(NAME_SERVER)

$(NAME_CLIENT): $(OBJS_SHARED) $(OBJS_CLIENT)
	 $(CC) $(OBJS_SHARED) $(OBJS_CLIENT) -o $(NAME_CLIENT) $(LDFLAGS)

$(NAME_SERVER): $(OBJS_SHARED) $(OBJS_SERVER)
	 $(CC) $(OBJS_SHARED) $(OBJS_SERVER) -o $(NAME_SERVER) $(LDFLAGS)

clean:
	$(RM) $(OBJS_CLIENT)
	$(RM) $(OBJS_SERVER)

fclean: clean
	$(RM) $(NAME_CLIENT)
	$(RM) $(NAME_SERVER)

re: fclean all

.PHONY: all clean fclean re
