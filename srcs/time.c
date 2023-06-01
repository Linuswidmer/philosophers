#include "philo.h"

long long current_timestamp_ms(void) 
{
  struct timeval te;
  long long milliseconds;
    
  gettimeofday(&te, NULL);
  // this can probably fail as well
  milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return (milliseconds);
}
