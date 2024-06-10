/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:23:32 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 22:31:24 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	childs(t_pipex pipex, int index)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		print_error("Fork");
	if (pid == 0)
	{
		if (index == 0 && pipex.here_doc == 0)
			open_infile(pipex, pipex.argv[1], pipex.pipe);
		else if (index == 0 && pipex.here_doc == 1)
			read_heredoc(pipex, pipex.argv[2], pipex.pipe);
		if (dup2(*(pipex.pipe + (index * 2)), 0) == -1 || (dup2(*(pipex.pipe
						+ (index * 2) + 3), 1) == -1))
		{
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(pipex.argv[index + 2]);
		}
		close_all_fd(pipex);
		exec_cmd(pipex, pipex.argv[index + 2 + pipex.here_doc]);
	}
	return (0);
}

int	last_child(t_pipex pipex, int *pipe)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid == -1)
		print_error("Fork");
	if (pid == 0)
	{
		fd = open_outfile(pipex);
		if (dup2(pipe[0], 0) == -1 || (dup2(fd, 1) == -1))
		{
			close(fd);
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(pipex.argv[pipex.argc - 2]);
		}
		close(fd);
		close_all_fd(pipex);
		exec_cmd(pipex, pipex.argv[pipex.argc - 2]);
	}
	return (0);
}

void	parent_process(t_pipex pipex)
{
	close_all_fd(pipex);
	free(pipex.pipe);
	while (1)
	{
		if (waitpid(-1, NULL, 0) == -1)
			break ;
	}
	if (pipex.here_doc == 1 && unlink("tmp") == -1)
		print_error("Unlink");
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	i = 0;
	if (argc < 5)
		return (print_msg(ERROR1, ARGUMENTS));
	pipex.envp = envp;
	is_a_here_doc(&pipex, argc, argv);
	set_pipex(&pipex, argc, argv);
	while (i < pipex.nb_cmd + 1)
	{
		if (pipe(pipex.pipe + (i * 2)) == -1)
			print_error("Pipe");
		i++;
	}
	i = 0;
	while (i < pipex.nb_cmd)
	{
		childs(pipex, i);
		i++;
	}
	last_child(pipex, pipex.pipe + 2 * (pipex.nb_cmd));
	parent_process(pipex);
	return (0);
}
