#!/bin/bash

set -eu -o pipefail

PKG_ROOT=`dirname $0`/..
VERSION=`cat $PKG_ROOT/VERSION`

BUILD=$HOME/rpmbuild
rm -rf $BUILD
mkdir -p $BUILD/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

tmpdir=$(mktemp -d)
echo $tmpdir
mkdir $tmpdir/fastmath-$VERSION
cp -r $PKG_ROOT/{include,configure.ac,LICENSE,Makefile.am,VERSION} $tmpdir/fastmath-$VERSION
(cd $tmpdir && tar cvzf fastmath-$VERSION.tar.gz fastmath-$VERSION)
mv $tmpdir/fastmath-$VERSION.tar.gz $BUILD/SOURCES
rm -rf $tmpdir

. $PKG_ROOT/pkg/build-rpm-spec.sh
mv $PKG_ROOT/pkg/fastmath.spec $BUILD/SPECS

rpmbuild -ba $BUILD/SPECS/fastmath.spec
