#!/bin/bash

#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --job-name=Rakesh_OpenMP
#SBATCH --time=00:50:00
#SBATCH --mail-user=rbaingol@buffalo.edu
#SBATCH --output=Rakesh_output_threads_1000000.out
#SBATCH --error=Error_Rakesh_threads_1000000.out

echo "SLURN Enviroment Variables:"
echo "Job ID = "$SLURM_JOB_ID
echo "Job Name = "$SLURM_JOB_NAME
echo "Job Node List = "$SLURM_JOB_NODELIST
echo "Number of Nodes = "$SLURM_NNODES
echo "Tasks per Nodes = "$SLURM_NTASKS_PER_NODE
echo "CPUs per task = "$SLURM_CPUS_PER_TASK
echo "/scratch/jobid = "$SLURMTMPDIR
echo "submit Host = "$SLURM_SUBMIT_HOST
echo "Subimt Directory = "$SLURM_SUBMIT_DIR
echo 


ulimit -s unlimited
module load intel
#
./OpenMP 1 -t 1000000 n

#
echo "All Dones!"
