/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/05/28 16:22:43 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// while waiting for commands table parsing i am creating a dummy one here
t_cmd_table    **temp_parse_commands(char *line)
{
    t_cmd_table	**cmd_table;
	char 		**tmp_cmd_table;
    int			i;

    cmd_table = (t_cmd_table **)malloc(sizeof(t_cmd_table *) * 50);
    i = 0;
    tmp_cmd_table = ft_split(line, '|');
    while (tmp_cmd_table[i] != NULL)
    {
        cmd_table[i] = (t_cmd_table *)malloc(sizeof(t_cmd_table));
		cmd_table[i]->cmd_args =  ft_split(tmp_cmd_table[i], ' ');
        i++;
    }
    cmd_table[i] = NULL;
    i = 0;
    while (tmp_cmd_table[i] != NULL)
    {
        free(tmp_cmd_table[i]);
        i++;
    }
    free(tmp_cmd_table);
	return (cmd_table);
}

void	pwd(void)
{
	char *buffer;
	// TD char buffer[PATH_MAX] // # define PATH_MAX  4096
	
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
    // TD (flag == '-n')
	write(1, "\n", 1);
}

void	env(t_list *env_lst)
{
    t_env *env;

    if (env_lst == NULL)
        return ;
    while (env_lst)
    {
        env = (t_env *)env_lst->content;
        char *key = env->key;
        ft_putstr_fd(key, 1);
        char *var = env->var;
        write(1, "=", 1);
        ft_putstr_fd(var, 1);
        write(1, "\n", 1);
        env_lst = env_lst->next;
    }
}

void    execute_builtin(t_cmd_table *cmd_table, t_list *env_lst)
{
    if (ft_strncmp(cmd_table->cmd_args[0], "pwd", 3) == 0) // && ft_strlen(command->commands) == 3)
		pwd();
    else if (ft_strncmp(cmd_table->cmd_args[0], "echo", 4) == 0)
		echo(cmd_table->cmd_args);
    else if (ft_strncmp(cmd_table->cmd_args[0], "env", 3) == 0) // && ft_strlen(command->commands) == 3)
		env(env_lst);
}

void	execute(t_shell *minishell, char *line)
{
    int i;
	int	j;

    i = 0;
    // while waiting for commands table parsing i am creating a dummy one here
	minishell->cmd_table = temp_parse_commands(line);
	// /// print cmd_table
	// while (minishell->cmd_table[i])
	// {
	// 	j = 0;
	// 	while (minishell->cmd_table[i]->cmd_args[j])
	// 	{
	// 		printf("cmd_table[%d]->cmd_args[%d] = %s\n", i, j, minishell->cmd_table[i]->cmd_args[j]);
	// 		j++;
	// 	}
	// 	i++;
	// }
	i = 0;
    while (minishell->cmd_table[i] != NULL)
    {
        execute_builtin(minishell->cmd_table[i], minishell->env_lst);    
        i++;
    }
}
