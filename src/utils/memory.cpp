# include "memory.hpp"

void	*ft_memset(void *b, int c, size_t len)
{
	while (len) {
		((unsigned char *)b)[len - 1] = (unsigned char) c;
		len--;
	}
	return (b);
}
