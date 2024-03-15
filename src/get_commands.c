#include "minishell.h"

// get_commands() is a ft_split tailored to minishell.
// It takes into account quotes and divides the given
// string into chuncks according to a delimiter ('|').
// Since open quotes are undefined behaviour, we save
// everything following the quote as if we were waiting for
// the user to close the quote (which is what bash does).
// for example:
//  			> ls -l | echo hello
// 				ls -l
// 				echo hello
//  			> ls -l " | echo hello
// 				ls -l " | echo hello
char	**get_commands(char *line, char c)
{
	char	**commands;
	int		nb_command;

	if (line == NULL)
			return (NULL);
	nb_command = ft_nb_command(line, c);
	// printf("nb_command = %d\n", nb_command);
	commands = calloc_or_exit(sizeof(char *), nb_command + 1);
	commands = ft_split(line, c, nb_command);
	return (commands);
}

int	ft_nb_command(char *line, char c)
{
	int		i;
	int		nb;
	char	quote;

	i = 0;
	nb = 0;
	quote = 0;
	while (line[i])
	{
		quote = check_quote(line[i], quote);
		if (line[i] != c && (line[i + 1] == 0 || (line[i + 1] == c && !quote)))
			nb++;
		i++;
	}
	return (nb);
}

// Super important function! It prevents the shell from
// interpreting the meta-characters in the quoted sequence.
char	check_quote(char letter, char quote)
{
	if ((letter == DBL_QUOTE || letter == SGL_QUOTE) && (!quote || (quote && letter == quote)))
	{
		if (quote)
			quote = 0;
		else
			quote = letter;
	}
	return (quote);
}

// Checks the length of the command until it hits a '|'
// or ignores the '|' if [i] is inside a quote
int	ft_command_len(char *s, char c, int i, char quote)
{
	int	len;

	len = 0;
	while (((s[i + len] != c && !quote) || quote) && s[i + len])
	{
		quote = check_quote(s[i + len], quote);
		len++;
	}
	return (len);
}

char	**ft_command_split(char *s, char c, char **result, int nb_command)
{
	int	a;
	int	i;
	int	j;
	int	len;
	char	quote;

	i = 0;
	a = 0;
	quote = 0;
	while (a < nb_command)
	{
		while (s[i] == c)
			i++;
		len = ft_command_len(s, c, i, quote);
		result[a] = (char *)malloc(sizeof(char) * (len + 1));
		if (!result)
			return (NULL);
		j = 0;
		while (j < len)
			result[a][j++] = s[i++];
		result[a][j] = '\0';
		a++;
	}
	result[a] = NULL;
	return (result);
}

char	**ft_split(char *s, char c, int nb_command)
{
	char	**result;

	if (!s)
		return (NULL);
	result = (char **)malloc((nb_command + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	result = ft_command_split(s, c, result, nb_command);
	return (result);
}
