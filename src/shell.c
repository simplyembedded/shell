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

    while (1) {
        struct _input *input = &(history[input_idx]);
        int c = getchar();
        if ((c == '\n') || (c == '\r')) {
            if (input->len > 0) {
                if(_parse(input->buffer, input->len) < 0) {
                    break;
                } else {
                    if (input_idx < SHELL_CONFIG_HISTORY_MAX) {
                        input_idx++;
                    }
                }
            }                    

            PROMPT();
		} else if (c == 0x1B) {
			seq = 1;   
		} else if (seq && (c == 0x5B)) {
			dir = 1;
		} else if ((seq == 1) && (dir == 1) && (c == 0x41)) {
			/* up arrow */
			seq = dir = 0;

            if (input_idx > 0) {
                input_idx--;
                printf("\r$ ");
				fputs(history[input_idx].buffer, stdout);
			}
		} else if ((seq == 1) && (dir == 1) && (c == 0x42)) {
		    /* down arrow */
			seq = dir = 0;
			
             if (input_idx < SHELL_CONFIG_HISTORY_MAX) {
                 if (input->len > 0) {
                    input_idx++;
                    printf("\r$ ");
				    fputs(history[input_idx].buffer, stdout);
                 }
			}
		} else if ((seq == 1) && (dir == 1) && (c == 0x43)) {
			/* right arrow */
			seq = dir = 0;
		} else if ((seq == 1) && (dir == 1) && (c == 0x44)) {
			/* left arrow */
			putchar('\b');
			seq = dir = 0;
         } else if ((c >= 0x20) && (c < 0x7E)) {
             if (input->len < SHELL_CONFIG_INPUT_BUFFER_MAX) {
                 putchar(c);
                 input->buffer[input->len++] = (char) c;
             }
         } else if ((c == 0x08) || (c == 0x7F)) {
            if(input->len > 0) {
                fputs("\b \b", stdout);
                input->buffer[--input->len] = '\0';
            }
        }
    }
}

static int _parse(char *buffer, size_t len)
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

        for(i = 0; i < shell_cmd_list_len; i++) {
            if(!strcmp(shell_cmd_list[i].name, argv[0])) {
                if(shell_cmd_list[i].funcptr(argc, argv) != 0) {
                    printf("%s: %s\n\tUsage %s", shell_cmd_list[i].name, shell_cmd_list[i].desc, shell_cmd_list[i].usage);
                }
                break;
            } 
        }

        if (i == shell_cmd_list_len) {
            printf("\n%s: command not found\n", argv[0]);
        }
    }
    
    return 0; 
}

