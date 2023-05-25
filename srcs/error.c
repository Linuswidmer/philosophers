/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:23:56 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/25 13:24:04 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error_msg(t_exit_code exit_code)
{
	if (exit_code == MALLOC_FAIL)
		printf("Dynamic memory allocation failed\n");
	else if (exit_code == INPUT_FAIL)
		printf("Invalid Input\n");
	else if (exit_code == INIT_FAIL)
		printf("Fail in initialization\n");
}
