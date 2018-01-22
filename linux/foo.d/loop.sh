#!/bin/bash

#
#	"xspectra_gui_bridge -n 127.0.0.1 -p 8888" "-c default.json"
#	"ni_gui_bridge 127.0.0.1 8888" "default.json"
#	"ni_gui_controller 127.0.0.1 8888" "default.json"
#			"ni_frame_builder" "default.json"
#

nLoop=0
programName=$1
programArg=$2
programSignature=$1" "$2

echo "< < < < < < < < < START of "${programSignature}" loop > > > > > > > > >"


while :
do
	let nLoop=nLoop+1
	echo "< < < < < < < < < [re]starting "${programSignature}" > > > > > > > > >" ${nLoop}
	"./"${programName} ${programArg} # NOOOO BACKGROUND !!! &
	sleep 1
done


echo "< < < < < < < < < END of "${programSignature}" loop > > > > > > > > >"
