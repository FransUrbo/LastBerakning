#!/bin/sh

DST=`mktemp -d`
VER=`grep 'define VERSION "' Version.h | sed -e 's@.* "@@' -e 's@"@@'`

cd FinalOutput/Release || exit 1
find -type d -name package | while read pkg; do find "$pkg" | cpio -vpd $DST > /dev/null 2>&1; done

cd $DST
find -type d -name package | while read dir; do cd "$DST/$dir/.."; mv package/* .; rmdir "package"; done

cd $DST
find -type f -name config.h | while read cfg; do rm "$cfg"; done
find -type f -print0 | sort | xargs -0 tar cjf ../LastBerakning-$VER.tar.bz2

echo "File: '/tmp/LastBerakning-$VER.tar.bz2'"
