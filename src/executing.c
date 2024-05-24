/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/24 20:43:55 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// while waiting for commands table parsing i am creating a dummy one here
t_cmd_table    **temp_parse_commands(char *line)
{
    t_cmd_table	**cmd_table;
	char 		**temp_cmd_table;
    int			i;

    cmd_table = (t_cmd_table **)malloc(sizeof(t_cmd_table *) * 50);
    i = 0;
    temp_cmd_table = ft_split(line, '|');
    while (temp_cmd_table[i] != NULL)
    {
        cmd_table[i] = (t_cmd_table *)malloc(sizeof(t_cmd_table));
		cmd_table[i]->cmd_args =  ft_split(temp_cmd_table[i], ' ');
        cmd_table[i]->builtin = 1;
        i++;
    }
    cmd_table[i] = NULL;
	return (cmd_table);
}

void	pwd(void)
{
	char *buffer;
	// char buffer[PATH_MAX] // # define PATH_MAX  4096
	
	buffer = NULL;
	buffer = getcwd(buffer, 4096); 
    if (buffer == NULL)
	{
		perror("Error");
		free(buffer);
		return;
	}
	ft_putstr_fd(buffer, 1);
	write(1, "\n", 1);
	free(buffer);
}

void    echo(char **cmd_args)
{
    int i;
	
	i = 1;
	while (cmd_args[i])
	{
		ft_putstr_fd(cmd_args[i], 1);
		write(1, " ", 1);
		i++;
	}
    // if (flag == '-n')
	// 	return;
	write(1, "\n", 1);
}

void    execute_builtin(t_cmd_table *cmd_table)
{
    if (ft_strncmp(cmd_table->cmd_args[0], "pwd", 3) == 0) // && ft_strlen(command->commands) == 3)
		pwd();
    if (ft_strncmp(cmd_table->cmd_args[0], "echo", 4) == 0)
		echo(cmd_table->cmd_args);
}

void	execute(t_shell *minishell, char *line)
{
    int i;
	int	j;

    i = 0;
    // while waiting for commands table parsing i am creating a dummy one here
	minishell->cmd_table = temp_parse_commands(line);
	/// print cmd_table
	while (minishell->cmd_table[i])
	{
		j = 0;
		while (minishell->cmd_table[i]->cmd_args[j])
		{
			printf("cmd_table[%d]->cmd_args[%d] = %s\n", i, j, minishell->cmd_table[i]->cmd_args[j]);
			j++;
		}
		i++;
	}
	///
	i = 0;
    while (minishell->cmd_table[i] != NULL)
    {
        if (minishell->cmd_table[i]->builtin == 1)
            execute_builtin(minishell->cmd_table[i]);    
        i++;
    }
    
    i = 0;
    while (minishell->cmd_table[i] != NULL)
    {
        free(minishell->cmd_table[i]);
        i++;
    }
    free(minishell->cmd_table);
}
