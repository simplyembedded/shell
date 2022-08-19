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

// textual settings
#define SHELL_PROMPT            "$ "
#define SHELL_NEWLINE           "\n\r"
#define SHELL_BACKSPACE         "\b "    // backspace and clear character
#define SHELL_ECHO              0       // whether the shell should print back most ascii

#define SHELL_CONFIG_INPUT_BUFFER_MAX       (10U)
#define SHELL_CONFIG_INPUT_ARGS_MAX         (10U)
#define SHELL_CONFIG_HISTORY_MAX            (10U)

/**
 * @brief puts a char to hal output 
 * 
 * @param c char
 * @return int putchar()
 */
int shell_putchar(char c);

/**
 * @brief puts a string (! WITHOUT NEWLINE !) to hal output
 * 
 * @param text string
 * @return int puts()
 */
int shell_puts(const char *text);

/**
 * @brief gets a char from hal input
 * 
 * @return int getchar()
 */
int shell_getchar(void);

#endif /* _SHELL_PORT_H_ */


