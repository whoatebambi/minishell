
void	exec_system(t_shell *minishell, int i)
{
	char	**execve_envp;
	char	**execve_path_table;
	char	*execve_path;

	char *tmp;
	tmp = ft_strtrim(minishell->cmd_table[i]->cmd_args[0], "\n");
	
	execve_envp = build_execve_envp(minishell->env_lst);
	execve_path_table = build_execve_path(minishell->env_lst);
	
	if (ft_strncmp("/", tmp, 1) == 0)
	{
		execve_path = tmp;
		printf("ABSOLUTE PATH = %s\n", execve_path);
	}
	else
	{
		execve_path = find_relative_path(tmp, execve_path_table);
		printf("RELATIVE PATH = %s\n", execve_path);
	}
	// if (execve_path == NULL)
	// 	perror("access");
	// ft_free_double_char(execve_envp);
	ft_free_double_char(execve_path_table);
	execve(execve_path, minishell->cmd_table[i]->cmd_args, execve_envp);
	//perror("execve");
}

char	*find_relative_path(char *arg, char **execve_path_table)
{
	char	*execve_path;
	char	*path_arg;
	int		i;

	i = 0;
	path_arg = ft_strjoin_no_free("/", arg);
	while (execve_path_table[i])
	{
		execve_path = ft_strjoin_no_free(execve_path_table[i], path_arg);
		if (access(execve_path, F_OK | X_OK) == 0)
		{
			free(path_arg);
			printf("FOUND RELATIVE PATH = %s\n", execve_path);
			return (execve_path);
		}
		free(execve_path);
		i++;
	}
	free(path_arg);
	return (execve_path);
}