import pandas as pd
import numpy as np
import time
import os


def generate_count_df(rows, cols, min_val=0, max_val=100, seed=42):
    """Generate a DataFrame of random integer counts with specified dimensions"""
    rng = np.random.default_rng(seed)
    data = rng.integers(min_val, max_val + 1, size=(rows, cols))
    return pd.DataFrame(data)

def write_df_to_txt(df, filename="temp_df.txt"):
    df.to_csv(filename, sep="\t", header=False, index=False)

def read_df_from_txt(filename="temp_df.txt"):
    return pd.read_csv(filename, sep="\t", header=None)

def profile_operations(df_row, df_col, label="DataFrame", n_iter=1000, filename_prefix="temp_df"):
    print(f"--- {label} Test ({df_row.shape[0]} x {df_row.shape[1]}) ---")
    print("DataFrame shapes:")
    print(f"- row-oriented: {df_row.shape}")
    print(f"- col-oriented: {df_col.shape}")

    operations = {
        f"row-oriented sum": lambda: df_row.sum(axis=1),
        f"col-oriented sum": lambda: df_col.sum(axis=0),
        f"row-oriented mean": lambda: df_row.mean(axis=1),
        f"col-oriented mean": lambda: df_col.mean(axis=0),
        f"row-oriented std": lambda: df_row.std(axis=1),
        f"col-oriented std": lambda: df_col.std(axis=0),
        f"row-oriented transpose": lambda: df_row.T,
        f"col-oriented transpose": lambda: df_col.T,
        f"row-oriented reshape (flat -> 50)": lambda: pd.DataFrame(df_row.values.reshape(-1, 50)),
        f"col-oriented reshape (flat -> 50)": lambda: pd.DataFrame(df_col.values.reshape(-1, 50)),
        f"row-oriented write to txt": lambda: write_df_to_txt(df_row, filename=f"{filename_prefix}_row.txt"),
        f"col-oriented write to txt": lambda: write_df_to_txt(df_col, filename=f"{filename_prefix}_col.txt"),
        f"row-oriented read from txt": lambda: read_df_from_txt(filename=f"{filename_prefix}_row.txt"),
        f"col-oriented read from txt": lambda: read_df_from_txt(filename=f"{filename_prefix}_col.txt")
    }

    print("\n=== Multiple Operations Profiling ===")
    for op_name, operation in operations.items():
        times = []
        n_runs = n_iter if "read" not in op_name and "write" not in op_name else max(1, int(n_iter / 10))
        for _ in range(n_runs):
            start_time = time.perf_counter()
            operation()
            end_time = time.perf_counter()
            times.append(end_time - start_time)
        mean_time = np.mean(times)
        std_time = np.std(times)

        print(f"{op_name:35s}: {mean_time:.6f} ± {std_time:.6f} seconds (mean ± std over {n_runs} runs)")

    # Clean up
    for suffix in ["row", "col"]:
        path = f"{filename_prefix}_{suffix}.txt"
        if os.path.exists(path):
            os.remove(path)

def run_all_profiles():
    sizes = {
        "Tiny": (100, 100, 100000),
        "Small": (1_000, 1_000, 10000),
        "Medium": (10_000, 1_000, 1000),
        "Large": (100_000, 1_000, 100),
    }
    for label, (n_row, n_col, n_iter) in sizes.items():
        df_row = generate_count_df(n_row, n_col)
        df_col = generate_count_df(n_col, n_row)
        profile_operations(df_row, df_col, label=label, filename_prefix=f"temp_{label.lower()}_df", n_iter=n_iter)
        print("\n\n")

time_start = time.perf_counter()
run_all_profiles()
time_end = time.perf_counter()
print(f"\n=== Test time taken: {time_end - time_start:.2f} seconds ===\n")