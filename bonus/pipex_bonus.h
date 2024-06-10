/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:57:23 by ahenault          #+#    #+#             */
/*   Updated: 2024/06/10 22:28:44 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <sys/wait.h>

# define ERROR1 "Invalid arguments. "
# define ARGUMENTS "Ex: ./pipex infile cmd ... cmd outfile"
# define HERE_DOC "Ex: ./pipex here_doc LIMITER cmd cmd file"

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	int		here_doc;
	int		nb_cmd;
	int		*pipe;
}			t_pipex;

void		is_a_here_doc(t_pipex *pipex, int argc, char **argv);
void		set_pipex(t_pipex *pipex, int argc, char **argv);
void		read_heredoc(t_pipex pipex, char *arg, int *pipe);
void		open_infile(t_pipex pipex, char *arg, int *pipe);
int			open_outfile(t_pipex pipex);
void		exec_cmd(t_pipex pipex, char *arg);
int			print_msg(char *s1, char *s2);
void		print_error(char *error);
void		free_all(char **tab);
void		close_all_fd(t_pipex pipex);

#endif