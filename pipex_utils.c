/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:56:53 by jocardos          #+#    #+#             */
/*   Updated: 2023/03/09 21:11:47 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/libft.h"
#include "pipex.h"

void	print_message(char *str, char *color)
{
	char *reset_color;

	reset_color = DEFAULT;
	ft_printf("%s%s%s\n", color, str, reset_color);
}

void	print_error(char *str)
{
	char *color;
	char *reset_color;

	color = RED;
	reset_color = DEFAULT;
	ft_printf("%s%s%s\n", color, str, reset_color);
	exit(EXIT_FAILURE);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*init_path;
	char	*path;
	char	**paths;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		init_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(init_path, cmd);
		free(init_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (cmd);
}

void	execute(char *argv, char **envp)
{
	char	*path;
	char	**cmd;

	cmd = ft_split(argv, ' ');
	if (ft_strnstr(cmd[0], "/", 1))
		path = cmd[0];
	else
		path = get_path(cmd[0], envp);
	execve(path, cmd, envp);
	print_error("Error: Failed to execute command");
	exit(127);
}
