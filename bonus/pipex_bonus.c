/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 19:23:32 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 20:15:02 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	close(*pipe);
	*pipe = fd;
	return (0);
}
int	read_heredoc(t_pipex pipex, char **argv, int *pipe)
{
	char	*line;
	int		fd;

	fd = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (ft_strncmp(line, argv[2], ft_strlen(line) - 1) == 0
			&& ft_strlen(line + 1) == ft_strlen(argv[2]))
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	get_next_line(-1);
	// ft_putstr_fd("\ntadah ", 1);
	free(line);
	close(fd);
	fd = open("tmp", O_RDONLY);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error("here_doc_tmp");
	}
	close(*pipe);
	*pipe = fd;
	return (0);
}

int	childs(t_pipex pipex, char **argv, int index)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (index == 0 && pipex.here_doc == 0)
			open_infile(pipex, argv[1], pipex.pipe);
		else if (index == 0 && pipex.here_doc == 1)
			read_heredoc(pipex, argv, pipex.pipe);
		if (dup2(*(pipex.pipe + (index * 2)), 0) == -1 || (dup2(*(pipex.pipe
						+ (index * 2) + 3), 1) == -1))
		{
			close_all_fd(pipex);
			free(pipex.pipe);
			print_error(argv[index + 2]);
		}
		close_all_fd(pipex);
		exec_cmd(argv[index + 2 + pipex.here_doc], pipex.envp, pipex);
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
		if (pipex.here_doc == 0)
			fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		else
			fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
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
int	is_here_doc(t_pipex *pipex, char **argv, int argc)
{
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
		{
			print_msg(ERROR1, HERE_DOC);
			exit(1);
		}
		pipex->here_doc = 1;
	}
	else
		pipex->here_doc = 0;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		i;

	i = 0;
	if (argc < 5)
		return (print_msg(ERROR1, ARGUMENTS));
	pipex.envp = envp;
	is_here_doc(&pipex, argv, argc);
	// printf("%i\n", pipex.here_doc);
	pipex.nb_cmd = argc - 4 - pipex.here_doc;
	pipex.pipe = malloc(sizeof(int) * (2 * (pipex.nb_cmd + 1)));
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
		if (waitpid(-1, NULL, 0) == -1)
			break ;
	}
	if (pipex.here_doc == 1 && unlink("tmp") == -1)
		print_error("Unlink");
}
