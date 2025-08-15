library(microbenchmark)

# Generate random integer matrix
generate_count_mat <- function(rows, cols, min_val = 0, max_val = 100, seed = 42) {
  set.seed(seed)
  matrix(sample(min_val:max_val, rows * cols, replace = TRUE), nrow = rows, ncol = cols)
}

# File I/O for dataframe
write_df_to_txt <- function(df, filename) {
  write.table(df, file = filename, sep = "\t", row.names = FALSE, col.names = FALSE)
}
read_df_from_txt <- function(filename) {
  read.table(file = filename, sep = "\t", header = FALSE)
}

# DataFrame benchmarking
profile_dataframe_operations <- function(df_row, df_col, label = "DataFrame", n_iter = 1000, filename_prefix = "temp_df") {
  cat(sprintf("--- %s Test (%d x %d) ---\n", label, nrow(df_row), ncol(df_row)))

  operations <- list(
    "row-major sum" = function() rowSums(df_row),
    "col-major sum" = function() colSums(df_col),
    "row-major mean" = function() rowMeans(df_row),
    "col-major mean" = function() colMeans(df_col),
    "row-major std" = function() apply(df_row, 1, sd),
    "col-major std" = function() apply(df_col, 2, sd),
    "row-major transpose" = function() t(as.matrix(df_row)),
    "col-major transpose" = function() t(as.matrix(df_col)),
    "row-major reshape" = function() matrix(as.matrix(df_row), ncol = 50),
    "col-major reshape" = function() matrix(as.matrix(df_col), ncol = 50),
    "row-major write to txt" = function() write_df_to_txt(df_row, paste0(filename_prefix, "_row.txt")),
    "col-major write to txt" = function() write_df_to_txt(df_col, paste0(filename_prefix, "_col.txt")),
    "row-major read from txt" = function() read_df_from_txt(paste0(filename_prefix, "_row.txt")),
    "col-major read from txt" = function() read_df_from_txt(paste0(filename_prefix, "_col.txt"))
  )

  cat("=== Multiple Operations Profiling ===\n")
  for (op_name in names(operations)) {
    n_runs <- ifelse(!grepl("read|write", op_name), n_iter, as.integer(n_iter/10))
    times <- microbenchmark(operations[[op_name]](), times = n_runs)
    mean_time <- mean(times$time) / 1e9
    sd_time <- sd(times$time) / 1e9
    cat(sprintf("%-30s: %.6f ± %.6f seconds (mean ± sd over %d runs)\n",
                op_name, mean_time, sd_time, length(times$time)))
  }

  unlink(c(paste0(filename_prefix, "_row.txt"), paste0(filename_prefix, "_col.txt")))
  cat("\n")
}


run_all_benchmarks <- function() {
  sizes <- list(
    "Tiny" = c(100, 100, 100000),
    "Small" = c(1000, 1000, 10000),
    "Medium" = c(10000, 1000, 1000),
    "Large" = c(100000, 1000, 100)
  )

  for (label in names(sizes)) {
    dims <- sizes[[label]]
    rows <- dims[1]
    cols <- dims[2]
    n_iter <- dims[3]

    # DataFrame
    df_row <- as.data.frame(generate_count_mat(rows, cols))
    df_col <- as.data.frame(generate_count_mat(cols, rows))
    profile_dataframe_operations(df_row, df_col, label = paste("DataFrame", label), filename_prefix = paste0("temp_", tolower(label), "_df"), n_iter = n_iter)
    cat("\n\n")
    
  }
}

# Run everything
time_taken <- system.time(run_all_benchmarks())
cat(sprintf("\n=== Test time taken: %.2f seconds ===\n", time_taken["elapsed"]))