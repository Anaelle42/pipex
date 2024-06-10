/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:57:23 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 19:51:55 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <unistd.h>

# define ERROR1 "Invalid arguments. "
# define ARGUMENTS "Ex: ./pipex infile cmd ... cmd outfile"
# define HERE_DOC "Ex: ./pipex here_doc LIMITER cmd cmd file"

typedef struct s_pipex
{
	int		nb_cmd;
	int		*pipe;
	char	**envp;
	int		here_doc;
}			t_pipex;

void		absolut_vodkapath(char **cmd, char **envp, t_pipex pipex);
void		cmd_path(char **cmd, char **envp, t_pipex pipex);
int			print_msg(char *s1, char *s2);
void		print_error(char *error);
void		free_all(char **tab);
void		close_all_fd(t_pipex pipex);

#endif