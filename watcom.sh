#!/bin/sh
set -e
mkdir -p obj
cd obj
SRC=`find ../src -name "*.c" -and -not -name "fileio.c" -and -not -name "config_read.c"`
for i in $SRC; do
	NAME=`echo $i | rev | cut -d"/" -f1 | rev`.hash
	HASH=`md5sum $i | cut -d" " -f1`
	if [ -f "$NAME" ]; then
		if [ "`cat $NAME`" = "$HASH" ]; then
			continue
		fi
	fi
	owcc -c -bnt -I$WATCOM/h -I$WATCOM/h/nt -I../include -I../include/common -I../include/server $i
	echo "CC $i"
	echo $HASH > $NAME
done
echo "LD ../ircd.exe"
owcc -bnt -o ../ircd.exe *.o *.obj wsock32.lib dnsapi.lib
