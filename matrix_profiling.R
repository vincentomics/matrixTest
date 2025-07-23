# ----------------------------
# 🔧 Utility Functions
# ----------------------------

generate_count_mat <- function(rows, cols, min_val = 0, max_val = 100, seed = 42) {
  set.seed(seed)
  matrix(sample(min_val:max_val, rows * cols, replace = TRUE), nrow = rows, ncol = cols)
}

matrix_to_df <- function(mat) as.data.frame(mat)

# ----------------------------
# 📊 Matrix-Only Profiling
# ----------------------------

profile_matrix_operations <- function(mat_row, mat_col, label, n_iter = 100) {
  cat("\n\nMatrix shapes:\n")
  cat(sprintf("- %s_row: %d x %d\n", label, nrow(mat_row), ncol(mat_row)))
  cat(sprintf("- %s_col: %d x %d\n", label, nrow(mat_col), ncol(mat_col)))
  cat("\n=== Multiple Operations Profiling ===\n")

  mat_ops <- list(
    "row std"       = function() apply(mat_row, 1, sd),
    "col std"       = function() apply(mat_col, 2, sd),
    "row mean"      = function() apply(mat_row, 1, mean),
    "col mean"      = function() apply(mat_col, 2, mean),
    "row sum"       = function() apply(mat_row, 1, sum),
    "col sum"       = function() apply(mat_col, 2, sum),
    "row transpose" = function() t(mat_row),
    "col transpose" = function() t(mat_col),
    "row reshape"   = function() matrix(mat_row, ncol = 50),
    "col reshape"   = function() matrix(mat_col, ncol = 50)
  )

  for (name in names(mat_ops)) {
    times <- numeric(n_iter)
    for (i in 1:n_iter) {
      start <- proc.time()[["elapsed"]]
      invisible(mat_ops[[name]]())
      end <- proc.time()[["elapsed"]]
      times[i] <- end - start
    }

    avg <- mean(times)
    std <- sd(times)

    cat(sprintf("%-20s: %.6f ± %.6f seconds (mean ± std over %d runs)\n",
                paste0(label, "_", name), avg, std, n_iter))
  }
}

# ----------------------------
# 🧾 DataFrame-Only Profiling
# ----------------------------

profile_dataframe_operations <- function(mat_row, mat_col, label, n_iter = 100) {
  cat("\n\nDataFrame shapes:\n")
  cat(sprintf("- %s_row: %d x %d\n", label, nrow(mat_row), ncol(mat_row)))
  cat(sprintf("- %s_col: %d x %d\n", label, nrow(mat_col), ncol(mat_col)))
  cat("\n=== Multiple Operations Profiling ===\n")

  df_row <- matrix_to_df(mat_row)
  df_col <- matrix_to_df(mat_col)

  df_ops <- list(
    "row std"       = function() apply(df_row, 1, sd),
    "col std"       = function() apply(df_col, 2, sd),
    "row mean"      = function() apply(df_row, 1, mean),
    "col mean"      = function() apply(df_col, 2, mean),
    "row sum"       = function() apply(df_row, 1, sum),
    "col sum"       = function() apply(df_col, 2, sum),
    "row transpose" = function() t(as.matrix(df_row)),
    "col transpose" = function() t(as.matrix(df_col)),
    "row reshape"   = function() as.data.frame(matrix(unlist(df_row), ncol = 50, byrow = TRUE)),
    "col reshape"   = function() as.data.frame(matrix(unlist(df_col), ncol = 50, byrow = TRUE))
  )

  for (name in names(df_ops)) {
    times <- numeric(n_iter)
    for (i in 1:n_iter) {
      start <- proc.time()[["elapsed"]]
      invisible(df_ops[[name]]())
      end <- proc.time()[["elapsed"]]
      times[i] <- end - start
    }

    avg <- mean(times)
    std <- sd(times)

    cat(sprintf("%-20s: %.6f ± %.6f seconds (mean ± std over %d runs)\n",
                paste0(label, "_", name), avg, std, n_iter))
  }
}

# ----------------------------
# 🚀 Run All Tests
# ----------------------------

# Small matrix tests
mat1_row <- generate_count_mat(10000, 500)
mat1_col <- generate_count_mat(500, 10000)
profile_matrix_operations(mat1_row, mat1_col, "mat1", n_iter = 100)

# Large matrix tests
mat2_row <- generate_count_mat(100000, 500)
mat2_col <- generate_count_mat(500, 100000)
profile_matrix_operations(mat2_row, mat2_col, "mat2", n_iter = 100)

# Small data frame tests
mat3_row <- generate_count_mat(10000, 500)
mat3_col <- generate_count_mat(500, 10000)
profile_dataframe_operations(mat3_row, mat3_col, "df1", n_iter = 100)

# Large data frame tests
mat4_row <- generate_count_mat(100000, 500)
mat4_col <- generate_count_mat(500, 100000)
profile_dataframe_operations(mat4_row, mat4_col, "df2", n_iter = 100)
