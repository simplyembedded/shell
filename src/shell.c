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
#include "shell_config.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PROMPT_PREFIX          "\r\n$ "
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

void shell(const struct shell_cmd *cmd_list, size_t cmd_list_len)
{
    static struct shell_ctx ctx;

    memset(&ctx, 0, sizeof(ctx));

    ctx.cmd_list = cmd_list;
    ctx.cmd_list_len = cmd_list_len;

    SHELL_PUTS(PROMPT_PREFIX);

    while (1) {
        struct input *input = &ctx.cmd_history[ctx.cmd_idx];
        int c = SHELL_GETC();
        if ((c == '\r') || (c == '\n')) {
            if (input->len > 0) {
                if(_parse(&ctx, input->buffer, input->len) < 0) {
                    break;
                } else {
                    if (ctx.cmd_idx < SHELL_CONFIG_HISTORY_MAX) {
                        ctx.cmd_idx++;
                    }
                }
            }                    

            SHELL_PUTS(PROMPT_PREFIX);
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
                SHELL_PUTS("\r$ ");
                SHELL_PUTS(ctx.cmd_history[ctx.cmd_idx].buffer);
			}
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x42)) {
		    /* down arrow */
			ctx.seq = false; 
            ctx.dir = false;
			
             if (ctx.cmd_idx < SHELL_CONFIG_HISTORY_MAX) {
                 if (input->len > 0) {
                    ctx.cmd_idx++;
                    SHELL_PUTS("\r$ ");
                    SHELL_PUTS(ctx.cmd_history[ctx.cmd_idx].buffer);
                 }
			}
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x43)) {
			/* right arrow */
			ctx.seq = false;
            ctx.dir = false;
		} else if ((ctx.seq == true) && (ctx.dir == true) && (c == 0x44)) {
			/* left arrow */
			putchar('\b');
			ctx.seq = false;
            ctx.dir = false;
         } else if ((c >= 0x20) && (c < 0x7E)) {
             if (input->len < SHELL_CONFIG_INPUT_BUFFER_MAX) {
                 SHELL_PUTCHAR(c);
                 input->buffer[input->len++] = (char) c;
             }
         } else if ((c == 0x08) || (c == 0x7F)) {
            if(input->len > 0) {
            	SHELL_PUTS("\b \b");
                input->buffer[--input->len] = '\0';
            }
        }
    }
}

static int _parse(struct shell_ctx *ctx, char *buffer, size_t len)
{
    if (!strcmp(buffer, "exit")) {
        return -1;
    } else {    
        char scratch[SHELL_CONFIG_INPUT_BUFFER_MAX];
        int argc = 1;
        char *argv[SHELL_CONFIG_INPUT_ARGS_MAX];
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
                if (ctx->cmd_list[i].funcptr(argc, argv) != 0) {
                    SHELL_PUTS(ctx->cmd_list[i].name);
                    SHELL_PUTS(": ");
                    SHELL_PUTS(ctx->cmd_list[i].desc);
					SHELL_PUTS("\n\tUsage ");
					SHELL_PUTS(ctx->cmd_list[i].usage);
                }
                break;
            } 
        }

        if (i == ctx->cmd_list_len) {
        	SHELL_PUTS("\n");
        	SHELL_PUTS(argv[0]);
			SHELL_PUTS(": Command not found\n");
        }
    }
    
    return 0; 
}

