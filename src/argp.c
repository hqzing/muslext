/* GNU argp-compatible command-line parser, clean-room implementation. */

#define _GNU_SOURCE 1

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <argp.h>

const char *argp_program_version;
void (*argp_program_version_hook) (FILE *, struct argp_state *);
const char *argp_program_bug_address;
error_t argp_err_exit_status = EXIT_FAILURE;

static const char *
program_name (char *argv0)
{
  const char *slash;

  if (argv0 == NULL)
    return "";
  slash = strrchr (argv0, '/');
  return slash != NULL ? slash + 1 : argv0;
}

static const struct argp_option *
find_long_option (const struct argp *argp, const char *name, int *owner)
{
  const struct argp *current = argp;

  while (current != NULL)
    {
      const struct argp_option *option;

      for (option = current->options; option != NULL && option->name != NULL;
           option++)
        if (strcmp (option->name, name) == 0)
          {
            if (owner != NULL)
              *owner = 1;
            return option;
          }
      current = NULL;
    }
  return NULL;
}

static const struct argp_option *
find_short_option (const struct argp *argp, int key)
{
  const struct argp_option *option;

  for (option = argp->options; option != NULL && option->name != NULL;
       option++)
    if (option->key == key)
      return option;
  return NULL;
}

static void
print_usage (FILE *stream, const char *name, const struct argp *argp,
             unsigned flags)
{
  fprintf (stream, "Usage: %s", name);
  if ((flags & ARGP_HELP_SHORT_USAGE) == 0)
    fputs (" [OPTION...]", stream);
  if (argp != NULL && argp->args_doc != NULL)
    fprintf (stream, " %s", argp->args_doc);
  fputc ('\n', stream);
}

static void
print_doc (FILE *stream, const char *doc, int before)
{
  const char *tab;
  const char *text;

  if (doc == NULL)
    return;
  tab = strchr (doc, '\v');
  text = before ? doc : (tab != NULL ? tab + 1 : NULL);
  if (text != NULL && *text != '\0')
    fprintf (stream, "\n%s\n", text);
}

static void
print_options (FILE *stream, const struct argp *argp)
{
  const struct argp_option *option;
  int last_group = -1;

  for (option = argp->options; option != NULL && option->name != NULL;
       option++)
    {
      if ((option->flags & (OPTION_HIDDEN | OPTION_DOC)) != 0)
        continue;
      if (option->group != last_group && option->doc != NULL)
        {
          fprintf (stream, "\n%s:\n", option->doc);
          last_group = option->group;
          continue;
        }
      fprintf (stream, "  ");
      if (option->key > 0 && option->key < 128)
        fprintf (stream, "-%c, ", option->key);
      else
        fputs ("    ", stream);
      fprintf (stream, "--%s", option->name);
      if (option->arg != NULL)
        fprintf (stream, "=%s", option->arg);
      if (option->doc != NULL)
        fprintf (stream, "\n          %s", option->doc);
      fputc ('\n', stream);
    }
}

static void
print_help (FILE *stream, const char *name, const struct argp *argp,
            unsigned flags)
{
  if ((flags & (ARGP_HELP_USAGE | ARGP_HELP_SHORT_USAGE)) != 0)
    print_usage (stream, name, argp, flags);
  if ((flags & ARGP_HELP_LONG) != 0 && argp != NULL)
    {
      if ((flags & ARGP_HELP_PRE_DOC) != 0)
        print_doc (stream, argp->doc, 1);
      print_options (stream, argp);
      if ((flags & ARGP_HELP_POST_DOC) != 0)
        print_doc (stream, argp->doc, 0);
    }
  if ((flags & ARGP_HELP_BUG_ADDR) != 0 && argp_program_bug_address != NULL)
    fprintf (stream, "\nReport bugs to %s.\n", argp_program_bug_address);
  fflush (stream);
}

static void
finish_help (unsigned flags)
{
  if ((flags & ARGP_HELP_EXIT_ERR) != 0)
    exit (argp_err_exit_status);
  if ((flags & ARGP_HELP_EXIT_OK) != 0)
    exit (0);
}

