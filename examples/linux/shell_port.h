/**
 * \file shell_port.h
 * \author Chris Karaplis
 * \brief 
 *
 * Copyright (c) 2022 Simply Embedded Inc.
 * All rights reserved.
 */

#ifndef  _SHELL_PORT_H_
#define  _SHELL_PORT_H_

#include <stdio.h>

#define SHELL_CONFIG_INPUT_BUFFER_MAX       (10U)
#define SHELL_CONFIG_INPUT_ARGS_MAX         (10U)
#define SHELL_CONFIG_HISTORY_MAX            (10U)
#define SHELL_PUTCHAR(c)                    putchar(c)
#define SHELL_PUTS(s)                       fputs(s, stdout)
#define SHELL_GETC()                        getc(stdin)

#endif /* _SHELL_PORT_H_ */


