#!/bin/sh
#
# Copyright 2014-2022 Will Cunningham
#
# This file is part of FastMath.
#
# Licensed under the GNU General Public License 3.0 (the "License").
# A copy of the License may be obtained with this software package or at
#
#     https://www.gnu.org/licenses/gpl-3.0.en.html
#
# Use of this file is prohibited except in compliance with the License. Any
# modifications or derivative works of this file must retain this copyright
# notice, and modified files must contain a notice indicating that they have
# been altered from the originals.
#
# FastMath is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
# FITNESS FOR A PARTICULAR PURPOSE. See the License for more details.

set -eu -o pipefail

./general
echo -e '\n'
./clone
echo -e '\n'
./count
echo -e '\n'
./intersection
echo -e '\n'
./union
echo -e '\n'
./disjointunion
echo -e '\n'
./difference
echo -e '\n'
echo 'Completed all tests on FastBitset.'
