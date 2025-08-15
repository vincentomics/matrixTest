import numpy as np
import time
import os


def generate_count_mat(rows, cols, min_val=0, max_val=100, seed=42):
    """Generate a matrix of random integer counts with specified dimensions"""
    rng = np.random.default_rng(seed)
    count_mat = rng.integers(min_val, max_val + 1, size=(rows, cols))
    return count_mat

def write_matrix_to_txt(matrix, filename="temp_matrix.txt"):
    np.savetxt(filename, matrix, fmt='%d', delimiter="\t")

def read_matrix_from_txt(filename="temp_matrix.txt"):
    return np.loadtxt(filename, dtype=int, delimiter="\t")

def profile_operations(matrix_row, matrix_col, label="Matrix", n_iter=1000, filename_prefix="temp_matrix"):
    print(f"--- {label} Test ({matrix_row.shape[0]} x {matrix_row.shape[1]}) ---")
    print("Matrix shapes:")
    print(f"- row-major: {matrix_row.shape}")
    print(f"- col-major: {matrix_col.shape}")

    operations = {
        f"row-major sum": lambda: np.sum(matrix_row, axis=1),
        f"col-major sum": lambda: np.sum(matrix_col, axis=0),
        f"row-major mean": lambda: np.mean(matrix_row, axis=1),
        f"col-major mean": lambda: np.mean(matrix_col, axis=0),
        f"row-major std": lambda: np.std(matrix_row, axis=1),
        f"col-major std": lambda: np.std(matrix_col, axis=0),
        f"row-major transpose": lambda: matrix_row.T,
        f"col-major transpose": lambda: matrix_col.T,
        f"row-major reshape": lambda: matrix_row.reshape(-1, 50),
        f"col-major reshape": lambda: matrix_col.reshape(-1, 50),
        f"row-major write to txt": lambda: write_matrix_to_txt(matrix_row, filename=f"{filename_prefix}_row.txt"),
        f"col-major write to txt": lambda: write_matrix_to_txt(matrix_col, filename=f"{filename_prefix}_col.txt"),
        f"row-major read from txt": lambda: read_matrix_from_txt(filename=f"{filename_prefix}_row.txt"),
        f"col-major read from txt": lambda: read_matrix_from_txt(filename=f"{filename_prefix}_col.txt")
    }

    print("\n=== Multiple Operations Profiling ===")
    for op_name, operation in operations.items():
        times = []
        n_runs = n_iter if "read" not in op_name and "write" not in op_name else int(n_iter/10)
        for _ in range(n_runs):
            start_time = time.perf_counter()
            operation()
            end_time = time.perf_counter()
            times.append(end_time - start_time)
        mean_time = np.mean(times)
        std_time = np.std(times)

        print(f"{op_name:25s}: {mean_time:.6f} ± {std_time:.6f} seconds (mean ± std over {n_runs} runs)")

    # Clean up

    if os.path.exists(f"{filename_prefix}_row.txt"):
        os.remove(f"{filename_prefix}_row.txt")
    if os.path.exists(f"{filename_prefix}_col.txt"):
        os.remove(f"{filename_prefix}_col.txt")

def run_all_profiles():
    sizes = {
        "Tiny": (100, 100, 100000),
        "Small": (1_000, 1_000, 10000),
        "Medium": (10_000, 1_000, 1000),
        "Large": (100_000, 1_000, 100),
    }
    for label, (n_row, n_col, n_iter) in sizes.items():
        mat_row = generate_count_mat(n_row, n_col)
        mat_col = generate_count_mat(n_col, n_row)
        profile_operations(mat_row, mat_col, label=label, filename_prefix=f"temp_{label.lower()}_matrix", n_iter=n_iter)
        print("\n\n")

time_start = time.perf_counter()
run_all_profiles()
time_end = time.perf_counter()
print(f"\n=== Test time taken: {time_end - time_start:.2f} seconds ===\n")