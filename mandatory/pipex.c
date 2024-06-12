/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:13:03 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/11 15:31:07 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(char *argv, char **envp)
{
	int		i;
	char	**cmd;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
	{
		if (cmd)
			free_all(cmd);
		print_msg("command not found :", " ");
		exit(1);
	}
	while (argv[i])
	{
		if (argv[i] == '/')
			absolute_path(cmd, envp);
		i++;
	}
	cmd_path(cmd, envp);
}

int	child1(char **argv, int *pipe, char **envp)
{
	int	fd;

	close(pipe[0]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		close(pipe[1]);
		print_error(argv[1]);
	}
	if (dup2(fd, 0) == -1 || (dup2(pipe[1], 1) == -1))
	{
		close(fd);
		close(pipe[1]);
		print_error(argv[1]);
	}
	close(fd);
	close(pipe[1]);
	exec_cmd(argv[2], envp);
	return (0);
}

int	child2(char **argv, int *pipe, char **envp)
{
	int	fd;

	close(pipe[1]);
	fd = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(pipe[0]);
		print_error(argv[4]);
	}
	if (dup2(pipe[0], 0) == -1 || (dup2(fd, 1) == -1))
	{
		close(fd);
		close(pipe[0]);
		print_error(argv[4]);
	}
	close(fd);
	close(pipe[0]);
	exec_cmd(argv[3], envp);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pids[2];
	int		fd[2];

	if (argc != 5)
		return (print_msg(ERROR1, ARGUMENTS));
	if (pipe(fd) == -1)
		print_error("Pipe");
	pids[0] = fork();
	if (pids[0] == -1)
		print_error("Fork");
	if (pids[0] == 0)
		child1(argv, fd, envp);
	pids[1] = fork();
	if (pids[1] == -1)
		print_error("Fork");
	if (pids[1] == 0)
		child2(argv, fd, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], NULL, 0);
}
