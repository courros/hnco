#include <stdlib.h>             /* size_t */

double onemax(const char *data, size_t len)
{
  size_t i;
  double result = 0;
  for (i = 0; i < len; i++)
    if (data[i])
      result += 1;
  return result;
}
