/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** commands
*/

#pragma once

int nick(t_env *, t_client *, char *);
int user(t_env *, t_client *, char *);
int join(t_env *, t_client *, char *);
int show_users(t_env *, t_client *, char *);
int list_chan(t_env *, t_client *, char *);
int leave_chan(t_env *, t_client *, char *);
int users_in_chan(t_env *, t_client *, char *);
int private_msg(t_env *, t_client *, char *);
int quit(t_env *, t_client *, char *);