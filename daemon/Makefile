TARGET = ft_shield
CC = cc
FLAGS = -Wall -Wextra -Werror
SRC = main.c ft_shield.c signal_handler.c lock_file.c daemon.c connection.c shell.c
OBJ = $(SRC:.c=.o)
INC = -I.
HDR = ft_shield.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c $(HDR)
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

clean:
	rm -f $(OBJ) $(TARGET)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
