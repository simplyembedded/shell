/**
 * \file shell.c
 * \brief Minimal POSIX shell implementation
 *
 * Copyright (c) 2022 Simply Embedded Inc. 
 * All Rights Reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

#include "shell.h"
#include "shell_port.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define is_space(x)             ((x)==' ' || (x)=='\t')

struct input 
{
    char buffer[SHELL_CONFIG_INPUT_BUFFER_MAX];
    size_t len;
};

struct shell_ctx
{
    const struct shell_cmd *cmd_list;
    size_t cmd_list_len;
    struct input cmd_history[SHELL_CONFIG_HISTORY_MAX]; 
    size_t cmd_idx;
    bool seq;
    bool dir; 
};

static int _parse(struct shell_ctx *ctx, char *buffer, size_t len);

/**
 * @brief prints available commands
 * 
 * @param argc unused
 * @param argv unused
 * @return int 0
 */
static int _cli_help(struct shell_ctx *ctx)
{
    shell_puts("Console Help - available commands:"SHELL_NEWLINE);
    for (size_t i = 0; i < ctx->cmd_list_len; i++) {
        shell_printf(SHELL_NEWLINE"Command: %s"SHELL_NEWLINE"-----------------"SHELL_NEWLINE, ctx->cmd_list[i].name);
        shell_printf("Usage: %s"SHELL_NEWLINE,
                     (ctx->cmd_list[i].usage == NULL) ? "" : ctx->cmd_list[i].usage);
        shell_printf("Description: %s"SHELL_NEWLINE, ctx->cmd_list[i].desc);
    }

    shell_printf(SHELL_NEWLINE"Shell Control Commands"SHELL_NEWLINE"-----------------"SHELL_NEWLINE);
    shell_printf("exit: closes the shell task"SHELL_NEWLINE);
    shell_printf("help: prints this menu"SHELL_NEWLINE);
    shell_printf(SHELL_NEWLINE);
    return 0;
}

void shell(const struct shell_cmd *cmd_list, size_t cmd_list_len)
{
    shell_printf(SHELL_NEWLINE"SIMPLY EMBEDDED POSIX SHELL"SHELL_NEWLINE);
    shell_printf("Try \"help\" to view available commands"SHELL_NEWLINE);
    static struct shell_ctx ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.cmd_list = cmd_list;
    ctx.cmd_list_len = cmd_list_len;

    shell_puts(SHELL_PROMPT);

    while (1) {
        struct input *input = &ctx.cmd_history[ctx.cmd_idx];
        int c = shell_getchar();
        if ((c == '\r') || (c == '\n')) {
            shell_puts(SHELL_NEWLINE);
            if (input->len > 0) {
                if(_parse(&ctx, input->buffer, input->len) < 0) {
                    break;
                } else {
                    if (ctx.cmd_idx < SHELL_CONFIG_HISTORY_MAX) {
                        ctx.cmd_idx++;
                    }
                }
            }                    

            shell_puts(SHELL_PROMPT);
		} else if (c == 0x1B) {
			ctx.seq = true; 
		} else if (ctx.seq && (c == 0x5B)) {
			ctx.dir = true;
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x41)) {
			/* up arrow */
			ctx.seq = false;
            ctx.dir = false;

            if (ctx.cmd_idx > 0) {
                ctx.cmd_idx--;
                shell_puts("\r$ ");
                shell_puts(ctx.cmd_history[ctx.cmd_idx].buffer);
			}
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x42)) {
		    /* down arrow */
			ctx.seq = false; 
            ctx.dir = false;
			
             if (ctx.cmd_idx < SHELL_CONFIG_HISTORY_MAX) {
                 if (input->len > 0) {
                    ctx.cmd_idx++;
                    shell_puts("\r$ ");
                    shell_puts(ctx.cmd_history[ctx.cmd_idx].buffer);
                 }
			}
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x43)) {
			/* right arrow */
			ctx.seq = false;
            ctx.dir = false;
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x44)) {
			/* left arrow */
			shell_putchar('\b');
			ctx.seq = false;
            ctx.dir = false;
         } else if ((c >= 0x20) && (c < 0x7E)) {
             if (input->len < SHELL_CONFIG_INPUT_BUFFER_MAX) {
                #if SHELL_ECHO
                    shell_putchar(c);
                #endif
                 input->buffer[input->len++] = (char) c;
             }
         } else if ((c == 0x08) || (c == 0x7F)) {
            if(input->len > 0) {
            	shell_puts(SHELL_BACKSPACE);
                input->buffer[--input->len] = '\0';
            }
        }
    }
}

static int _parse(struct shell_ctx *ctx, char *buffer, size_t len)
{
    if (!strcmp(buffer, "exit")) {
        return -1;
    } else if (!strcmp(buffer, "help")){
        _cli_help(ctx);
    } else {    
        char scratch[SHELL_CONFIG_INPUT_BUFFER_MAX];
        int argc = 1;
        const char *argv[SHELL_CONFIG_INPUT_ARGS_MAX];
        size_t i = 0;
  
        memcpy(scratch, buffer, len);
        scratch[len] = '\0';

        /* Skip leading spaces */
        while(is_space(scratch[i])) {
            i++;
        }

        argv[0] = &(scratch[i++]);
        while(i < len) {
            if (scratch[i] == ' ') {
                scratch[i] = '\0';
                argv[argc++] = &(scratch[++i]);
            } else {
                i++;
            }
        }

        for (i = 0; i < ctx->cmd_list_len; i++) {
            if (!strcmp(ctx->cmd_list[i].name, argv[0])) {
                int errcode = ctx->cmd_list[i].funcptr(argc, argv);
                if (errcode != 0) {
                    shell_printf("%s: returned non-zero error code [%d]"SHELL_NEWLINE, ctx->cmd_list[i].name, errcode);
                    shell_printf("Usage: %s"SHELL_NEWLINE, ctx->cmd_list[i].usage);
                }
                break;
            } 
        }

        if (i == ctx->cmd_list_len) {
            shell_printf("%s: Command not found"SHELL_NEWLINE, argv[0]);
        }
    }
    
    return 0; 
}

