/* pthread_yield() is a deprecated alias for sched_yield().
   Kept in its own translation unit because some libc headers redirect the
   name at the assembler level. */

extern int sched_yield (void);

int
pthread_yield (void)
{
  return sched_yield ();
}
