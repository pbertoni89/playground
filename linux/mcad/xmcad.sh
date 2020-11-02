#!/bin/sh

# $$ is defined to return the process ID of the parent in a subshell;
# from the man page under "Special Parameters":
#    $ Expands to the process ID of the shell. In a () subshell, it expands
#    to the process ID of the current shell, not the subshell.


xmPid=$(cut -d' ' -f4 < /proc/self/stat)
xmPidSubshell=$(echo $$)
xmPpid=$(echo ${BASHPID})  # won't work in every shell !!!


xnext_abort() {
	nProcs=1
	while [ ${nProcs} -gt 0 ]; do
		echo "[xmcad] issuing abort (${nProcs})"
		ps -aux | grep niproducer                               | awk '{print $2}' | xargs kill -9  2> /dev/null

		sleep 2
		nProcs=`ps -aux | grep niproducer | grep -v grep | wc -l`
	done
}

xnext_halt() {
	nProcs=2
	while [ ${nProcs} -gt 1 ]; do  # dunno where that +1 comes from...
		echo "[xmcad] issuing halt (${nProcs}), excluding ${xmPid}, subshell ${xmPidSubshell}"
		ps -aux | grep xmcad  | grep -v grep
		echo "after filtering"
		ps -aux | grep xmcad | grep -v grep | grep -v ${xmPid} | grep -v ${xmPidSubshell}

		ps -aux | grep xnextp | grep -v grep                                               | awk '{print $2}' | xargs kill -9  2> /dev/null
		ps -aux | grep xmcad  | grep -v grep | grep -v ${xmPid} | grep -v ${xmPidSubshell} | awk '{print $2}' | xargs kill -9  2> /dev/null

		sleep 2
		nProcs=`ps -aux | grep xmcad | grep -v grep | grep -v ${xmPid} | grep -v ${xmPidSubshell} | wc -l`
		echo "found ${nProcs} after filtering"
		ps -aux | grep xmcad | grep -v grep | grep -v ${xmPid} | grep -v ${xmPidSubshell}
	done
	xnext_abort
}



while getopts ":ao" o; do
	case "${o}" in
		a)
			xnext_abort
			;;
		o)
			xnext_halt
			;;
	esac
done
shift $((OPTIND-1))


while [ 1 -eq 1 ]; do
	echo "xm pid ${xmPid}, subshell ${xmPidSubshell}, ppid ${xmPpid}"
	/bin/sh niproducer.sh
done
