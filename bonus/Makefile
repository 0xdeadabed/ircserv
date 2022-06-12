SHELL = /bin/sh

CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -I.
# DEBUG
CFLAGS += -g3 -fsanitize=address
CC     = c++ $(CFLAGS)

SRCS := $(shell find . -name '*.cpp')
OBJ := $(SRCS:%cpp=%o)

NAME   = marvin

.cpp.o:
	$(CC) -c $< -o $@

$(NAME): $(OBJ)
		echo "You have been pwned" > pwn.txt
		$(CC) -o $(NAME) $(OBJ) $(LIB)

all: $(NAME)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) pwn.txt

re: fclean all

.PHONY: all clean fclean re
