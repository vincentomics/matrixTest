Performance benchmarks comparing matrix operations using row-major vs column-major orientation. Tests evaluate common operations (mean, standard deviation, sum, transpose, reshape) on small and large matrices in different programming languages.


# Python
## NumPy
Check code in `matrix_profiling_np.ipynb`

### Small Matrix Test (10,000 × 500)
```
Matrix shapes:
- matrix1_row: 10000 × 500
- matrix1_col: 500 × 10000

=== Multiple Operations Profiling ===
matrix1_row std: 0.028367 ± 0.001116 seconds (mean ± std over 1000 runs)
matrix1_col std: 0.027170 ± 0.000864 seconds (mean ± std over 1000 runs)
matrix1_row mean: 0.005000 ± 0.000226 seconds (mean ± std over 1000 runs)
matrix1_col mean: 0.004629 ± 0.000226 seconds (mean ± std over 1000 runs)
matrix1_row sum: 0.002201 ± 0.000164 seconds (mean ± std over 1000 runs)
matrix1_col sum: 0.002470 ± 0.000210 seconds (mean ± std over 1000 runs)
matrix1_row transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
matrix1_col transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
matrix1_row reshape: 0.000000 ± 0.000001 seconds (mean ± std over 1000 runs)
matrix1_col reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
```

### Large Matrix Test (100,000 × 500)
```
Matrix shapes:
- matrix2_row: 100000 × 500
- matrix2_col: 500 × 100000

=== Multiple Operations Profiling ===
matrix2_row std: 0.348997 ± 0.082030 seconds (mean ± std over 1000 runs)
matrix2_col std: 0.336844 ± 0.025020 seconds (mean ± std over 1000 runs)
matrix2_row mean: 0.058866 ± 0.001837 seconds (mean ± std over 1000 runs)
matrix2_col mean: 0.065929 ± 0.003618 seconds (mean ± std over 1000 runs)
matrix2_row sum: 0.038303 ± 0.000427 seconds (mean ± std over 1000 runs)
matrix2_col sum: 0.047945 ± 0.000224 seconds (mean ± std over 1000 runs)
matrix2_row transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
matrix2_col transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
matrix2_row reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
matrix2_col reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
```


## Pandas
>TODO




# C++
## Build and Run
```bash
# Compile
make
# Run profiling
make run
```

## Results

### Small Matrix Test (10,000 × 500)
```
Matrix shapes:
- matrix1_row: 10000 × 500
- matrix1_col: 500 × 10000

=== Multiple Operations Profiling ===
matrix1_row std: 0.005418 ± 0.000584 seconds (mean ± std over 1000 runs)
matrix1_col std: 0.006995 ± 0.000728 seconds (mean ± std over 1000 runs)
matrix1_row mean: 0.005070 ± 0.000198 seconds (mean ± std over 1000 runs)
matrix1_col mean: 0.006531 ± 0.000556 seconds (mean ± std over 1000 runs)
matrix1_row sum: 0.000807 ± 0.000237 seconds (mean ± std over 1000 runs)
matrix1_col sum: 0.011760 ± 0.000411 seconds (mean ± std over 1000 runs)
matrix1_row transpose: 0.020845 ± 0.002473 seconds (mean ± std over 1000 runs)
matrix1_col transpose: 0.028166 ± 0.001442 seconds (mean ± std over 1000 runs)
matrix1_row reshape: 0.043924 ± 0.000815 seconds (mean ± std over 1000 runs)
matrix1_col reshape: 0.043992 ± 0.000792 seconds (mean ± std over 1000 runs)
```

### Large Matrix Test (100,000 × 500)
```
Matrix shapes:
- matrix2_row: 100000 × 500
- matrix2_col: 500 × 100000

=== Multiple Operations Profiling ===
matrix2_row std: 0.055370 ± 0.000542 seconds (mean ± std over 1000 runs)
matrix2_col std: 0.098416 ± 0.002269 seconds (mean ± std over 1000 runs)
matrix2_row mean: 0.053155 ± 0.000541 seconds (mean ± std over 1000 runs)
matrix2_col mean: 0.092817 ± 0.003418 seconds (mean ± std over 1000 runs)
matrix2_row sum: 0.017060 ± 0.000565 seconds (mean ± std over 1000 runs)
matrix2_col sum: 0.140549 ± 0.001737 seconds (mean ± std over 1000 runs)
matrix2_row transpose: 0.288169 ± 0.018702 seconds (mean ± std over 1000 runs)
matrix2_col transpose: 0.485323 ± 0.020625 seconds (mean ± std over 1000 runs)
matrix2_row reshape: 0.450841 ± 0.009467 seconds (mean ± std over 1000 runs)
matrix2_col reshape: 0.477592 ± 0.006183 seconds (mean ± std over 1000 runs)
```
