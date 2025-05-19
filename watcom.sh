#!/bin/sh
set -e
mkdir -p obj/server
cd obj/server
SRC=`find ../../src/server ../../src/common -name "*.c" -and -not -name "fileio.c" -and -not -name "config_read.c"`
for i in $SRC; do
	NAME=`echo $i | rev | cut -d"/" -f1 | rev`.hash
	if which md5 >/dev/null 2>&1; then
		HASH=`md5 $i | rev | cut -d" " -f1 | rev`
	else
		HASH=`md5sum $i | cut -d" " -f1`
	fi
	if [ -f "$NAME" ]; then
		if [ "`cat $NAME`" = "$HASH" ]; then
			continue
		fi
	fi
	owcc -c -bnt -I$WATCOM/h -I$WATCOM/h/nt -I../../include -I../../include/common -I../../include/server $i
	echo "CC $i"
	echo $HASH > $NAME
done
echo "LD ../../ircd.exe"
owcc -bnt -o ../../ircd.exe *.o *.obj wsock32.lib dnsapi.lib
cd ../..

mkdir -p obj/service
cd obj/service
SRC=`find ../../src/service -name "*.c"`
for i in $SRC; do
	NAME=`echo $i | rev | cut -d"/" -f1 | rev`.hash
	if which md5 >/dev/null 2>&1; then
		HASH=`md5 $i | rev | cut -d" " -f1 | rev`
	else
		HASH=`md5sum $i | cut -d" " -f1`
	fi
	if [ -f "$NAME" ]; then
		if [ "`cat $NAME`" = "$HASH" ]; then
			continue
		fi
	fi
	owcc -c -bnt -I$WATCOM/h -I$WATCOM/h/nt -I../../include -I../../include/common -I../../include/service $i
	echo "CC $i"
	echo $HASH > $NAME
done
echo "LD ../../service.exe"
owcc -bnt -o ../../service.exe *.o *.obj wsock32.lib
cd ../..
