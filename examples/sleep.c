/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:16:26 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/24 11:17:46 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Program started.\n");

    // Sleep for 1 second (1,000,000 microseconds)
    usleep(1000000);

    printf("Program resumed after 1 second.\n");

    return 0;
}
