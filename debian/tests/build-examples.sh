#!/bin/bash

for f in $(ls samples/*.c); do
	name=$(basename $f .c)
	flags=$(pkg-config --libs libtraceevent)
	cflags=$(pkg-config --cflags libtraceevent)

	cc -o $AUTOPKGTEST_TMP/$name $f $flags $cflags
	echo "[I] Built example $f"
done
