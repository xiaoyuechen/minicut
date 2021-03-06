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

#ifndef MINICUT_MAIN_H
#define MINICUT_MAIN_H

#include "minicut-mini.h"
#include <stdio.h>

static inline void
MC_print_banner (const char *prog)
{
  printf ("==================================================================="
          "===\n"
          "minicut is testing %s\n"
          "-------------------------------------------------------------------"
          "---\n",
          prog);
}

static inline void
MC_print_test_summary (MC_test_node *head)
{
  int total, pass, fail;
  MC_result (&total, &pass, &fail);
  printf ("-------------------------------------------------------------------"
          "---\n"
          "TOTAL %d PASS %d FAIL %d\n"
          "==================================================================="
          "===\n",
          total, pass, fail);
}

static inline void
MC_print_test_name (MC_test_node *test)
{
  printf ("%s ", test->name);
  fflush (stdout);
}

static inline void
MC_print_test_result (MC_test_node *test)
{
  const char *status = test->error_head ? "FAIL" : "PASS";
  printf ("%s\n", status);
  fflush (stdout);
  for (MC_error_node *error = test->error_head; error; error = error->next)
    {
      fprintf (stderr, "%s:%lu: error: assertion failed: %s\n", error->file,
               error->line, error->exp);
    }
}

int
main (int argc, char *argv[])
{
  MC_print_banner (argv[0]);
  MC_run (MC_print_test_name, MC_print_test_result);
  MC_print_test_summary (MC_test_head);
  int nfailed;
  MC_result (0, 0, &nfailed);
  MC_fini ();
  return nfailed != 0;
}

#endif
