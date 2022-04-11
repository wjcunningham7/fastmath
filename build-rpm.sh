#!/bin/bash

set -eu -o pipefail

PKG_ROOT=`dirname $0`
VERSION=`cat $PKG_ROOT/VERSION`

BUILD=$HOME/rpmbuild
rm -rf $BUILD
mkdir -p $BUILD/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

tmpdir=$(mktemp -d)
mkdir $tmpdir/fastmath-$VERSION
cp -r $PKG_ROOT/{include,configure,LICENSE} $tmpdir
(cd $tmpdir && tar cvzf fastmath-$VERSION.tar.gz fastmath-$VERSION)
mv $tmpdir/fastmath-$VERSION.tar.gz $BUILD/SOURCES
rm -rf $tmpdir

. $PKG_ROOT/build-rpm-spec.sh
mv $PKG_ROOT/fastmath.spec $BUILD/SPECS

rpmbuild -bs $BUILD/SPECS/fastmath.spec
