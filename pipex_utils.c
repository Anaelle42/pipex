/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 20:52:22 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/16 18:49:19 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_all(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**get_all_paths(char **envp)
{
	while (*envp)
	{
		if (ft_strnstr(*envp, "PATH=", 5))
			break ;
		envp++;
	}
	if (*envp)
		return (ft_split(*envp + 5, ':'));
	return (NULL);
}

char	*get_path(char **all_paths, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;

	i = 0;
	while (all_paths && cmd && all_paths[i])
	{
		tmp = ft_strjoin(all_paths[i], "/");
		if (!tmp)
			return (0);
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!path)
			return (0);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	if (cmd && access(cmd, X_OK) == 0)
		return (cmd);
	return (0);
}

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
