/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahenault <ahenault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:57:23 by ahenault          #+#    #+#             */
/*   Updated: 2024/05/24 18:38:20 by ahenault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <stdio.h>
# include <sys/wait.h>

# define ERROR1 "Invalid arguments : "
# define ARGUMENTS "./pipex infile cmd cmd outfile"

void	absolut_vodkapath(char **cmd, char **envp);
void	cmd_path(char **cmd, char **envp);
int		print_msg(char *s1, char *s2);
void	print_error(char *error);
void	free_all(char **tab);

#endif