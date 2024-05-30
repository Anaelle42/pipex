/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:23:32 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/30 20:31:35 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
		print_msg("command not found : ", " ");
		exit(1);
	}
	while (argv[i])
	{
		if (argv[i] == '/')
			absolut_vodkapath(cmd, envp);
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
		close(pipe[1]);
		print_error(argv[1]);
	}
	close(fd);
	close(pipe[1]);
	exec_cmd(argv[2], envp);
	return (0);
}

int	childs(char *argv, int *previous_pipe, int *next_pipe, char **envp)
{
	if (dup2(previous_pipe[0], 0) == -1 || (dup2(next_pipe[1], 1) == -1))
	{
		print_error(argv);
	}
	close(previous_pipe[0]);
	close(previous_pipe[1]);
	close(next_pipe[0]);
	close(next_pipe[1]);
	exec_cmd(argv, envp);
	return (0);
}

int	child2(char **argv, int *pipe, char **envp)
{
	int	fd;

	close(pipe[1]);
	fd = open(argv[5], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(pipe[0]);
		print_error(argv[4]);
	}
	if (dup2(pipe[0], 0) == -1 || (dup2(fd, 1) == -1))
	{
		close(pipe[0]);
		print_error(argv[4]);
	}
	close(fd);
	close(pipe[0]);
	exec_cmd(argv[4], envp);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pids[3];
	int		fd[2];
	int		fd2[2];
	int		i;

	if (argc < 5)
		return (print_msg(ERROR1, ARGUMENTS));
	//
	if (pipe(fd) == -1)
		print_error("Pipe");
	if (pipe(fd2) == -1)
		print_error("Pipe");
	//
	i = 0;
	pids[i] = fork();
	if (pids[i] == -1)
		print_error("Fork");
	if (pids[i] == 0)
	{
		close(fd2[0]);
		close(fd2[1]);
		child1(argv, fd, envp);
	}
	//
	{
		i++;
		pids[i] = fork();
		if (pids[i] == 0)
		{
			childs(argv[3], fd, fd2, envp);
		}
	}
	//
	pids[i] = fork();
	if (pids[i] == -1)
		print_error("Fork");
	if (pids[i] == 0)
	{
		close(fd[0]);
		close(fd[1]);
		child2(argv, fd2, envp);
	}
	//
	close(fd[0]);
	close(fd[1]);
	close(fd2[0]);
	close(fd2[1]);
	waitpid(pids[0], NULL, 0);
	waitpid(pids[1], NULL, 0);
	waitpid(pids[2], NULL, 0);
}
