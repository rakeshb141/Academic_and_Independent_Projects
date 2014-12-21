1. Run the script parse_original.py

python parse_original.py 

Run this on the data set and this will create a file out.csv.

2. Run the script update_extreme.py

python update_extreme.py > final.csv

This will take input as out.csv and output a final.csv file which is the data set to be processed.

3. Modify line 86 of slurm.sh file to mention the path of file and submit job to ccr

sbatch slurm.sh
