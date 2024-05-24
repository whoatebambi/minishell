/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/24 17:58:00 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// while waiting for commands table parsing i am creating a dummy one here
t_command    **temp_parse_commands(char *line)
{
    t_command	**commands;
	char 		**temp_cmd_table;
    int			i;

    commands = (t_command **)malloc(sizeof(t_command *) * 50);
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

void    echo(char *commands)
{
    int i = 0;
    while (commands[i] == 'e')
        i++;
    while (commands[i] == 'c')
        i++;
    while (commands[i] == 'h')
        i++;
    while (commands[i] == 'o')
        i++;
    while (commands[i] != '\0')
	{
		write(1, &commands[i], 1);
		i++;
	}
	write(1, "\n", 1);
}

void    execute_builtin(t_command *command)
{
    if (ft_strncmp(command->commands, "pwd", 3) == 0) // && ft_strlen(command->commands) == 3)
		pwd();
    if (ft_strncmp(command->commands, "echo", 4) == 0)
		echo(command->commands);
}

void	execute(t_shell *minishell, char *line)
{
    int i;

    i = 0;
    // while waiting for commands table parsing i am creating a dummy one here
	minishell->commands = temp_parse_commands(line);

    while (minishell->commands[i] != NULL)
    {
        if (minishell->commands[i]->builtin == 1)
            execute_builtin(minishell->commands[i]);    
        i++;
    }
    
    i = 0;
    while (minishell->commands[i] != NULL)
    {
        free(minishell->commands[i]);
        i++;
    }
    free(minishell->commands);
}
