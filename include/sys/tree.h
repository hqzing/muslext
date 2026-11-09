/* Red-black tree macros compatible with the BSD <sys/tree.h> API.
   Clean-room implementation from the standard red-black tree algorithm. */

#ifndef MUSLEXT_SYS_TREE_H
#define MUSLEXT_SYS_TREE_H 1

#include <stddef.h>

#define RB_HEAD(name, type) \
  struct name { struct type *rbh_root; }

#define RB_INITIALIZER(root) { NULL }

#define RB_INIT(root) do { (root)->rbh_root = NULL; } while (0)

#define RB_ENTRY(type) \
  struct { \
    struct type *rbe_left; \
    struct type *rbe_right; \
    struct type *rbe_parent; \
    int rbe_color; \
  }

#define RB_ROOT(head) ((head)->rbh_root)
#define RB_EMPTY(head) (RB_ROOT (head) == NULL)

#define RB_LEFT(elm, field) ((elm)->field.rbe_left)
#define RB_RIGHT(elm, field) ((elm)->field.rbe_right)
#define RB_PARENT(elm, field) ((elm)->field.rbe_parent)
#define RB_COLOR(elm, field) ((elm)->field.rbe_color)
#define RB_IS_RED(elm, field) ((elm) != NULL && RB_COLOR (elm, field) == RB_RED)
#define RB_IS_BLACK(elm, field) ((elm) == NULL || RB_COLOR (elm, field) == RB_BLACK)

#define RB_RED 0
#define RB_BLACK 1

#define RB_SET(elm, parent, color, field) do { \
  RB_PARENT (elm, field) = (parent); \
  RB_COLOR (elm, field) = (color); \
} while (0)

#define RB_SET_RED(elm, field) ((elm)->field.rbe_color = RB_RED)
#define RB_SET_BLACK(elm, field) ((elm)->field.rbe_color = RB_BLACK)

#define RB_SET_BLACKRED(black, red, field) do { \
  RB_COLOR (black, field) = RB_BLACK; \
  RB_COLOR (red, field) = RB_RED; \
} while (0)

#define RB_PROTOTYPE(name, type, field, cmp) \
  struct type *name##_RB_INSERT (struct name *, struct type *); \
  struct type *name##_RB_REMOVE (struct name *, struct type *); \
  struct type *name##_RB_FIND (struct name *, struct type *); \
  struct type *name##_RB_NFIND (struct name *, struct type *); \
  struct type *name##_RB_MIN (struct name *); \
  struct type *name##_RB_MAX (struct name *); \
  struct type *name##_RB_NEXT (struct type *); \
  struct type *name##_RB_PREV (struct type *);

