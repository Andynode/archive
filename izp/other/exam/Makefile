NAME=exam
$(NAME): $(NAME).c
	gcc -std=c99 -Wall -Wextra $(NAME).c -o $(NAME)

strict:
	gcc -std=c99 -Wall -Wextra -Werror $(NAME).c -o $(NAME)

run:
	./$(NAME)

clean:
	rm -f *~ $(NAME)
