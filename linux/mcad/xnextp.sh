#!/bin/sh

xpPid=$(cut -d' ' -f4 < /proc/self/stat)
xpPidSubshell=$(echo $$)

echo "xnextp start with ${xpPid}, ${xpPidSubshell}"

while [ 1 -eq 1 ]; do
	echo "xnextp restarting xmca"
	/bin/sh xmcad.sh
	sleep 1
done

echo "xnextp stop with ${xpPid}, ${xpPidSubshell}"
