#!/bin/bash
#SBATCH --partition=general-compute
#SBATCH --nodes=2
#SBATCH --tasks-per-node=2
#SBATCH --exclusive
#SBATCH --time=01:00:00
#SBATCH --job-name=naive_b
#SBATCH --mail-user=rbaingol@buffalo.edu
#SBATCH --output=nb.out
#SBATCH --error=nb.out

##################################
# This script is modified for CSE603 assignment 4. 
# Reference: http://chimpler.wordpress.com/2013/03/13/using-the-mahout-naive-bayes-classifier-to-automatically-classify-twitter-messages/
# by Ruhan Sa, Nov. 14, 2014.
##################################


echo "SLURM Environment Variables:"
echo "Job ID = "$SLURM_JOB_ID
echo "Job Name = "$SLURM_JOB_NAME
echo "Job Node List = "$SLURM_JOB_NODELIST
echo "Number of Nodes = "$SLURM_NNODES
echo "Tasks per node = "$SLURM_NTASKS_PER_NODE
echo "CPUs per task = "$SLURM_CPUS_PER_TASK
echo "/scratch/jobid = "$SLURMTMPDIR
echo "Submit Host = "$SLURM_SUBMIT_HOST
echo "Submit Directory = "$SLURM_SUBMIT_DIR
echo 
echo

#. $MODULESHOME/init/sh

#myhadoop is tool to help config and run hadoop.
module load myhadoop/0.2a/hadoop-0.20.1
module load mahout/0.8
module list
echo "MY_HADOOP_HOME="$MY_HADOOP_HOME
echo "HADOOP_HOME="$HADOOP_HOME
echo "MAHOUT_HOME="$MAHOUT_HOME

#### Set this to the directory where Hadoop configs should be generated
# Don't change the name of this variable (HADOOP_CONF_DIR) as it is
# required by Hadoop - all config files will be picked up from here
#
# Make sure that this is accessible to all nodes
export HADOOP_CONF_DIR=$SLURM_SUBMIT_DIR/config
echo "MyHadoop config directory="$HADOOP_CONF_DIR
### Set up the configuration
# Make sure number of nodes is the same as what you have requested from PBS
# usage: $MY_HADOOP_HOME/bin/pbs-configure.sh -h
echo "Set up the configurations for myHadoop"
# this is the non-persistent mode
export PBS_NODEFILE=nodelist.$$
srun --nodes=${SLURM_NNODES} bash -c 'hostname' | sort > $PBS_NODEFILE
NNuniq=`cat $PBS_NODEFILE | uniq | wc -l`
echo "Number of nodes in nodelist="$NNuniq
$MY_HADOOP_HOME/bin/pbs-configure.sh -n $NNuniq -c $HADOOP_CONF_DIR

sleep 5
# this is the persistent mode
# $MY_HADOOP_HOME/bin/pbs-configure.sh -n 4 -c $HADOOP_CONF_DIR -p -d /oasis/cloudstor-group/HDFS

#### Format HDFS, if this is the first time or not a persistent instance
echo
echo "Format HDFS"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR namenode -format

sleep 15

echo
echo "start dfs"
$HADOOP_HOME/bin/start-dfs.sh

echo
echo "start jobtracker (mapred)"
$HADOOP_HOME/bin/start-mapred.sh



sleep 15
echo
echo "------- make directory ----"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR dfs -mkdir /data
echo "------- copy training data to dfs---"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR dfs -put ../data /data
sleep 15
echo "------- list files in dfs---"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR dfs -ls /data

echo "-------Using mahout to transform the training data into vectors using tfidf weights ---"
##### this will generate files in HDFS in the directory /data/tweets-vectors#######
$MAHOUT_HOME/bin/mahout seq2sparse -i /data/data -o /data/tweets-vectors
echo "-------list directory tweets-vectors ---"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR dfs -ls /data/tweets-vectors

echo " ------Using mahout to splits the sets into two sets: traing and testing sets ---"
##### training set is to train the classifier and testing set is to test the classifier ######
$MAHOUT_HOME/bin/mahout split -i /data/tweets-vectors/tfidf-vectors --trainingOutput /data/train-vectors --testOutput /data/test-vectors --randomSelectionPct 40 --overwrite --sequenceFiles -xm sequential
echo "-------Train the classifier on training set---"
$MAHOUT_HOME/bin/mahout trainnb -i /data/train-vectors -el -li labelindex -o model -ow -c
echo "---------See what's in HDFS ------"
$HADOOP_HOME/bin/hadoop --config $HADOOP_CONF_DIR dfs -ls /
echo "-------To test the classifier is working properly on training set ---"
$MAHOUT_HOME/bin/mahout testnb -i /data/train-vectors -m model -l labelindex -ow -o /data/tweets-testing -c
echo "------- To test the classifier is working properly on testing set ---"
$MAHOUT_HOME/bin/mahout testnb -i /data/test-vectors -m model -l labelindex -ow -o tweets-testing -c
##### Check the above results to see the correctly classified instance ####

echo "stop jobtracker (mapred)"
$HADOOP_HOME/bin/stop-mapred.sh

echo "stop dfs"
$HADOOP_HOME/bin/stop-dfs.sh


#### Clean up the working directories after job completion
echo "Clean up"
$MY_HADOOP_HOME/bin/pbs-cleanup.sh -n $NNuniq
echo
