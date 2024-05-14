/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 20:52:22 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/14 17:49:48 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		// write(2, "i\n", 2);
		i++;
	}
	free(tab);
}
// char	**get_path(char **cmd)
// {
// }

int	path(char *argv, char **envp)
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
	cmd = ft_split(argv, ' ');
	path = ft_split(&envp[i][5], ':');
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd[0]);
		// printf("%s\n", path[i]);
		if (access(path[i], X_OK) == 0)
		{
			// printf("path found : %s\n", path[i]);
			break ;
		}
		i++;
	}
	if (execve(path[i], cmd, envp) == -1)
	{
		free_all(path);
		free_all(cmd);
		write(2, "hello\n", 6);
	}
	return (0);
}

int	premier(char **argv, int *pipe)
{
	int	fd;

	// char	buf[10];
	// printf("Ici le fils \n");
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		return (0);
	close(pipe[0]);
	if (dup2(fd, 0) == -1)
		return (0);
	if (dup2(pipe[1], 1) == -1)
		return (0);
	// read(0, buf, 20);
	// printf("%s\n", buf);
	close(fd);
	close(pipe[1]);
	return (0);
}

int	deuxieme(char **argv, int *pipe)
{
	int	fd;

	fd = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (0);
	close(pipe[1]);
	// char	buf[10];
	// read(fd[0], buf, 20);
	// printf("%s\n", buf);
	if (dup2(pipe[0], 0) == -1)
		return (0);
	// read(0, buf, 20);
	// printf("%s\n", buf);
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
	pipe(fd);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		premier(argv, fd);
		path(argv[2], envp);
	}
	else
	{
		deuxieme(argv, fd);
		path(argv[3], envp);
	}
}
