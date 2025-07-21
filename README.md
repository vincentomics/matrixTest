Performance benchmarks comparing matrix operations using row-major vs column-major orientation. Tests evaluate common operations (mean, standard deviation, sum, transpose, reshape) on small and large matrices in different programming languages.


# Python
## NumPy (`matrix_profiling_np.ipynb`)
### Small Matrix (10,000 × 500)
```
Matrix shapes:
- mat1_row: (10000, 500)
- mat1_col: (500, 10000)

=== Multiple Operations Profiling ===
mat1_row std   : 0.032689 ± 0.000834 seconds (mean ± std over 1000 runs)
mat1_col std   : 0.030677 ± 0.001359 seconds (mean ± std over 1000 runs)
mat1_row mean  : 0.005789 ± 0.000095 seconds (mean ± std over 1000 runs)
mat1_col mean  : 0.005915 ± 0.000395 seconds (mean ± std over 1000 runs)
mat1_row sum   : 0.003081 ± 0.000160 seconds (mean ± std over 1000 runs)
mat1_col sum   : 0.003322 ± 0.000121 seconds (mean ± std over 1000 runs)
mat1_row transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat1_col transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat1_row reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat1_col reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
```

### Large Matrix (100,000 × 500)
```
Matrix shapes:
- mat2_row:  (100000, 500)
- mat2_col:  (500, 100000)

=== Multiple Operations Profiling ===
mat2_row std   : 0.339105 ± 0.013733 seconds (mean ± std over 1000 runs)
mat2_col std   : 0.453834 ± 0.155855 seconds (mean ± std over 1000 runs)
mat2_row mean  : 0.056295 ± 0.000564 seconds (mean ± std over 1000 runs)
mat2_col mean  : 0.059546 ± 0.001250 seconds (mean ± std over 1000 runs)
mat2_row sum   : 0.036302 ± 0.000322 seconds (mean ± std over 1000 runs)
mat2_col sum   : 0.046006 ± 0.000667 seconds (mean ± std over 1000 runs)
mat2_row transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat2_col transpose: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat2_row reshape: 0.000000 ± 0.000000 seconds (mean ± std over 1000 runs)
mat2_col reshape: 0.000000 ± 0.000001 seconds (mean ± std over 1000 runs)
```


## Pandas (`matrix_profiling_pd.ipynb`)
### Small Matrix (10,000 × 500)
```
DataFrame shapes:
- df1_row: (10000, 500)
- df1_col: (500, 10000)

=== Multiple Operations Profiling ===
df1_row std      : 0.051562 ± 0.000987 seconds (mean ± std over 1000 runs)
df1_col std      : 0.048520 ± 0.001471 seconds (mean ± std over 1000 runs)
df1_row mean     : 0.006009 ± 0.000179 seconds (mean ± std over 1000 runs)
df1_col mean     : 0.005399 ± 0.000371 seconds (mean ± std over 1000 runs)
df1_row sum      : 0.003168 ± 0.000127 seconds (mean ± std over 1000 runs)
df1_col sum      : 0.003037 ± 0.000126 seconds (mean ± std over 1000 runs)
df1_row transpose: 0.000083 ± 0.000006 seconds (mean ± std over 1000 runs)
df1_col transpose: 0.000638 ± 0.000022 seconds (mean ± std over 1000 runs)
df1_row reshape  : 0.000014 ± 0.000006 seconds (mean ± std over 1000 runs)
df1_col reshape  : 0.000014 ± 0.000003 seconds (mean ± std over 1000 runs)
```

### Large Matrix (100,000 × 500)
```
DataFrame shapes:
- df2_row: (100000, 500)
- df2_col: (500, 100000)

=== Multiple Operations Profiling ===
df2_row std    : 1.079865 ± 0.524880 seconds (mean ± std over 1000 runs)
df2_col std    : 0.600393 ± 0.177012 seconds (mean ± std over 1000 runs)
df2_row mean   : 0.067530 ± 0.000698 seconds (mean ± std over 1000 runs)
df2_col mean   : 0.070661 ± 0.007745 seconds (mean ± std over 1000 runs)
df2_row sum    : 0.046184 ± 0.000347 seconds (mean ± std over 1000 runs)
df2_col sum    : 0.052997 ± 0.000224 seconds (mean ± std over 1000 runs)
df2_row transpose: 0.000084 ± 0.000011 seconds (mean ± std over 1000 runs)
df2_col transpose: 0.005900 ± 0.000114 seconds (mean ± std over 1000 runs)
df2_row reshape: 0.000014 ± 0.000011 seconds (mean ± std over 1000 runs)
df2_col reshape: 0.000014 ± 0.000002 seconds (mean ± std over 1000 runs)
```


# C++
## Build and Run
```bash
# Compile
make
# Run profiling
make run
```

## Results

### Small Matrix
```
Matrix shapes:
- mat1_row: 10000 x 500
- mat1_col: 500 x 10000

=== Multiple Operations Profiling ===
mat1_row std   : 0.005302 ± 0.000065 seconds (mean ± std over 1000 runs)
mat1_col std   : 0.007257 ± 0.000603 seconds (mean ± std over 1000 runs)
mat1_row mean  : 0.005080 ± 0.000100 seconds (mean ± std over 1000 runs)
mat1_col mean  : 0.006765 ± 0.000526 seconds (mean ± std over 1000 runs)
mat1_row sum   : 0.000793 ± 0.000052 seconds (mean ± std over 1000 runs)
mat1_col sum   : 0.012943 ± 0.001479 seconds (mean ± std over 1000 runs)
mat1_row transpose: 0.022760 ± 0.001684 seconds (mean ± std over 1000 runs)
mat1_col transpose: 0.032140 ± 0.001369 seconds (mean ± std over 1000 runs)
mat1_row reshape: 0.042536 ± 0.001544 seconds (mean ± std over 1000 runs)
mat1_col reshape: 0.042440 ± 0.000905 seconds (mean ± std over 1000 runs)
```

### Large Matrix
```
Matrix shapes:
- mat2_row: 100000 x 500
- mat2_col: 500 x 100000

=== Multiple Operations Profiling ===
mat2_row std   : 0.055088 ± 0.001228 seconds (mean ± std over 1000 runs)
mat2_col std   : 0.107474 ± 0.013479 seconds (mean ± std over 1000 runs)
mat2_row mean  : 0.054145 ± 0.005881 seconds (mean ± std over 1000 runs)
mat2_col mean  : 0.091631 ± 0.007014 seconds (mean ± std over 1000 runs)
mat2_row sum   : 0.023738 ± 0.002975 seconds (mean ± std over 1000 runs)
mat2_col sum   : 0.140037 ± 0.005347 seconds (mean ± std over 1000 runs)
mat2_row transpose: 0.274522 ± 0.010989 seconds (mean ± std over 1000 runs)
mat2_col transpose: 0.498102 ± 0.017182 seconds (mean ± std over 1000 runs)
mat2_row reshape: 0.450668 ± 0.003951 seconds (mean ± std over 1000 runs)
mat2_col reshape: 0.476853 ± 0.004305 seconds (mean ± std over 1000 runs)
```


# R
> TODO
