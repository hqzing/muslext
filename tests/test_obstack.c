#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <obstack.h>

#define obstack_chunk_alloc malloc
#define obstack_chunk_free free

int
main (void)
{
  struct obstack ob;
  char *first;
  int *number;
  char *large;
  size_t i;

  obstack_init (&ob);
  obstack_grow (&ob, "hello", 5);
  assert (obstack_printf (&ob, "-%d", 42) == 3);
  obstack_1grow (&ob, '\0');
  first = obstack_finish (&ob);
  assert (strcmp (first, "hello-42") == 0);
  assert (obstack_object_size (&ob) == 0);

  obstack_int_grow (&ob, 123);
  number = obstack_finish (&ob);
  assert (*number == 123);

  for (i = 0; i < 10000; i++)
    obstack_1grow (&ob, 'a' + (i % 26));
  obstack_1grow (&ob, '\0');
  large = obstack_finish (&ob);
  assert (strlen (large) == 10000);
  assert (large[0] == 'a');

  obstack_blank (&ob, 16);
  obstack_free (&ob, NULL);
  return 0;
}
