NAME=t
$(NAME): $(NAME).c
	gcc -std=c99 -Wall -Wextra -Werror $(NAME).c -lm -o $(NAME)
	rm -f *~
	./$(NAME)

run:
	./$(NAME)

clean:
	rm -f *~ $(NAME)                                   
