NAME = philo

CC = gcc
ifndef DEBUG
	CFLAGS = -Wall -Wextra -Werror
else
	CFLAGS = -Wall -Wextra -g -fsanitize=thread
endif
COMPILE = $(CC) $(CFLAGS)

FILES =	main.c utils.c initialize.c simulation.c philosopher.c \
		forks.c timestamp.c is_dead.c checks.c

OBJS = $(FILES:.c=.o)

%.o : %.c philosophers.h
	@$(CC) $(CFLAGS) -c $<

all: $(NAME)

$(NAME): $(OBJS)
	@$(COMPILE) $(OBJS) -o $(NAME)
	@echo "Creating executable"

clean:
	@rm -rf $(OBJS)
	@echo "Removing object files"

fclean: clean
	@rm -f $(NAME)
	@echo "Removing executable file"

re: fclean | all

.PHONY: all clean fclean re