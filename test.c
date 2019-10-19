#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int main()
{

	size_t n = 140000000000000;
	char *s = malloc(n);
	if (s == NULL)
		printf("HELLO\n");

	return (0);
}

// Allocated 33237332 Mb total

// RLIMIT_CORE :rlim_cur => 0, :rlim_max => 9223372036854775807
// RLIMIT_CPU :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_DATA :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_FSIZE :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_MEMLOCK :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_NOFILE :rlim_cur => 256, :rlim_max => 9223372036854775807
// RLIMIT_NPROC :rlim_cur => 709, :rlim_max => 709
// RLIMIT_RSS :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_STACK :rlim_cur => 8388608, :rlim_max => 67104768
// RLIMIT_AS :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_CORE :rlim_cur => 0, :rlim_max => 9223372036854775807
// RLIMIT_CPU :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_DATA :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_FSIZE :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_MEMLOCK :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_NOFILE :rlim_cur => 256, :rlim_max => 9223372036854775807
// RLIMIT_NPROC :rlim_cur => 709, :rlim_max => 709
// RLIMIT_RSS :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807
// RLIMIT_STACK :rlim_cur => 8388608, :rlim_max => 67104768
// RLIMIT_AS :rlim_cur => 9223372036854775807, :rlim_max => 9223372036854775807