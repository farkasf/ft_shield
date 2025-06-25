#include "./inc/ft_shield.h"

int	main()
{
	if (geteuid())
		exit (EXIT_FAILURE);
	printf("ffarkas-edupless\n");
}
