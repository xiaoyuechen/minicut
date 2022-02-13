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

#include "minicut-main.h"

MC_test (test_a)
{
  MC_assert (1);
  MC_assert (3 == 3);
}

MC_test (test_b)
{
  MC_assert (0);
  MC_assert (1);
  MC_assert (1 == 3);
}
