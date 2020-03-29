#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BSIZE 8192

int main(void)
{
	char b[BSIZE + 1];
	ssize_t s;
	if ((s = read(0, b, BSIZE)) < 0)
		return (-1);
	b[s] = '\0';
	if (s > 0 && b[s - 1] == '\n')
		b[--s] = '\0';
	size_t len = (size_t)s;
	int found;
	for (size_t n = 0; n < len / 2; ++n)
		if (!strncmp(b, b + n, n))
		{
			found = 1;
			for (size_t i = 2 * n; i < len; i += n)
				if (strncmp(b, b + i, (i + n < len ? n : len - i)))
				{
					found = 0;
					break ;
				}
			if (found)
			{
				printf("cycle size: %lu\n%.*s\n", n, (int)n, b);
				return (0);
			}
		}
	fprintf(stderr, "error: no cycle found!\n");
	return (0);
}
