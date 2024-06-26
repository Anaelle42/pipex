/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:33:46 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 21:59:11 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	absolute_path(t_pipex pipex, char **cmd)
{
	if (execve(cmd[0], cmd, pipex.envp) == -1)
	{
		perror(cmd[0]);
		close_all_fd(pipex);
		free(pipex.pipe);
		free_all(cmd);
		exit(1);
	}
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
	print_msg("command not found : ", cmd);
	return (0);
}

void	cmd_path(char **cmd, t_pipex pipex)
{
	char	*path;
	char	**all_paths;

	all_paths = get_all_paths(pipex.envp);
	path = get_path(all_paths, cmd[0]);
	if (!path)
	{
		free_all(all_paths);
		free_all(cmd);
		close_all_fd(pipex);
		free(pipex.pipe);
		exit(1);
	}
	if (execve(path, cmd, pipex.envp) == -1)
	{
		perror("Execve");
		free(path);
		free_all(all_paths);
		free_all(cmd);
		close_all_fd(pipex);
		free(pipex.pipe);
	}
	exit(1);
}

void	exec_cmd(t_pipex pipex, char *arg)
{
	int		i;
	char	**cmd;

	i = 0;
	cmd = ft_split(arg, ' ');
	if (!cmd || !cmd[0])
	{
		if (cmd)
			free_all(cmd);
		close_all_fd(pipex);
		free(pipex.pipe);
		print_msg("command not found : ", " ");
		exit(1);
	}
	while (arg[i])
	{
		if (arg[i] == '/')
			absolute_path(pipex, cmd);
		i++;
	}
	cmd_path(cmd, pipex);
}
