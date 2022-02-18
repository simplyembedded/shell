/**
 * \file shell.h
 * \brief Minimal POSIX shell implementation
 *
 * Copyright (c) 2022 Simply Embedded Inc. 
 * All Rights Reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

#include "shell.h"
#include "shell_config.h"

#include <stdio.h>
#include <string.h>

#define PROMPT_PREFIX          "\n$ "
#define is_space(x)             ((x)==' ' || (x)=='\t')

struct _input {
    char buffer[SHELL_CONFIG_INPUT_BUFFER_MAX];
    size_t len;
};

static int _parse(char *buffer, size_t len);

void shell(void)
{
    static struct _input history[SHELL_CONFIG_HISTORY_MAX];
    size_t input_idx = 0;
    int seq = 0;
    int dir = 0;
    
    memset(history, 0, sizeof(history));

    PROMPT();

 * This software is licensed under terms that can be found in the 
 * LICENSE file in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#ifndef _SHELL_H_ 
#define _SHELL_H_

#include <stddef.h>

struct shell_cmd
{
    const char *name;
    const char *usage;
    const char *desc;
    int (*funcptr)(int argc, const char *argv[]);
};

extern const struct shell_cmd shell_cmd_list[];
extern const size_t shell_cmd_list_len;

void shell(void);

#endif /* _SHELL_H_ */
