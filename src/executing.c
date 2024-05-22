/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/02/23 20:09:35 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// while waiting for commands table parsing i am creating a dummy one here
t_command    **temp_parse_commands(char *line)
{
    t_command	**commands;
	char 		**temp_cmd_table;
    int			i;

	ft_printf("line = %s\n", line);
    commands = (t_command **)malloc(sizeof(t_command *) * 5);
    i = 0;
    temp_cmd_table = ft_split(line, '|');
    while (temp_cmd_table[i] != NULL)
    {
        commands[i] = (t_command *)malloc(sizeof(t_command));  // Allocate memory for each t_command
        commands[i]->commands = temp_cmd_table[i];
        commands[i]->builtin = 1;
        ft_printf("command[%d] = %s\n", i, temp_cmd_table[i]);
        i++;
    }
    commands[i] = NULL;
	return (commands);
}


void    execute_builtin(t_shell *minishell, t_command *command)
{
	// pwd
    if (ft_strncmp(command->commands, "pwd", 3) == 0);// && ft_strlen(command->commands) == 3)
	{
		char *cwd_str;
		cwd_str = getcwd(cwd_str, 0);
		ft_putstr_fd(cwd_str, 1);
		write(1, "\n", 1);
		free(cwd_str);
	}
}

void	execute(t_shell *minishell)
{
    int i;

    i = 0;
    // while waiting for commands table parsing i am creating a dummy one here
	minishell->commands = temp_parse_commands("pwd | hello"); //("pwd | ls -l | grep .c | wc -l");

    while (minishell->commands[i] != NULL)
    {

        if (minishell->commands[i]->builtin == 1)
            execute_builtin(minishell, minishell->commands[i]);    
        i++;
    }

}

// // permet d avoir toutes les info d une commande
// typedef struct s_command
// {
// 	char		*commands;
// 	int			builtin;
// 	int			path;
// 	char		**option;
// 	char		**words;
// 	char 		*redir;
// }				t_command;