#!/bin/bash

#SBATCH --partition=gpu
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=1
#SBATCH --gres=gpu:1
#SBATCH --job-name=_MPI_CUDA_VecAdd
#SBATCH --time=00:50:00
#SBATCH --mail-user=chaofeng@buffalo.edu
#SBATCH --output=Result_VecAdd_MPI_CUDA.out
#SBATCH --error=Result_VecAdd_MPI_CUDA.out

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

module load intel
module load intel-mpi
module load cuda
module list
ulimit -s unlimited

#
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so #used for cooperating MPI with slurm.
srun ./MPI_CUDA_PRIME -t 100
#
echo "All Dones!"