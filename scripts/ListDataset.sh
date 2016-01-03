
#!/usr/bin/env bash

. $ROOTCOREDIR/user_scripts/RootUtils/shflags/src/shflags

DEFINE_string 'dataset'   "" "name of dataset" 'd'

FLAGS_HELP="USAGE: $0 [flags]"

FLAGS "$@" || exit 1
eval set -- "${FLAGS_ARGV}"

if [[ $FLAGS_dataset == "" ]]; then
   echo "Must enter a dataset name to search via --dataset."
else 
	dq2SearchString="${FLAGS_dataset}*"

	#echo "${dq2SearchString}"
	#dq2-ls "${dq2SearchString}" | sort -n

	for i in $(dq2-ls "${dq2SearchString}/" | sort -n); do
		dq2-ls -pfL NERSC_LOCALGROUPDISK $i | grep "SFN=" | grep ".root" | sed s_"SFN="_" "_g | awk '{print $NF}' | sort -t. -k4
	done	
fi



