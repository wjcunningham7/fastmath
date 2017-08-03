#!/bin/sh

###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

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
