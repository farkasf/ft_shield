TARGET = ft_shield
DAEMON = ./daemon/shield_payload
PAYLOAD = ./inc/payload.h
CC = cc
FLAGS = -Wall -Wextra -Werror

DAEMON_SRC =	./daemon/main.c \
				./daemon/shield.c \
				./daemon/signal_handler.c \
				./daemon/lock_file.c \
				./daemon/daemon.c \
				./daemon/connection.c \
				./daemon/shell.c
TARGET_SRC =	ft_shield.c

DAEMON_OBJ = $(DAEMON_SRC:.c=.o)
TARGET_OBJ = $(TARGET_SRC:.c=.o)
DAEMON_HDR = ./inc/ft_daemon.h
TARGET_HDR = ./inc/ft_shield.h

INC = -Iinc

all: $(DAEMON) $(PAYLOAD) $(TARGET)

$(DAEMON): $(DAEMON_OBJ)
	$(CC) $(DAEMON_OBJ) -o $(DAEMON)

$(PAYLOAD): $(DAEMON)
	xxd -i $(DAEMON) > $(PAYLOAD)

$(TARGET): $(TARGET_OBJ)
	$(CC) $(TARGET_OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ $(INC)

clean:
	rm -f $(DAEMON_OBJ) $(TARGET_OBJ)

fclean: clean
	rm -f $(DAEMON) $(PAYLOAD) $(TARGET)

re: fclean all

.PHONY: all clean fclean re
