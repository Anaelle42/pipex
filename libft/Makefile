NAME	=	libft.a

SRCS	=	ft_isalpha.c	\
			ft_isdigit.c	\
			ft_isalnum.c	\
			ft_isascii.c	\
			ft_isprint.c	\
			ft_strlen.c	\
			ft_memset.c	\
			ft_bzero.c	\
			ft_memcpy.c	\
			ft_memmove.c	\
			ft_strlcpy.c	\
			ft_strlcat.c	\
			ft_toupper.c	\
			ft_tolower.c	\
			ft_strchr.c	\
			ft_strrchr.c	\
			ft_strncmp.c	\
			ft_memchr.c	\
			ft_memcmp.c	\
			ft_strnstr.c	\
			ft_atoi.c		\
			ft_calloc.c	\
			ft_strdup.c	\
			ft_substr.c	\
			ft_strjoin.c	\
			ft_strtrim.c	\
			ft_split.c	\
			ft_itoa.c		\
			ft_strmapi.c	\
			ft_striteri.c	\
			ft_putchar_fd.c	\
			ft_putstr_fd.c	\
			ft_putendl_fd.c	\
			ft_putnbr_fd.c	\
			get_next_line/get_next_line.c \
			get_next_line/get_next_line_utils.c \
			printf/ft_printf.c \
      		printf/ft_putnbr_hexa.c \
      		printf/tools.c \
      		printf/ft_printu.c

SRCS_BONUS =	ft_lstnew.c \
				ft_lstadd_front.c \
				ft_lstsize.c \
				ft_lstlast.c \
				ft_lstadd_back.c \
				ft_lstdelone.c \
				ft_lstclear.c \
				ft_lstiter.c \
				ft_lstmap.c \
				

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

CC		= cc

CFLAGS	=	-Wall -Werror -Wextra

.c.o :
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME)	: $(OBJS)
	@ar rcs $@ $^

all: $(NAME)

clean	:
	@$(RM) $(OBJS) $(OBJS_BONUS)

fclean	:	clean
	@$(RM) $(NAME) $(BONUS)			

re	:	fclean all

bonus: $(NAME) $(OBJS_BONUS)
	ar rcs $(NAME) -o $^

.PHONY	:	all clean fclean re bonus