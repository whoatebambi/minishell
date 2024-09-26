/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/09/26 13:21:53 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_sig(int s)
{
	g_sig = s;
	if (wait(NULL) != -1)
	{
		if (s == SIGQUIT)
			printf("Quit (core dumped)");
		printf("\n");
	}
	else if (s == SIGINT)
	{
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (s == SIGQUIT)
		g_sig = 0;
}

void	ft_sig_heredoc(int s)
{
	g_sig = s;
	close(STDIN_FILENO);
	write(2,"^C",2);
}

void	ft_sig_heredoc_setting(void)
{
	struct sigaction	sig;

	sig.sa_flags = 0;
	sig.sa_handler = &ft_sig_heredoc;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGINT, &sig, NULL);
}

int	ft_catchsignals(t_shell *minishell)
{
	if (g_sig == SIGINT)
	{
		minishell->tmpexcode = 130;
		g_sig = 0;
		return (1);
	}
	else if (g_sig == SIGQUIT)
	{
		minishell->tmpexcode = 131;
		g_sig = 0;
		return (1);
	}
	return (0);
}

void	ft_signals(void)
{
	struct sigaction	sig;

	rl_catch_signals = 0;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	sig.sa_handler = &ft_handle_sig;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
	sigaction(SIGQUIT, &sig, NULL);
}