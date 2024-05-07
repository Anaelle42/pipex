NAME = pipex

SRC = main.c

OBJ =  $(SRC:.c=.o)

CC = cc 
CFLAGS = -Wall -Werror -Wextra 
LIB = libft/libft.a

all : $(NAME)

$(NAME) : $(LIB) $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(NAME)
	@echo "	\033[0;33m \033[7m OK \033[0m"

$(LIB):
	make -s -C libft

.c.o :
	$(CC) $(CFLAGS) -I $(LIB) -c $< -o $@

clean :
	@rm -f $(OBJ)
	@make clean -s -C libft

fclean : clean
	@rm -f $(NAME)
	@make fclean -s -C libft

re : fclean all

.PHONY : clean fclean re

