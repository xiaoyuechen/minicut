/*
 * minicut --- MINImalist C Unit Test
 * Copyright (C) 2022  Xiaoyue Chen
 *
 * This file is part of minicut.
 *
 * minicut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * minicut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with minicut.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MINICUT_MINI_H
#define MINICUT_MINI_H

#include <stdlib.h>

typedef struct MC_error_node
{
  const char *file;
  unsigned long line;
  const char *exp;
  struct MC_error_node *next;
} MC_error_node;

typedef struct MC_test_node
{
  const char *name;
  void (*fn) (struct MC_test_node *);
  MC_error_node *error_head;
  struct MC_test_node *next;
} MC_test_node;

static MC_test_node *MC_test_head;

static inline void
__MC_add_test (const char *name, void (*fn) (MC_test_node *))
{
  MC_test_node *node = (MC_test_node *)malloc (sizeof (*node));
  node->name = name;
  node->fn = fn;
  node->error_head = 0;
  node->next = 0;
  MC_test_node **last = &MC_test_head;
  for (; *last; last = &(*last)->next)
    ;
  *last = node;
}

#define MC_test(NAME)                                                         \
  static inline void __MC_test_##NAME (MC_test_node *);                       \
  static inline void __attribute__ ((constructor))                            \
  __MC_construct_##NAME (void)                                                \
  {                                                                           \
    __MC_add_test (#NAME, __MC_test_##NAME);                                  \
  }                                                                           \
  static inline void __MC_test_##NAME (MC_test_node *__test)

#define MC_assert(EXP)                                                        \
  do                                                                          \
    {                                                                         \
      if (!(EXP))                                                             \
        {                                                                     \
          MC_error_node *error = (MC_error_node *)malloc (sizeof (*error));   \
          error->file = __FILE__;                                             \
          error->line = __LINE__ + 0UL;                                       \
          error->exp = #EXP;                                                  \
          error->next = 0;                                                    \
          MC_error_node **last = &__test->error_head;                         \
          for (; *last; last = &(*last)->next)                                \
            ;                                                                 \
          *last = error;                                                      \
        }                                                                     \
    }                                                                         \
  while (0)

static inline void
MC_run (void (*pre_test_hook) (MC_test_node *),
        void (*post_test_hook) (MC_test_node *))
{
  for (MC_test_node *test = MC_test_head; test; test = test->next)
    {
      if (pre_test_hook)
        pre_test_hook (test);
      test->fn (test);
      if (post_test_hook)
        post_test_hook (test);
    }
}

#define __MC_safe_assign(PTR, EXP)                                            \
  do                                                                          \
    {                                                                         \
      if (PTR)                                                                \
        *PTR = (EXP);                                                         \
    }                                                                         \
  while (0)

static inline void
MC_result (int *total, int *pass, int *fail)
{
  __MC_safe_assign (total, 0);
  __MC_safe_assign (pass, 0);
  __MC_safe_assign (fail, 0);
  for (MC_test_node *test = MC_test_head; test; test = test->next)
    {
      __MC_safe_assign (total, *total + 1);
      __MC_safe_assign (pass, *pass + (test->error_head == 0));
      __MC_safe_assign (fail, *fail + (test->error_head != 0));
    }
}

static inline void
MC_fini ()
{
  for (MC_test_node *t = MC_test_head; t;)
    {
      for (MC_error_node *e = t->error_head; e;)
        {
          MC_error_node *ne = e->next;
          free (e);
          e = ne;
        }
      MC_test_node *nt = t->next;
      free (nt);
      t = nt;
    }
}

#endif
