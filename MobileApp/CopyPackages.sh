#!/bin/sh

DST=`mktemp -d`
VER=`grep 'define VERSION "' Version.h | sed -e 's@.* "@@' -e 's@"@@'`

mkdir "$DST/LastBerakning-$VER"
DST="$DST/LastBerakning-$VER"

echo -n "Copying packages: "
cd FinalOutput/Release || exit 1
find -type d -name package | while read pkg; do find "$pkg" | cpio -vpd $DST > /dev/null 2>&1; done
echo "done."

echo -n "Cleaning up: "
cd $DST
find -type d -name package | while read dir; do cd "$DST/$dir/.."; mv package/* .; rmdir "package"; done

cd $DST
find -type f -name config.h -o -name '*.jad' | while read cfg; do rm "$cfg"; done
echo "done."

echo -n "Creating tarball: "
cd $DST/..
find -type f -print0 | sort | xargs -0 tar cjf /tmp/LastBerakning-$VER.tar.bz2
echo "done."

echo ; echo "File: '/tmp/LastBerakning-$VER.tar.bz2'"
