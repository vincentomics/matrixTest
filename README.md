Performance benchmarks comparing matrix operations using row-major vs column-major orientation. Tests evaluate common operations (mean, standard deviation, sum, transpose, reshape) from small and large matrices in different programming languages.


# Design
### Matrix Configurations
| Size       | Dimensions      | Elements    | Memory (MB) | Test iterations |
| ---------- | --------------- | ----------- | ----------- | --------------- |
| **Tiny**   | 100 × 100       | 10,000      | 0.08        | 100,000         |
| **Small**  | 1,000 × 1,000   | 1,000,000   | 7.63        | 10,000          |
| **Medium** | 10,000 × 1,000  | 10,000,000  | 76.29       | 1,000           |
| **Large**  | 100,000 × 1,000 | 100,000,000 | 762.94      | 100             |

(*File I/O uses 10× fewer iterations*)

### Operations
**Row-major vs Column-major** matrix operation performance are measured:
- **Calculation**: `sum` `mean` `std` (axis-aligned)
- **Structure**: `transpose` `reshape` 
- **File I/O**: `read`, `write`

（*Note that for numpy, transpose and reshape is just a view, this might cause some operations in python looks faster than that of C++*）

### Languages & Libraries
- **C++**
- **Python** → NumPy + Pandas  
- **R** → Base matrices + DataFrames


# Run
## C++ compile
```bash
make # Compile
# make run # Run profiling
```

## run test
```bash
bash run.sh &
```


# Results
TBA
