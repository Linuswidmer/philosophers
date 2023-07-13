/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <lwidmer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 08:10:47 by lwidmer           #+#    #+#             */
/*   Updated: 2023/07/13 08:12:48 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long current_timestamp_ms(void) 
{
	struct timeval te;
	long long milliseconds;
    
	gettimeofday(&te, NULL);
	// this can probably fail as well
	// this is actually microseconds
	milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
	return (milliseconds);
}
