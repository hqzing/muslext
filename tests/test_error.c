#include <assert.h>
#include <errno.h>

#include <error.h>

int
main (void)
{
  error_message_count = 0;
  error_one_per_line = 0;

  error (0, 0, "plain %d", 1);
  assert (error_message_count == 1);

  error_at_line (0, ENOENT, "file.c", 10, "with errno");
  assert (error_message_count == 2);

  error_one_per_line = 1;
  error_at_line (0, 0, "file.c", 10, "again");
  assert (error_message_count == 2);

  error_at_line (0, 0, "file.c", 11, "new line");
  assert (error_message_count == 3);
  return 0;
}
