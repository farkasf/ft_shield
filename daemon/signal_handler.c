#include "ft_shield.h"

void	signal_handler(int sig)
{
	g_signal = sig;
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGTERM, &sa, NULL); // kill -15
	sigaction(SIGINT, &sa, NULL); // kill -2 (Ctrl+C)
	sigaction(SIGHUP, &sa, NULL); // kill -1 (terminal disconnect)
	sigaction(SIGQUIT, &sa, NULL); // kill -3
	sigaction(SIGUSR1, &sa, NULL); // kill -10
	sigaction(SIGUSR2, &sa, NULL); // kill -12
	sigaction(SIGABRT, &sa, NULL); // kill -6
	sigaction(SIGALRM, &sa, NULL); // kill -14

	struct sigaction sa_ignore;
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGPIPE, &sa_ignore, NULL);
}
