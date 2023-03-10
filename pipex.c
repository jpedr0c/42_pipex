/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 17:58:52 by jocardos          #+#    #+#             */
/*   Updated: 2023/03/09 19:53:28 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/include/libft.h"
#include "pipex.h"

void	child_process(char **argv, char **envp, int *fd)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile == -1)
		print_error("Error: Failed to open input file");
	dup2(fd[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(fd[0]);
	execute(argv[2], envp);
}

void	parent_process(char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		print_error("Error: Failed to open or create output file");
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			print_error("Error: Failed to create pipe");
		pid1 = fork();
		if (pid1 == -1)
			print_error("Error: Failed to fork process");
		if (pid1 == 0)
			child_process(argv, envp, fd);
		waitpid(pid1, NULL, 0);
		parent_process(argv, envp, fd);
	}
	else
	{
		print_error("Error: Wrong number of arguments\n \
		Correct format: ./pipex infile cmd1 cmd2 outfile");
	}
	return (0);
}
