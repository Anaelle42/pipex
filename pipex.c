/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:13:03 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/16 18:55:02 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// erreurs dont infile X

int	exec_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	char	**all_paths;

	all_paths = get_all_paths(envp);
	cmd = ft_split(argv, ' ');
	if (!cmd)
	{
		free_all(all_paths);
		return (0);
	}
	path = get_path(all_paths, cmd[0]);
	if (!path)
	{
		free_all(all_paths);
		free_all(cmd);
		return (0);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		free_all(all_paths);
		free_all(cmd);
	}
	return (0);
}

int	cmd1(char **argv, int *pipe)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (0);
	close(pipe[0]);
	if (dup2(fd, 0) == -1)
		return (0);
	if (dup2(pipe[1], 1) == -1)
		return (0);
	close(fd);
	close(pipe[1]);
	return (0);
}

int	cmd2(char **argv, int *pipe)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (0);
	close(pipe[1]);
	if (dup2(pipe[0], 0) == -1)
		return (0);
	if (dup2(fd, 1) == -1)
		return (0);
	close(fd);
	close(pipe[0]);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (argc != 5)
		return (0);
	if (pipe(fd) == -1)
		return (0);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		cmd1(argv, fd);
		exec_cmd(argv[2], envp);
	}
	else
	{
		cmd2(argv, fd);
		exec_cmd(argv[3], envp);
	}
	waitpid(pid, NULL, 0);
}
