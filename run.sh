#!/bin/sh


#------------------------------------------
#--- Setting which you may need to change -----
#------------------------------------------

# Prepare EOS.
source /etc/profile.d/eos_aliases.sh      # In case of LPC

# Input file directory. 
indir=/store/user/satoshi/PUPPI80xTuning/QCD

# Prefix of output root file. 
prefixoutputrootfile="QCD"

# Output dir. Relative path with respect to this script.
outputdir_relativepath=output

executable=main

# Number of maximum input files
NMAXFILES=1

#--------------------------
#----- Setting end --------
#--------------------------



SCRIPT_DIR=$(cd $(dirname ${BASH_SOURCE:-$0}); pwd)

exeFullpath=${SCRIPT_DIR}/${executable}

outputdir=${SCRIPT_DIR}/${outputdir_relativepath}/
mkdir -p ${outputdir}


# --------
# Preparation of input files is done
#
inputs=""
for f in ` eosls ${indir} | grep ".root" | head -n ${NMAXFILES}`
do
echo "input : ${indir}/$f"
inputs="${inputs} -i root://cmsxrootd.fnal.gov/${indir}/${f}"
done
#
# Preparation of input files is done
#---------

for algo in AK8  
#for algo in AK8 CA10 AK4 
do
#for pu in PF PFSK PFCHS PFPUPPI
for pu in PF 
do

TDirectory=${algo}${pu}

outputfullpath="${outputdir}/${prefixoutputrootfile}_${TDirectory}.root"
outputoption=" -o ${outputfullpath}"
echo "debug : output option = $outputoption"

eval $exeFullpath ${inputs} ${outputoption}  -j ${algo} -p $pu 

done
# end PU loop PF/SK/CHS/PUPPI.

done
# end "algo" loop
