/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:23:32 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/31 19:52:52 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//
// if (ft_strnstr(argv[1], "here_doc", 8))
// 	here_doc();
//

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

int	open_infile(char *argv, int *pipe)
{
	int	fd;

	fd = open(argv, O_RDONLY);
	if (fd == -1)
	{
		print_error(argv);
	}
	*pipe = fd;
	// close(fd);
	return (0);
}

void	close_all_fd(t_pipex pipex)
{
	int	i;

	i = 0;
	while (i < (2 * (pipex.nb_cmd + 1)))
	{
		close(pipex.pipe[i]);
		i++;
	}
}

int	childs(t_pipex pipex, char **argv, int i, int index, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (index == 0)
			open_infile(argv[1], pipex.pipe);
		if (dup2(*(pipex.pipe + i), 0) == -1 || (dup2(*(pipex.pipe + i + 3),
					1) == -1))
		{
			print_error(argv[index + 2]);
		}
		close_all_fd(pipex);
		exec_cmd(argv[index + 2], envp);
	}
	return (0);
}

int	last_one(int argc, char **argv, t_pipex pipex, int *pipe, char **envp)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == 0)
	{
		// close(pipe[1]);
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
		{
			// close(pipe[0]);
			print_error("argv[argc - 1]");
		}
		if (dup2(pipe[0], 0) == -1 || (dup2(fd, 1) == -1))
		{
			// close(pipe[0]);
			print_error("argv[argc - 2]");
		}
		close(fd);
		// close(pipe[0]);
		close_all_fd(pipex);
		exec_cmd(argv[argc - 2], envp);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;
	int		a;

	i = 0;
	a = 0;
	if (argc < 5)
		return (print_msg(ERROR1, ARGUMENTS));
	pipex.nb_cmd = argc - 4;
	pipex.pipe = malloc(sizeof(int) * (2 * (pipex.nb_cmd + 1)));
	while (i < pipex.nb_cmd + 1)
	{
		pipe(pipex.pipe + (i * 2));
		i++;
	}
	i = 0;
	while (i < pipex.nb_cmd)
	{
		childs(pipex, argv, a, i, envp);
		i++;
		a += 2;
	}
	last_one(argc, argv, pipex, pipex.pipe + i + 2, envp);
	close_all_fd(pipex);
	while (1)
	{
		if (waitpid(-1, NULL, 0) != -1)
			break ;
	}
}
