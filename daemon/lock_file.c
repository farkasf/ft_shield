#include "shield.h"

int	create_lock_file(t_shield *daemon)
{
	if (!daemon)
		return (-1);

	daemon->lock_fd = open(LOCK_PATH, O_RDWR | O_CREAT, 0640);
	if (daemon->lock_fd < 0) {
		return (-1);
	}

	if (flock(daemon->lock_fd, LOCK_EX | LOCK_NB) < 0) {
		close(daemon->lock_fd);
		daemon->lock_fd = -1;
		return (-1);
	}
	return (0);
}

void	remove_lock_file(t_shield *daemon)
{
	if (!daemon || daemon->lock_fd == -1)
		return ;

	flock(daemon->lock_fd, LOCK_UN);
	close(daemon->lock_fd);
	unlink(LOCK_PATH);
	daemon->lock_fd = -1;
}
