
#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = ft_strlen(src);
	while (*src != '\0')
		*dest++ = *src++;
	*dest = '\0';
	return (dest - i);
}
