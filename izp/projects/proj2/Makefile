NAME=proj2
$(NAME): $(NAME).c
	gcc -std=c99 -Wall -Wextra -Werror $(NAME).c -lm -o $(NAME)

run:
	./$(NAME) 3.3 110 1e-9

clean:
	rm -f *~ $(NAME)