#define RB_GENERATE(name, type, field, cmp) \
static void \
name##_RB_ROTATE_LEFT (struct name *head, struct type *x) \
{ \
  struct type *y = RB_RIGHT (x, field); \
  RB_RIGHT (x, field) = RB_LEFT (y, field); \
  if (RB_LEFT (y, field) != NULL) \
    RB_PARENT (RB_LEFT (y, field), field) = x; \
  RB_PARENT (y, field) = RB_PARENT (x, field); \
  if (RB_PARENT (x, field) == NULL) \
    head->rbh_root = y; \
  else if (x == RB_LEFT (RB_PARENT (x, field), field)) \
    RB_LEFT (RB_PARENT (x, field), field) = y; \
  else \
    RB_RIGHT (RB_PARENT (x, field), field) = y; \
  RB_LEFT (y, field) = x; \
  RB_PARENT (x, field) = y; \
} \
static void \
name##_RB_ROTATE_RIGHT (struct name *head, struct type *x) \
{ \
  struct type *y = RB_LEFT (x, field); \
  RB_LEFT (x, field) = RB_RIGHT (y, field); \
  if (RB_RIGHT (y, field) != NULL) \
    RB_PARENT (RB_RIGHT (y, field), field) = x; \
  RB_PARENT (y, field) = RB_PARENT (x, field); \
  if (RB_PARENT (x, field) == NULL) \
    head->rbh_root = y; \
  else if (x == RB_RIGHT (RB_PARENT (x, field), field)) \
    RB_RIGHT (RB_PARENT (x, field), field) = y; \
  else \
    RB_LEFT (RB_PARENT (x, field), field) = y; \
  RB_RIGHT (y, field) = x; \
  RB_PARENT (x, field) = y; \
} \
static void \
name##_RB_INSERT_FIXUP (struct name *head, struct type *z) \
{ \
  while (z != NULL && RB_PARENT (z, field) != NULL \
         && RB_COLOR (RB_PARENT (z, field), field) == RB_RED) \
    { \
      struct type *parent = RB_PARENT (z, field); \
      struct type *grand = RB_PARENT (parent, field); \
      if (parent == RB_LEFT (grand, field)) \
        { \
          struct type *uncle = RB_RIGHT (grand, field); \
          if (RB_IS_RED (uncle, field)) \
            { \
              RB_SET_BLACK (parent, field); \
              RB_SET_BLACK (uncle, field); \
              RB_SET_RED (grand, field); \
              z = grand; \
            } \
          else \
            { \
              if (z == RB_RIGHT (parent, field)) \
                { \
                  z = parent; \
                  name##_RB_ROTATE_LEFT (head, z); \
                  parent = RB_PARENT (z, field); \
                  grand = RB_PARENT (parent, field); \
                } \
              RB_SET_BLACK (parent, field); \
              RB_SET_RED (grand, field); \
              name##_RB_ROTATE_RIGHT (head, grand); \
            } \
        } \
      else \
        { \
          struct type *uncle = RB_LEFT (grand, field); \
          if (RB_IS_RED (uncle, field)) \
            { \
              RB_SET_BLACK (parent, field); \
              RB_SET_BLACK (uncle, field); \
              RB_SET_RED (grand, field); \
              z = grand; \
            } \
          else \
            { \
              if (z == RB_LEFT (parent, field)) \
                { \
                  z = parent; \
                  name##_RB_ROTATE_RIGHT (head, z); \
                  parent = RB_PARENT (z, field); \
                  grand = RB_PARENT (parent, field); \
                } \
              RB_SET_BLACK (parent, field); \
              RB_SET_RED (grand, field); \
              name##_RB_ROTATE_LEFT (head, grand); \
            } \
        } \
    } \
  if (head->rbh_root != NULL) \
    RB_SET_BLACK (head->rbh_root, field); \
} \
struct type * \
name##_RB_INSERT (struct name *head, struct type *elm) \
{ \
  struct type *parent = NULL; \
  struct type *current = head->rbh_root; \
  int compare = 0; \
  while (current != NULL) \
    { \
      parent = current; \
      compare = (cmp) (elm, current); \
      if (compare < 0) \
        current = RB_LEFT (current, field); \
      else if (compare > 0) \
        current = RB_RIGHT (current, field); \
      else \
        return current; \
    } \
  RB_SET (elm, parent, RB_RED, field); \
  RB_LEFT (elm, field) = NULL; \
  RB_RIGHT (elm, field) = NULL; \
  if (parent == NULL) \
    head->rbh_root = elm; \
  else if (compare < 0) \
    RB_LEFT (parent, field) = elm; \
  else \
    RB_RIGHT (parent, field) = elm; \
  name##_RB_INSERT_FIXUP (head, elm); \
  return NULL; \
} \
static void \
name##_RB_TRANSPLANT (struct name *head, struct type *u, struct type *v) \
{ \
  if (RB_PARENT (u, field) == NULL) \
    head->rbh_root = v; \
  else if (u == RB_LEFT (RB_PARENT (u, field), field)) \
    RB_LEFT (RB_PARENT (u, field), field) = v; \
  else \
    RB_RIGHT (RB_PARENT (u, field), field) = v; \
  if (v != NULL) \
    RB_PARENT (v, field) = RB_PARENT (u, field); \
} \
static void \
name##_RB_DELETE_FIXUP (struct name *head, struct type *x, struct type *parent) \
{ \
  while (x != head->rbh_root && RB_IS_BLACK (x, field)) \
    { \
      if (parent == NULL) \
        break; \
      if (x == RB_LEFT (parent, field)) \
        { \
          struct type *w = RB_RIGHT (parent, field); \
          if (RB_IS_RED (w, field)) \
            { \
              RB_SET_BLACK (w, field); \
              RB_SET_RED (parent, field); \
              name##_RB_ROTATE_LEFT (head, parent); \
              w = RB_RIGHT (parent, field); \
            } \
          if (RB_IS_BLACK (RB_LEFT (w, field), field) \
              && RB_IS_BLACK (RB_RIGHT (w, field), field)) \
            { \
              RB_SET_RED (w, field); \
              x = parent; \
              parent = RB_PARENT (x, field); \
            } \
          else \
            { \
              if (RB_IS_BLACK (RB_RIGHT (w, field), field)) \
                { \
                  RB_SET_BLACK (RB_LEFT (w, field), field); \
                  RB_SET_RED (w, field); \
                  name##_RB_ROTATE_RIGHT (head, w); \
                  w = RB_RIGHT (parent, field); \
                } \
              RB_COLOR (w, field) = RB_COLOR (parent, field); \
              RB_SET_BLACK (parent, field); \
              RB_SET_BLACK (RB_RIGHT (w, field), field); \
              name##_RB_ROTATE_LEFT (head, parent); \
              x = head->rbh_root; \
              parent = NULL; \
            } \
        } \
      else \
        { \
          struct type *w = RB_LEFT (parent, field); \
          if (RB_IS_RED (w, field)) \
            { \
              RB_SET_BLACK (w, field); \
              RB_SET_RED (parent, field); \
              name##_RB_ROTATE_RIGHT (head, parent); \
              w = RB_LEFT (parent, field); \
            } \
          if (RB_IS_BLACK (RB_RIGHT (w, field), field) \
              && RB_IS_BLACK (RB_LEFT (w, field), field)) \
            { \
              RB_SET_RED (w, field); \
              x = parent; \
              parent = RB_PARENT (x, field); \
            } \
          else \
            { \
              if (RB_IS_BLACK (RB_LEFT (w, field), field)) \
                { \
                  RB_SET_BLACK (RB_RIGHT (w, field), field); \
                  RB_SET_RED (w, field); \
                  name##_RB_ROTATE_LEFT (head, w); \
                  w = RB_LEFT (parent, field); \
                } \
              RB_COLOR (w, field) = RB_COLOR (parent, field); \
              RB_SET_BLACK (parent, field); \
              RB_SET_BLACK (RB_LEFT (w, field), field); \
              name##_RB_ROTATE_RIGHT (head, parent); \
              x = head->rbh_root; \
              parent = NULL; \
            } \
        } \
    } \
  if (x != NULL) \
    RB_SET_BLACK (x, field); \
} \
struct type * \
name##_RB_REMOVE (struct name *head, struct type *z) \
{ \
  struct type *y = z; \
  struct type *x = NULL; \
  struct type *xparent = NULL; \
  int ycolor = RB_COLOR (y, field); \
  if (RB_LEFT (z, field) == NULL) \
    { \
      x = RB_RIGHT (z, field); \
      xparent = RB_PARENT (z, field); \
      name##_RB_TRANSPLANT (head, z, RB_RIGHT (z, field)); \
    } \
  else if (RB_RIGHT (z, field) == NULL) \
    { \
      x = RB_LEFT (z, field); \
      xparent = RB_PARENT (z, field); \
      name##_RB_TRANSPLANT (head, z, RB_LEFT (z, field)); \
    } \
  else \
    { \
      y = RB_RIGHT (z, field); \
      while (RB_LEFT (y, field) != NULL) \
        y = RB_LEFT (y, field); \
      ycolor = RB_COLOR (y, field); \
      x = RB_RIGHT (y, field); \
      if (RB_PARENT (y, field) == z) \
        xparent = y; \
      else \
        { \
          xparent = RB_PARENT (y, field); \
          name##_RB_TRANSPLANT (head, y, RB_RIGHT (y, field)); \
          RB_RIGHT (y, field) = RB_RIGHT (z, field); \
          RB_PARENT (RB_RIGHT (y, field), field) = y; \
        } \
      name##_RB_TRANSPLANT (head, z, y); \
      RB_LEFT (y, field) = RB_LEFT (z, field); \
      RB_PARENT (RB_LEFT (y, field), field) = y; \
      RB_COLOR (y, field) = RB_COLOR (z, field); \
    } \
  if (ycolor == RB_BLACK) \
    name##_RB_DELETE_FIXUP (head, x, xparent); \
  return z; \
} \
struct type * \
name##_RB_FIND (struct name *head, struct type *elm) \
{ \
  struct type *current = head->rbh_root; \
  while (current != NULL) \
    { \
      int compare = (cmp) (elm, current); \
      if (compare < 0) \
        current = RB_LEFT (current, field); \
      else if (compare > 0) \
        current = RB_RIGHT (current, field); \
      else \
        return current; \
    } \
  return NULL; \
} \
struct type * \
name##_RB_NFIND (struct name *head, struct type *elm) \
{ \
  struct type *current = head->rbh_root; \
  struct type *result = NULL; \
  while (current != NULL) \
    { \
      int compare = (cmp) (elm, current); \
      if (compare <= 0) \
        { \
          result = current; \
          current = RB_LEFT (current, field); \
        } \
      else \
        current = RB_RIGHT (current, field); \
    } \
  return result; \
} \
struct type * \
name##_RB_MIN (struct name *head) \
{ \
  struct type *current = head->rbh_root; \
  if (current == NULL) \
    return NULL; \
  while (RB_LEFT (current, field) != NULL) \
    current = RB_LEFT (current, field); \
  return current; \
} \
struct type * \
name##_RB_MAX (struct name *head) \
{ \
  struct type *current = head->rbh_root; \
  if (current == NULL) \
    return NULL; \
  while (RB_RIGHT (current, field) != NULL) \
    current = RB_RIGHT (current, field); \
  return current; \
} \
struct type * \
name##_RB_NEXT (struct type *elm) \
{ \
  if (RB_RIGHT (elm, field) != NULL) \
    { \
      elm = RB_RIGHT (elm, field); \
      while (RB_LEFT (elm, field) != NULL) \
        elm = RB_LEFT (elm, field); \
      return elm; \
    } \
  while (RB_PARENT (elm, field) != NULL \
         && elm == RB_RIGHT (RB_PARENT (elm, field), field)) \
    elm = RB_PARENT (elm, field); \
  return RB_PARENT (elm, field); \
} \
struct type * \
name##_RB_PREV (struct type *elm) \
{ \
  if (RB_LEFT (elm, field) != NULL) \
    { \
      elm = RB_LEFT (elm, field); \
      while (RB_RIGHT (elm, field) != NULL) \
        elm = RB_RIGHT (elm, field); \
      return elm; \
    } \
  while (RB_PARENT (elm, field) != NULL \
         && elm == RB_LEFT (RB_PARENT (elm, field), field)) \
    elm = RB_PARENT (elm, field); \
  return RB_PARENT (elm, field); \
}

