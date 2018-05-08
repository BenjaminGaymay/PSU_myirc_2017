/*
** EPITECH PROJECT, 2018
** PSU_myirc_2017
** File description:
** socket
*/

#pragma once

#include "basics.h"

typedef enum {
	CLIENT,
	SERVER
} e_socket_type;

int safe_close(const int, const int);
int create_socket(const int, const in_addr_t, e_socket_type s_t, int);