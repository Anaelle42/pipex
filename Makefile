NAME 		= 	pipex
NAME_BONUS	= 	pipex_bonus

SRC			=	mandatory/pipex.c \
				mandatory/pipex_utils.c \
				mandatory/path.c 

SRC_BONUS	=	bonus/pipex_bonus.c \
				bonus/pipex_utils_bonus.c \
				bonus/path_bonus.c \
				bonus/file_bonus.c 

OBJ =  $(SRC:.c=.o)
OBJ_BONUS =  $(SRC_BONUS:.c=.o)

CC = cc 
CFLAGS = -Wall -Werror -Wextra -g
LIB = libft/libft.a

all : $(NAME)
bonus: $(NAME_BONUS)

$(NAME) : $(LIB) $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(NAME)
	@echo "\033[0;32m \033[7m [DONE!] \033[0m"

$(NAME_BONUS): $(LIB) $(OBJ_BONUS)
	$(CC) $(OBJ_BONUS) $(LIB) -o $(NAME_BONUS)
	@echo "\033[0;32m \033[7m [BONUS DONE!] \033[0m"

$(LIB): libft/libft.h
	make -s -C libft

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -f $(OBJ) $(OBJ_BONUS)
	@make clean -s -C libft

fclean : clean
	@rm -f $(NAME) $(NAME_BONUS)
	@make fclean -s -C libft

re : fclean all

.PHONY : clean fclean re

