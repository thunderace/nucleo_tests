/*
 * Copyright (c) 2015 Francesco Balducci
 *
 * This file is part of nucleo_tests.
 *
 *    nucleo_tests is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    nucleo_tests is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with nucleo_tests.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <unistd.h>
#include <time.h>
#include <limits.h>

unsigned sleep(unsigned seconds)
{
    struct timespec rqt;
    struct timespec rmt;

    if (seconds > INT_MAX)
    {
        seconds = INT_MAX;
    }
    rqt.tv_sec = seconds;
    rqt.tv_nsec = 0;

    (void)nanosleep(&rqt, &rmt);

    return (unsigned)rmt.tv_sec;
}

