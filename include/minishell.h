/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbeaudoi <gbeaudoi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 18:24:48 by gbeaudoi          #+#    #+#             */
/*   Updated: 2024/02/27 20:15:22 by gbeaudoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/ft_printf.h"
# include "../libft/include/libft.h"
# include "../libft/include/get_next_line_bonus.h"

#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


typedef enum e_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
}			t_mode;

typedef struct s_minishell
{
	t_mode		mode;
	int			nb_cmds;
	char		**instructions;
	// t_list		*env;
	// char		**env_paths;
	// t_cmd_table	*cmd_table;
	pid_t		*child_pids;
	int			exit_code;
}				t_minishell;


#endif