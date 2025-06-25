#include "../inc/ft_daemon.h"

volatile sig_atomic_t g_signal = 0;

int	main(void)
{
	if (geteuid())
		exit (EXIT_FAILURE);
	
	t_shield daemon;

	if (shield_init(&daemon) != 0)
		return (EXIT_FAILURE);
	shield_run(&daemon);
	shield_destroy(&daemon);

	return (EXIT_SUCCESS);
}
