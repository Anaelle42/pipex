/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:23:32 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/03 18:20:18 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//
// if (ft_strnstr(argv[1], "here_doc", 8))
// 	here_doc();
//

void	exec_cmd(char *argv, char **envp, t_pipex pipex)
{
	int		i;
	char	**cmd;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
	{
		if (cmd)
			free_all(cmd);
		close_all_fd(pipex);
		free(pipex.pipe);
		print_msg("command not found : ", " ");
		exit(1);
	}
	while (argv[i])
	{
		if (argv[i] == '/')
			absolut_vodkapath(cmd, envp, pipex);
		i++;
	}
	cmd_path(cmd, envp, pipex);
}

int	open_infile(t_pipex pipex, char *argv, int *pipe)
{
	int	fd;

	fd = open(argv, O_RDONLY);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error(argv);
	}
	if (dup2(fd, 0) == -1 || (dup2(pipe[0], 0) == -1))
	{
		close(fd);
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error(argv);
	}
	close(fd);
	return (0);
}

int	childs(t_pipex pipex, char **argv, int index)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			open_infile(pipex, argv[1], pipex.pipe);
		if (dup2(*(pipex.pipe + (index * 2)), 0) == -1 || (dup2(*(pipex.pipe
						+ (index * 2) + 3), 1) == -1))
		{
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(argv[index + 2]);
		}
		close_all_fd(pipex);
		exec_cmd(argv[index + 2], pipex.envp, pipex);
	}
	return (0);
}

int	last_one(int argc, char **argv, t_pipex pipex, int *pipe)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
		{
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(argv[argc - 1]);
		}
		if (dup2(pipe[0], 0) == -1 || (dup2(fd, 1) == -1))
		{
			close(fd);
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(argv[argc - 2]);
		}
		close(fd);
		close_all_fd(pipex);
		exec_cmd(argv[argc - 2], pipex.envp, pipex);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	i = 0;
	if (argc < 5)
		return (print_msg(ERROR1, ARGUMENTS));
	pipex.nb_cmd = argc - 4;
	pipex.pipe = malloc(sizeof(int) * (2 * (pipex.nb_cmd + 1)));
	pipex.envp = envp;
	while (i < pipex.nb_cmd + 1)
	{
		pipe(pipex.pipe + (i * 2));
		i++;
	}
	i = 0;
	while (i < pipex.nb_cmd)
	{
		childs(pipex, argv, i);
		i++;
	}
	last_one(argc, argv, pipex, pipex.pipe + 2 * (pipex.nb_cmd));
	close_all_fd(pipex);
	free(pipex.pipe);
	while (1)
	{
		if (waitpid(-1, NULL, 0) != -1)
			break ;
	}
}
