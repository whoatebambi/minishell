/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parameters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcouserg <fcouserg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:27:20 by fcouserg          #+#    #+#             */
/*   Updated: 2024/02/23 20:09:35 by fcouserg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_argc(int argc)
{
	if (argc > 2) // usage is either "./minishell" or "./minishell <file>"
	{
		errno = E2BIG; // the int errno is set to the macro "E2BIG"
		perror("Error");
		exit(E2BIG); // "E2BIG" macro from <errno.h> expands to "Argument list too long."
	}
}

int	init_fd(int argc, char **argv, int fd)
{
	fd = 0;
	if (argc == 2) // opens the file (we are in non-interactive mode)
		fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		// no need to set errno to a macro here, the system call open() automatically did it
		perror("Error");
		//  When a system call fails, it usually returns -1 and sets the variable errno
		// to a value describing what went wrong. (These values can be found in <errno.h>.) 
		// Many library functions do likewise.  The function perror() serves to translate
		// this error code into human-readable form.
		exit(EXIT_FAILURE);
		// Indicates unsuccessful termination. However, its usage is non-portable.
		// For example, on OpenVMS, exit(1) actually indicates success.
		// Only EXIT_FAILURE is the standard value for returning unsuccessful termination,
		// but 1 is used for the same in many implementations.
	}
    return (fd);
}


t_list  *init_env_lst(char **envp)
{
    t_list  *env_lst;
    t_env   *env;
    int     i;

    // parse envp and create a linked list of env variables

    
    return (env_lst);
}