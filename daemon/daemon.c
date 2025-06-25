#include "shield.h"

void	daemonize(t_shield *daemon)
{
	if (!daemon)
		return ;

	pid_t pid = fork();
	if (pid < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
	if (pid > 0)
		exit (EXIT_SUCCESS);
	if (setsid() < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}

	pid = fork();
	if (pid < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
	if (pid > 0)
		exit (EXIT_SUCCESS);

	if (chdir("/") < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}

	int fd = open("/dev/null", O_RDWR);
	if (fd < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	if (fd > 2)
		close(fd);
}

void	setup_socket(t_shield *daemon)
{
	if (!daemon)
		return ;

	daemon->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (daemon->listen_fd < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(daemon->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(daemon->listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
	if (listen(daemon->listen_fd, 5) < 0)
	{
		remove_lock_file(daemon);
		exit (EXIT_FAILURE);
	}
}