void
argp_help (const struct argp *argp, FILE *stream, unsigned flags, char *name)
{
  print_help (stream, name != NULL ? name : "", argp, flags);
  finish_help (flags);
}

void
argp_state_help (const struct argp_state *state, FILE *stream, unsigned flags)
{
  print_help (stream, state->name, state->root_argp, flags);
  if ((flags & ARGP_HELP_EXIT_ERR) != 0
      && (state->flags & ARGP_NO_EXIT) == 0)
    exit (argp_err_exit_status);
  if ((flags & ARGP_HELP_EXIT_OK) != 0
      && (state->flags & ARGP_NO_EXIT) == 0)
    exit (0);
}

static void
report (const struct argp_state *state, int status, int errnum,
        const char *format, va_list ap)
{
  FILE *stream = state != NULL && state->err_stream != NULL
                 ? state->err_stream : stderr;

  if (state != NULL && state->name != NULL)
    fprintf (stream, "%s: ", state->name);
  vfprintf (stream, format, ap);
  if (errnum != 0)
    fprintf (stream, ": %s", strerror (errnum));
  fputc ('\n', stream);
  fflush (stream);

  if (status != 0
      && (state == NULL || (state->flags & ARGP_NO_EXIT) == 0))
    exit (status);
}

void
argp_error (const struct argp_state *state, const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  report (state, argp_err_exit_status, 0, format, ap);
  va_end (ap);
}

void
argp_failure (const struct argp_state *state, int status, int errnum,
              const char *format, ...)
{
  va_list ap;

  va_start (ap, format);
  report (state, status, errnum, format, ap);
  va_end (ap);
}

static error_t
call_parser (const struct argp *argp, int key, char *arg,
             struct argp_state *state)
{
  if (argp == NULL || argp->parser == NULL)
    return ARGP_ERR_UNKNOWN;
  return argp->parser (key, arg, state);
}

