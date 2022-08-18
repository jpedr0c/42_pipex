/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:56:53 by jocardos          #+#    #+#             */
/*   Updated: 2022/08/18 11:01:56 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/libft.h"
#include "pipex.h"

void	error(void)
{
	ft_putstr_fd("Error!!", 2);
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
	int		i;
	char	*path;
	char	**cmd;

	i = -1;
	cmd = ft_split(argv, ' ');
	if (ft_strnstr(cmd[0], "/", 1))
		path = cmd[0];
	else
		path = get_path(cmd[0], envp);
	execve(path, cmd, envp);
	error();
	exit(127);
}
