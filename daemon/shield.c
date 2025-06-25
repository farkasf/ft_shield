#include "shield.h"

int	shield_init(t_shield *daemon)
{
	if (!daemon)
		return (-1);

	daemon->lock_fd = -1;
	daemon->listen_fd = -1;
	daemon->client_count = 0;
	daemon->stop_flag = 0;
	daemon->bytes_in = 0;
	daemon->bytes_out = 0;

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		daemon->client_fds[i] = -1;
		daemon->client_authenticated[i] = 0;
	}
	return (0);
}

void	shield_destroy(t_shield *daemon)
{
	if (!daemon)
		return ;

	if (daemon->listen_fd != -1)
		close(daemon->listen_fd);

	close_all_clients(daemon);
	remove_lock_file(daemon);
}

void	shield_run(t_shield *daemon)
{
	if (!daemon)
		return ;
		
	if (create_lock_file(daemon))
		exit (EXIT_FAILURE);

	daemonize(daemon);
	setup_signal_handlers();

	setup_socket(daemon);
	allow_connections(daemon);
}

static unsigned int	ft_hash(const char *str)
{
	if (!str)
		return (0);

	unsigned int hash = 4242;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 2) + hash) + c;
	}
	return (hash);
}

int	verify_pass(const char *pass)
{
	if (!pass)
		return (0);

	unsigned int input = ft_hash(pass);

	return (input == MASTER);
}