#define RB_INSERT(name, head, elm) name##_RB_INSERT ((head), (elm))
#define RB_REMOVE(name, head, elm) name##_RB_REMOVE ((head), (elm))
#define RB_FIND(name, head, elm) name##_RB_FIND ((head), (elm))
#define RB_NFIND(name, head, elm) name##_RB_NFIND ((head), (elm))
#define RB_MIN(name, head) name##_RB_MIN ((head))
#define RB_MAX(name, head) name##_RB_MAX ((head))
#define RB_NEXT(name, field, elm) name##_RB_NEXT ((elm))
#define RB_PREV(name, field, elm) name##_RB_PREV ((elm))

#define RB_FOREACH(var, name, head) \
  for ((var) = name##_RB_MIN (head); (var) != NULL; \
       (var) = name##_RB_NEXT (var))

#define RB_FOREACH_REVERSE(var, name, head) \
  for ((var) = name##_RB_MAX (head); (var) != NULL; \
       (var) = name##_RB_PREV (var))

#define RB_FOREACH_SAFE(var, name, head, next) \
  for ((var) = name##_RB_MIN (head); \
       (var) != NULL && ((next) = name##_RB_NEXT (var), 1); \
       (var) = (next))

#endif /* MUSLEXT_SYS_TREE_H */
