/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocardos <jocardos@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:49:04 by jocardos          #+#    #+#             */
/*   Updated: 2023/03/11 11:09:25 by jocardos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/include/libft.h"
# include "libft/include/ft_printf.h"
# include "colors.h"

// PIPEX
void	child_process(char **argv, char **envp, int *fd);
void	parent_process(char **argv, char **envp, int *fd);

// PIPEX_UTILS
void	print_error(char *str);
char	*get_path(char *cmd, char **envp);
void	execute(char *argv, char **envp);

#endif
