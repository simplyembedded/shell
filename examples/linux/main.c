/**
 * \file main.c
 * \author Chris Karaplis
 * \brief 
 *
 * Copyright (c) 2022 Simply Embedded Inc. All Rights Reserved.
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
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


