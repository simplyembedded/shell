/**
 * \file main.c
 * \brief A sample shell application in Linux.
 *
 * Copyright (c) 2022 Simply Embedded Inc. 
 * All Rights Reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

#include "shell.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

static void _enable_raw_mode(void); 
static void _disable_raw_mode(void);
static int _help(int argc, char *argv[]);

const struct shell_cmd shell_cmd_list[] = {
    {
        .name = "help", 
        .usage = NULL, 
        .desc = "A help menu", 
        .funcptr = _help
    },
};
const size_t shell_cmd_list_len = ARRAY_SIZE(shell_cmd_list);

static struct termios orig_termios;

int main(int argc, const char *argv[])
{
    _enable_raw_mode();
    shell();
}

static void _disable_raw_mode(void) 
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    putchar('\n');
}

static void _enable_raw_mode(void) 
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(_disable_raw_mode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static int _help(int argc, char *argv[])
{
    size_t i;

    (void)argc;
    (void)argv;

    puts("\nConsole Help - available commands:\n");
    
    for (i = 0; i < shell_cmd_list_len; i++) {
        printf("%s: %s\n\t%s", shell_cmd_list[i].name, shell_cmd_list[i].usage, shell_cmd_list[i].desc);
    }

    return 0;
}


