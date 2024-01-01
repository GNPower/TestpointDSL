#!/bin/bash

#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=2:00:00
#SBATCH --job-name project
#SBATCH --output=project_%j.log
#SBATCH --mail-type=FAIL

module load intel/2019u4
module load cmake/3.21.4
module load python/3.11.5

pip install matplotlib

./build.sh
./run.sh
