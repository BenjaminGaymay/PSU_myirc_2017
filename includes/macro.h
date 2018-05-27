/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** macro
*/

#pragma once

#define SUCCESS 0
#define FAILURE 1
#define ERROR 84
#define EXIT -1
#define FD_ERROR -1

#define QUIET 0
#define VERBOSE 1

#define NONE -1
#define DELETE 2

#define FCT_FAIL(name) fprintf(stderr, "Error: %s failed\n", name)

#define CONNECT_CLOSE_TIMEOUT "Connection closed (server timed out)\r\n"
#define CONNECTION_FAILED "Connection failed\r\n"
#define CONNECTED_TO "Connected to %s\r\n"
#define USER_MSG ":127.0.0.1 001 %s :Welcome to the IRC Network %s\r\n"
#define NICKUSE ":127.0.0.1 433 ERR_NICKNAMEINUSE :%s is already in use\r\n"
#define NONICK ":127.0.0.1 433 ERR_NONICKNAME :No nickname given\r\n"
#define NOPARAM ":127.0.0.1 461 ERR_NEEDMOREPARAMS PART"/*
*/" :Not enough parameters\r\n"
#define NOTONCHAN ":127.0.0.1 442 ERR_NOTONCHANNEL %s :"/*
*/"You're not on that channel\r\n"