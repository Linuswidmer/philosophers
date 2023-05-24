/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timestamps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:10:42 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/24 11:14:36 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>

long long current_timestamp_ms() {
    struct timeval te;
    gettimeofday(&te, NULL); // Get current time
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // Calculate milliseconds
    return milliseconds;
}

int main() {
    long long timestamp_ms = current_timestamp_ms();
    printf("%lld\n", timestamp_ms);
    return 0;
}

