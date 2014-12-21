#!/bin/bash

#SBATCH --partition=debug
#SBATCH --nodes=3
#SBATCH --ntasks-per-node=4
#SBATCH --job-name=MP_MPI
#SBATCH --time=00:50:00
#SBATCH --mail-user=rbaingol@buffalo.edu
#SBATCH --output=Result_MP_MPI_nodes_3_threads_5.out
#SBATCH --error=Result_MP_MPI_nodes_3_threads_5.out

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
module list
ulimit -s unlimited

#
export I_MPI_PMI_LIBRARY=/usr/lib64/libpmi.so #used for cooperating MPI with slurm.
srun ./plus 5 100000000
#
echo "All Dones!"
