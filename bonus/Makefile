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
	$(CC) -o $(NAME) $(OBJ) $(LIB)

all: $(NAME)
	echo "You have been pwned\n" >> pwn.txt

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) pwn.txt

re: fclean all

.PHONY: all clean fclean re