#!/bin/sh
# First argument ($1) = CMSSW release
# Remaining arguments (${@:2}) are passed to cmsRun

# send all output to stderr
exec 1>&2
source /cvmfs/cms.cern.ch/cmsset_default.sh
export PYTHONHOME=/cvmfs/cms.cern.ch/slc6_amd64_gcc472/external/python/2.6.4/
cmsrel "$1"
tar -xzf src.tar.gz -C "$1/src"
cd "$1/src"
cmsenv
scram b -j2 USER_CXXFLAGS='-g'
cd -

set -x

echo "Starting at `date`"
start=`date "+%s"`
which cmsRun
cmsRun "${@:2}"

cmsRun_rc=$?

echo "cmsRun exited with status $cmsRun_rc"

ls -ltr

# discard all output
#rm -f *.root

echo "Finished at `date`"
finish=`date "+%s"`
echo Total runtime: $(($finish - $start)) seconds

if [ "$cmsRun_rc" != "0" ]; then
  echo "FAILURE"
  exit 1
fi

echo "SUCCESS"
