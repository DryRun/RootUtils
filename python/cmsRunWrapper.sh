#!/bin/bash
cd /uscms/home/dryu/
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd -
cd /uscms/home/dryu/Dijets/CMSSW_5_3_32_patch3
cmsenv
cd -
cmsRun "$@"
