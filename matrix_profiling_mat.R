library(microbenchmark)

# Generate random integer matrix
generate_count_mat <- function(rows, cols, min_val = 0, max_val = 100, seed = 42) {
  set.seed(seed)
  matrix(sample(min_val:max_val, rows * cols, replace = TRUE), nrow = rows, ncol = cols)
}

# File I/O for matrix
write_matrix_to_txt <- function(mat, filename) {
  write.table(mat, file = filename, sep = "\t", row.names = FALSE, col.names = FALSE)
}
read_matrix_from_txt <- function(filename) {
  as.matrix(read.table(file = filename, sep = "\t", header = FALSE))
}


# Matrix benchmarking
profile_matrix_operations <- function(mat_row, mat_col, label = "Matrix", n_iter = 100, filename_prefix = "temp_matrix") {
  cat(sprintf("--- %s Test (%d x %d) ---\n", label, nrow(mat_row), ncol(mat_row)))

  operations <- list(
    "row-major sum" = function() rowSums(mat_row),
    "col-major sum" = function() colSums(mat_col),
    "row-major mean" = function() rowMeans(mat_row),
    "col-major mean" = function() colMeans(mat_col),
    "row-major std" = function() apply(mat_row, 1, sd),
    "col-major std" = function() apply(mat_col, 2, sd),
    "row-major transpose" = function() t(mat_row),
    "col-major transpose" = function() t(mat_col),
    "row-major reshape" = function() matrix(mat_row, ncol = 50),
    "col-major reshape" = function() matrix(mat_col, ncol = 50),
    "row-major write to txt" = function() write_matrix_to_txt(mat_row, paste0(filename_prefix, "_row.txt")),
    "col-major write to txt" = function() write_matrix_to_txt(mat_col, paste0(filename_prefix, "_col.txt")),
    "row-major read from txt" = function() read_matrix_from_txt(paste0(filename_prefix, "_row.txt")),
    "col-major read from txt" = function() read_matrix_from_txt(paste0(filename_prefix, "_col.txt"))
  )

  cat("=== Multiple Operations Profiling ===\n")
  for (op_name in names(operations)) {
    times <- microbenchmark(operations[[op_name]](), times = if (grepl("read|write", op_name)) 10 else n_iter)
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
    "Small" = c(10000, 100),
    "Medium" = c(100000, 500),
    "Large" = c(1000000, 500)
  )

  for (label in names(sizes)) {
    dims <- sizes[[label]]
    rows <- dims[1]
    cols <- dims[2]

    # Matrix
    mat_row <- generate_count_mat(rows, cols)
    mat_col <- generate_count_mat(cols, rows)
    profile_matrix_operations(mat_row, mat_col, label = paste("Matrix", label), filename_prefix = paste0("temp_", tolower(label), "_matrix"))

  }
}

# Run everything
run_all_benchmarks()
