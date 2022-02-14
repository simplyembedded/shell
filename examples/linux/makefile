# Example makefile to build shell application for Linux

PROJECT_ROOT:=../..
SRC_DIR:=$(PROJECT_ROOT)/src
INCLUDE_DIR:=$(PROJECT_ROOT)/include

shell: $(SRC_DIR)/shell.c main.c 
	gcc -O0 -g3 $^ -I$(INCLUDE_DIR) -I. -o $@
