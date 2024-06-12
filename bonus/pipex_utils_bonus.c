/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 20:52:22 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/11 15:52:08 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	print_error(char *error)
{
	perror(error);
	exit(1);
}

int	print_msg(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n", 2);
	if (str)
		free(str);
	return (0);
}

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

void	set_pipex(t_pipex *pipex, int argc, char **argv)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->nb_cmd = argc - 4 - pipex->here_doc;
	pipex->pipe = malloc(sizeof(int) * (2 * (pipex->nb_cmd + 1)));
	if (!pipex->pipe)
		print_error("Malloc");
}
