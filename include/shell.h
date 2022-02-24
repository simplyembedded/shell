/**
 * \file shell.h
 * \brief Minimal POSIX shell implementation
 *
 * Copyright (c) 2022 Simply Embedded Inc. 
 * All Rights Reserved.
 * 
 * SPDX-License-Identifier: MIT
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

void shell(const struct shell_cmd *cmd_list, size_t cmd_list_len);

#endif /* _SHELL_H_ */
