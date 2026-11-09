#include <stdio.h>
#include <stdlib.h>
#include <sys/tree.h>

struct node { int key; RB_ENTRY(node) link; };
static int node_cmp (struct node *a, struct node *b);
RB_HEAD(tree, node);
RB_PROTOTYPE(tree, node, link, node_cmp)
RB_GENERATE(tree, node, link, node_cmp)

static int
node_cmp (struct node *a, struct node *b)
{
  return (a->key > b->key) - (a->key < b->key);
}

static int
check_node (struct node *n, int min, int max, int *black_height)
{
  int left_height = 0, right_height = 0;

  if (n == NULL) { *black_height = 1; return 0; }
  if (n->key < min || n->key > max) return -1;
  if (n->link.rbe_color == RB_RED
      && ((n->link.rbe_left && n->link.rbe_left->link.rbe_color == RB_RED)
          || (n->link.rbe_right && n->link.rbe_right->link.rbe_color == RB_RED)))
    return -1;
  if (n->link.rbe_left != NULL && n->link.rbe_left->link.rbe_parent != n) return -1;
  if (n->link.rbe_right != NULL && n->link.rbe_right->link.rbe_parent != n) return -1;
  if (check_node (n->link.rbe_left, min, n->key - 1, &left_height) != 0) return -1;
  if (check_node (n->link.rbe_right, n->key + 1, max, &right_height) != 0) return -1;
  if (left_height != right_height) return -1;
  *black_height = left_height + (n->link.rbe_color == RB_BLACK);
  return 0;
}

int
main (void)
{
  struct tree root;
  struct node *n, *next;
  int i, bh;
  int present[2000] = {0};
  int count = 0;

  RB_INIT (&root);
  srand (1);
  for (i = 0; i < 20000; i++)
    {
      int key = rand () % 2000;
      struct node probe = { .key = key };
      n = RB_FIND (tree, &root, &probe);
      if (n != NULL)
        {
          RB_REMOVE (tree, &root, n);
          free (n);
          present[key] = 0;
          count--;
        }
      else
        {
          n = malloc (sizeof (*n));
          n->key = key;
          if (RB_INSERT (tree, &root, n) != NULL) { fprintf (stderr, "dup insert\n"); return 1; }
          present[key] = 1;
          count++;
        }
      if (i % 97 == 0)
        {
          int seen = 0;
          if (root.rbh_root && root.rbh_root->link.rbe_color != RB_BLACK) { fprintf (stderr, "root red at %d\n", i); return 2; }
          if (check_node (root.rbh_root, -1, 2000, &bh) != 0) { fprintf (stderr, "invariant at %d\n", i); return 3; }
          RB_FOREACH (n, tree, &root) seen++;
          if (seen != count) { fprintf (stderr, "count %d != %d at %d\n", seen, count, i); return 4; }
        }
    }
  {
    struct node *all[2000];
    int total = 0;
    RB_FOREACH_SAFE (n, tree, &root, next) { all[total++] = n; }
    for (i = 0; i < total; i++) free (all[i]);
  }
  puts ("ok");
  return 0;
}
