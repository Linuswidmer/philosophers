# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/24 13:46:23 by lwidmer           #+#    #+#              #
#    Updated: 2023/05/24 14:45:08 by lwidmer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

FILENAMES = philo utils

SRCS_DIR = ./srcs/
SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(FILENAMES)))

OBJS_DIR = ./srcs/
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILENAMES)))

CFLAGS = -Werror -Wall -Wextra

INCLUDES_DIR = -I ./includes/

${NAME}: ${OBJS}
	cc -o philo $^ 

.c.o: ${SRCS}
	cc -c -o $@ $< ${INCLUDES_DIR}

test:
	./philo 2 3 4 5
