#!/bin/sh

###########################
#(C) Will Cunningham 2017 #
#         DK Lab          #
# Northeastern University #
###########################

mkdir -p m4
touch NEWS AUTHORS ChangeLog
autoreconf -vfi
