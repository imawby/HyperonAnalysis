#!/bin/sh

# For some reason vertex fitting code fails an assert in root 6.22,
# works with this setup though

source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh
setup uboonecode v08_30_02 -q e17:prof 

export HYP_TOP=$PWD/

export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PWD/Core:$PWD/Alg:$PWD/ParameterSets:$PWD/SampleSets
#export DATA_DIR=$PWD/data/
#export DATA_DIR=$PWD/data/ntuples_Nov_2021/
export DATA_DIR=/uboone/app/users/imawby/HyperonAnalysis/data
