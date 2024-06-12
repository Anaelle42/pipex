/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 20:34:00 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 22:28:37 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_infile(t_pipex pipex, char *arg, int *pipe)
{
	int	fd;

	fd = open(arg, O_RDONLY);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error(arg);
	}
	close(*pipe);
	*pipe = fd;
}

int	open_outfile(t_pipex pipex)
{
	int	fd;

	if (pipex.here_doc == 0)
		fd = open(pipex.argv[pipex.argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(pipex.argv[pipex.argc - 1], O_CREAT | O_RDWR | O_APPEND,
				0644);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error(pipex.argv[pipex.argc - 1]);
	}
	return (fd);
}

void	set_heredoc(t_pipex pipex, int *pipe)
{
	int	fd;

	fd = open("tmp", O_RDONLY);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error("here_doc_tmp");
	}
	close(*pipe);
	*pipe = fd;
}

void	read_heredoc(t_pipex pipex, char *arg, int *pipe)
{
	char	*line;
	int		fd;

	fd = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		close_all_fd(pipex);
		free(pipex.pipe);
		print_error("here_doc_tmp");
	}
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (ft_strncmp(line, arg, ft_strlen(line) - 1) == 0 && ft_strlen(line
				+ 1) == ft_strlen(arg))
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	get_next_line(-1);
	close(fd);
	set_heredoc(pipex, pipe);
}

void	is_a_here_doc(t_pipex *pipex, int argc, char **argv)
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
}
