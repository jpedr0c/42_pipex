#include "../../include/libft.h"

int main()
{
	char str[] = "'Cadetes','da',,,,,,,,,,42,,,,rio,,,,,,Cheetahs,,,,,,,,,,,,";
	char **str1 = ft_split(str, ',');
	int i;

	i = -1;
	while (str1[++i])
		ft_putstr_fd(str1[i], 1);
	return (0);
}