TARGET = ft_shield
CC = cc
FLAGS = -Wall -Wextra -Werror

DAEMON_SRC =	./daemon/main.c \
				./daemon/shield.c \
				./daemon/signal_handler.c \
				./daemon/lock_file.c \
				./daemon/daemon.c \
				./daemon/connection.c \
				./daemon/shell.c

DAEMON_OBJ = $(DAEMON_SRC:.c=.o)
DAEMON_HDR = ./inc/ft_daemon.h

INC = -Iinc

all: $(TARGET)

$(TARGET): $(DAEMON_OBJ)
	$(CC) $(DAEMON_OBJ) -o $(TARGET)

%.o: %.c $(DAEMON_HDR)
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

clean:
	rm -f $(DAEMON_OBJ) $(TARGET)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
