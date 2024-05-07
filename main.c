/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 20:52:22 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/07 19:18:13 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	path(char **argv, char **envp)
{
	int		i;
	char	**path;
	char	**cmd;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	path = ft_split(&envp[i][5], ':');
	i = 0;
	// while (path[i])
	// {
	// 	printf("%s\n", path[i]);
	// 	i++;
	// }
	cmd = ft_split(argv[2], ' ');
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd[0]);
		// printf("%s\n", path[i]);
		if (access(path[i], X_OK) == 0)
		{
			printf("path found : %s\n", path[i]);
			break ;
		}
		i++;
	}
	execve(path[i], cmd, envp);
	return (0);
}

int	premier(char **argv, int *fd)
{
	int	fd_file;

	// char	buf[10];
	// printf("Ici le fils \n");
	fd_file = open(argv[1], O_RDONLY);
	if (fd_file == -1)
		return (0);
	dup2(fd_file, 0);
	dup2(fd[1], 1);
	// read(0, buf, 20);
	// printf("%s\n", buf);
	close(fd[0]);
	close(fd[1]);
	return (0);
}

int	deuxieme(char **argv, int *pipe)
{
	int	fd_final;

	fd_final = open(argv[4], O_RDWR);
	// char	buf[10];
	// read(fd[0], buf, 20);
	// printf("%s\n", buf);
	dup2(pipe[0], 0);
	// read(0, buf, 20);
	// printf("%s\n", buf);
	// dup2(fd_final, 1);
	(void)argv;
	close(pipe[0]);
	close(pipe[1]);
	return (0);
}

int	path2(char **argv, char **envp)
{
	int		i;
	char	**path;
	char	**cmd;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	path = ft_split(&envp[i][5], ':');
	i = 0;
	// while (path[i])
	// {
	// 	printf("%s\n", path[i]);
	// 	i++;
	// }
	cmd = ft_split(argv[3], ' ');
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd[0]);
		// printf("%s\n", path[i]);
		if (access(path[i], X_OK) == 0)
		{
			printf("path found : %s\n", path[i]);
			break ;
		}
		i++;
	}
	execve(path[i], cmd, envp);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		fd[2];

	(void)argc;
	// if (argc != 5)
	// 	return (0);
	pipe(fd);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		premier(argv, fd);
		path(argv, envp);
	}
	else
	{
		// printf("Ici le père\n");
		deuxieme(argv, fd);
		path2(argv, envp);
	}
}
