# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/24 13:46:23 by lwidmer           #+#    #+#              #
#    Updated: 2023/07/07 16:46:01 by lwidmer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

FILENAMES = philo init time utils

SRCS_DIR = ./srcs/
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILENAMES)))

OBJS_DIR = ./srcs/
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILENAMES)))

CFLAGS = -Werror -Wall -Wextra

INCLUDES_DIR = -I ./includes/

${NAME}: ${OBJS}
	cc -o philo $^ 

.c.o: ${SRCS}
	cc -c -o $@ $< ${INCLUDES_DIR} -g

val: ${NAME}
	valgrind ./philo 3 6000 500000 1000 

hel: ${NAME}
	valgrind --tool=helgrind ./philo 3 6000 500000 1000 

test: ${NAME}
	./philo 3 6000 500000 1000

test2:
	./philo 3 600 500000 5

test3:
	./philo 10 60000 500000 5
