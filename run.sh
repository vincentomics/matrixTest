#!/bin/bash


# load environment
source $HOME/apps/miniforge3/etc/profile.d/conda.sh
conda activate dev


# clean up
function clean_up() {
    if [ -f $1 ]; then
        rm -f $1
    fi
}

clean_up temp_*.txt


# run test
## C++
./matrix_profiling > log_cpp_mat.txt


## Python
python matrix_profiling_np.py > log_python_np.txt
python matrix_profiling_pd.py > log_python_pd.txt


## R
Rscript matrix_profiling_df.R > log_R_df.txt
Rscript matrix_profiling_mat.R > log_R_mat.txt

