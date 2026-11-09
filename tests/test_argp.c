#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <argp.h>

static int parsed_foo;
static char *parsed_arg;
static int parsed_positional;

static error_t
parser (int key, char *arg, struct argp_state *state)
{
  (void) state;
  switch (key)
    {
    case 'f':
      parsed_foo = 1;
      return 0;
    case 'n':
      parsed_arg = arg;
      return 0;
    case ARGP_KEY_ARG:
      parsed_positional++;
      return 0;
    default:
      return ARGP_ERR_UNKNOWN;
    }
}

int
main (void)
{
  static const struct argp_option options[] =
  {
    { "foo", 'f', NULL, 0, "foo option", 0 },
    { "name", 'n', "NAME", 0, "name option", 0 },
    { NULL, 0, NULL, 0, NULL, 0 }
  };
  struct argp argp = { options, parser, "[FILE...]", "test parser", NULL, NULL, NULL };
  char *argv[] = { "prog", "--foo", "--name=value", "file", NULL };
  int index = -1;

  assert (argp_parse (&argp, 4, argv, 0, &index, NULL) == 0);
  assert (parsed_foo == 1);
  assert (parsed_arg != NULL && strcmp (parsed_arg, "value") == 0);
  assert (parsed_positional == 1);
  assert (index == 4);
  return 0;
}
