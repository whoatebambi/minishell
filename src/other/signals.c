/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/28 16:17:47 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void set_signals(t_mode mode)
{
    if (mode == INTERACTIVE)
    {
        signal(SIGINT, sig_handler); // ctrl + c
        //signal(SIGTERM, SIG_IGN); // ctrl + d
        signal(SIGQUIT, SIG_IGN); // ctrl + /\/
    }
}

void	sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        write(1, "\n", 1);
        //rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}