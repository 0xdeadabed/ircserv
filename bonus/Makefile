NAME = marvin

all: $(NAME)

$(NAME):
	g++ main.cpp bot.cpp -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all

run:
	./$(NAME) 127.0.0.1 6697 1234 #ft