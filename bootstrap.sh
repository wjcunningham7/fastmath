#!/bin/sh
#
# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the MIT License (the "License"). A copy of the
# License may be obtained with this software package or at
#
#     https://opensource.org/licenses/MIT
#
# FastMath is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

mkdir -p m4
touch NEWS AUTHORS ChangeLog
autoreconf -vfi
