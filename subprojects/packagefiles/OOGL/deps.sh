#!/bin/sh

for i in src/GL/Util/libjpeg/*.c; do
    echo $i
done

for i in src/GL/Util/libpng/*.c; do
    echo $i
done

for i in src/GL/Util/zlib/*.c; do
    echo $i
done