error_t
argp_parse (const struct argp *argp, int argc, char **argv, unsigned flags,
            int *arg_index, void *input)
{
  struct argp_state state;
  error_t result = 0;
  int index;
  int no_args = 1;

  if (argp == NULL || argv == NULL)
    return EINVAL;

  memset (&state, 0, sizeof (state));
  state.root_argp = argp;
  state.argc = argc;
  state.argv = argv;
  state.next = (flags & ARGP_PARSE_ARGV0) != 0 ? 0 : 1;
  state.flags = flags;
  state.input = input;
  state.name = (char *) program_name (argv[0]);
  state.err_stream = stderr;
  state.out_stream = stdout;

  result = call_parser (argp, ARGP_KEY_INIT, NULL, &state);
  if (result != 0 && result != ARGP_ERR_UNKNOWN)
    goto done;

  index = state.next;
  while (index < argc)
    {
      char *argument = argv[index];

      if (strcmp (argument, "--") == 0)
        {
          index++;
          state.quoted = index;
          break;
        }

      if (argument[0] == '-' && argument[1] == '-' && argument[2] != '\0')
        {
          char *name = argument + 2;
          char *value = strchr (name, '=');
          const struct argp_option *option;
          char name_buffer[256];
          char *option_name = name;

          if (value != NULL)
            {
              size_t name_length = (size_t) (value - name);

              if (name_length >= sizeof (name_buffer))
                {
                  if ((flags & ARGP_NO_ERRS) == 0)
                    argp_error (&state, "option name too long");
                  result = EINVAL;
                  goto done;
                }
              memcpy (name_buffer, name, name_length);
              name_buffer[name_length] = '\0';
              option_name = name_buffer;
              value++;
            }

          if ((flags & ARGP_NO_HELP) == 0 && strcmp (option_name, "help") == 0)
            {
              argp_state_help (&state, stdout, ARGP_HELP_STD_HELP);
              index++;
              continue;
            }
          if ((flags & ARGP_NO_HELP) == 0 && strcmp (option_name, "usage") == 0)
            {
              argp_state_help (&state, stdout,
                               ARGP_HELP_STD_USAGE & ~ARGP_HELP_EXIT_ERR);
              index++;
              continue;
            }
          if ((flags & ARGP_NO_HELP) == 0
              && strcmp (option_name, "version") == 0)
            {
              if (argp_program_version_hook != NULL)
                argp_program_version_hook (stdout, &state);
              else if (argp_program_version != NULL)
                fprintf (stdout, "%s\n", argp_program_version);
              if ((flags & ARGP_NO_EXIT) == 0)
                exit (0);
              index++;
              continue;
            }

          option = find_long_option (argp, option_name, NULL);
          if (option == NULL)
            {
              if ((flags & ARGP_NO_ERRS) == 0)
                argp_error (&state, "unrecognized option '--%s'", option_name);
              result = EINVAL;
              goto done;
            }
          if ((option->flags & OPTION_DOC) != 0)
            {
              index++;
              continue;
            }
          if (value == NULL && option->arg != NULL
              && (option->flags & OPTION_ARG_OPTIONAL) == 0)
            {
              if (index + 1 >= argc)
                {
                  if ((flags & ARGP_NO_ERRS) == 0)
                    argp_error (&state, "option '--%s' requires an argument",
                                option_name);
                  result = EINVAL;
                  goto done;
                }
              value = argv[++index];
            }
          result = call_parser (argp, option->key, value, &state);
          if (result == ARGP_ERR_UNKNOWN)
            {
              result = EINVAL;
              goto done;
            }
          if (result != 0)
            goto done;
          index++;
          continue;
        }

      if (argument[0] == '-' && argument[1] != '\0')
        {
          char *p = argument + 1;

          while (*p != '\0')
            {
              int key = (unsigned char) *p;
              const struct argp_option *option = find_short_option (argp, key);
              char *value = NULL;

              if (option == NULL)
                {
                  if ((flags & ARGP_NO_ERRS) == 0)
                    argp_error (&state, "invalid option -- '%c'", key);
                  result = EINVAL;
                  goto done;
                }
              if (option->arg != NULL && (option->flags & OPTION_ARG_OPTIONAL) == 0)
                {
                  if (p[1] != '\0')
                    value = p + 1;
                  else if (index + 1 < argc)
                    value = argv[++index];
                  else
                    {
                      if ((flags & ARGP_NO_ERRS) == 0)
                        argp_error (&state, "option requires an argument -- '%c'",
                                    key);
                      result = EINVAL;
                      goto done;
                    }
                  result = call_parser (argp, key, value, &state);
                  break;
                }
              result = call_parser (argp, key, value, &state);
              if (result == ARGP_ERR_UNKNOWN)
                result = EINVAL;
              if (result != 0)
                goto done;
              p++;
            }
          index++;
          continue;
        }

      no_args = 0;
      result = call_parser (argp, ARGP_KEY_ARG, argument, &state);
      if (result == ARGP_ERR_UNKNOWN)
        {
          if (arg_index != NULL)
            *arg_index = index;
          goto done;
        }
      if (result != 0)
        goto done;
      state.arg_num++;
      index++;
    }

  if (arg_index != NULL)
    *arg_index = index;
  if (no_args != 0)
    {
      result = call_parser (argp, ARGP_KEY_NO_ARGS, NULL, &state);
      if (result != 0 && result != ARGP_ERR_UNKNOWN)
        goto done;
    }
  result = call_parser (argp, ARGP_KEY_END, NULL, &state);
  if (result != 0 && result != ARGP_ERR_UNKNOWN)
    goto done;
  result = call_parser (argp, ARGP_KEY_SUCCESS, NULL, &state);
  if (result != 0 && result != ARGP_ERR_UNKNOWN)
    goto done;
  result = 0;

done:
  if (result != 0)
    call_parser (argp, ARGP_KEY_ERROR, NULL, &state);
  call_parser (argp, ARGP_KEY_FINI, NULL, &state);
  return result;
}
