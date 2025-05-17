#include "../../minishell.h"


void	syntax_error(char error)
{
	printf("\n Syntax Error !\n");
	if (error == '"')
		printf("The \" is not closed\n");
	if (error == '<')
		printf("< leads to nothing\n");
}

char *ft_strncpy(char *dest, const char *src, int n)
{
    int i = 0;

    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}

// int	ft_strcmp(const char *s1, const char *s2)
// {
// 	while (*s1 && *s2 && *s1 == *s2)
// 	{
// 		s1++;
// 		s2++;
// 	}
// 	return ((unsigned char)*s1 - (unsigned char)*s2);
// }

// char	*ft_strndup(char *input, int start, int end)
// {
// 	int	i;

// 	i = 0;
// 	char	*word;
// 	word = malloc((end - start + 1) * sizeof(char));
// 	while(start < end)
// 	{
// 		word[i] = input[start];
// 		i++;
// 		start++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = c;
	return (s);
}