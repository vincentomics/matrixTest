#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <string>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(rows, std::vector<int>(cols));
    }

    // Generate random matrix
    void generateRandom(int min_val = 0, int max_val = 100) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min_val, max_val);
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = dis(gen);
            }
        }
    }

    // Get dimensions
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Access elements
    int& operator()(size_t i, size_t j) { return data[i][j]; }
    const int& operator()(size_t i, size_t j) const { return data[i][j]; }

    // Matrix operations
    std::vector<double> rowStd() const {
        std::vector<double> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0, sum_sq = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                sum += data[i][j];
                sum_sq += static_cast<double>(data[i][j]) * data[i][j];
            }
            double mean = sum / cols;
            double variance = (sum_sq / cols) - (mean * mean);
            result[i] = std::sqrt(variance);
        }
        return result;
    }

    std::vector<double> colStd() const {
        std::vector<double> result(cols);
        for (size_t j = 0; j < cols; ++j) {
            double sum = 0.0, sum_sq = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                sum += data[i][j];
                sum_sq += static_cast<double>(data[i][j]) * data[i][j];
            }
            double mean = sum / rows;
            double variance = (sum_sq / rows) - (mean * mean);
            result[j] = std::sqrt(variance);
        }
        return result;
    }

    std::vector<double> rowMean() const {
        std::vector<double> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                sum += data[i][j];
            }
            result[i] = sum / cols;
        }
        return result;
    }

    std::vector<double> colMean() const {
        std::vector<double> result(cols);
        for (size_t j = 0; j < cols; ++j) {
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) {
                sum += data[i][j];
            }
            result[j] = sum / rows;
        }
        return result;
    }

    std::vector<int> rowSum() const {
        std::vector<int> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            int sum = 0;
            for (size_t j = 0; j < cols; ++j) {
                sum += data[i][j];
            }
            result[i] = sum;
        }
        return result;
    }

    std::vector<int> colSum() const {
        std::vector<int> result(cols);
        for (size_t j = 0; j < cols; ++j) {
            int sum = 0;
            for (size_t i = 0; i < rows; ++i) {
                sum += data[i][j];
            }
            result[j] = sum;
        }
        return result;
    }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(j, i) = data[i][j];
            }
        }
        return result;
    }

    Matrix reshape(size_t new_rows, size_t new_cols) const {
        if (rows * cols != new_rows * new_cols) {
            throw std::invalid_argument("Invalid reshape dimensions");
        }
        Matrix result(new_rows, new_cols);
        size_t idx = 0;
        for (size_t i = 0; i < new_rows; ++i) {
            for (size_t j = 0; j < new_cols; ++j) {
                size_t old_i = idx / cols;
                size_t old_j = idx % cols;
                result(i, j) = data[old_i][old_j];
                ++idx;
            }
        }
        return result;
    }
};

// Timing utilities
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        return duration.count() / 1e9; // Convert to seconds
    }
};

// Statistics utilities
struct Statistics {
    double mean;
    double std_dev;
};

Statistics calculateStats(const std::vector<double>& times) {
    double sum = 0.0, sum_sq = 0.0;
    for (double time : times) {
        sum += time;
        sum_sq += time * time;
    }
    
    double mean = sum / times.size();
    double variance = (sum_sq / times.size()) - (mean * mean);
    double std_dev = std::sqrt(variance);
    
    return {mean, std_dev};
}

// Profiling function
template<typename Func>
Statistics profileOperation(const std::string& name, Func operation, int num_runs = 1000) {
    std::vector<double> times;
    times.reserve(num_runs);
    
    for (int i = 0; i < num_runs; ++i) {
        Timer timer;
        timer.start();
        operation();
        times.push_back(timer.elapsed());
    }
    
    Statistics stats = calculateStats(times);
    std::cout << std::left << std::setw(15) << name 
              << ": " << std::fixed << std::setprecision(6) 
              << stats.mean << " ± " << stats.std_dev 
              << " seconds (mean ± std over " << num_runs << " runs)" << std::endl;
    
    return stats;
}

int main() {
    std::cout << "=== C++ Matrix Operations Profiling ===" << std::endl;
    
    // Test with smaller matrices
    std::cout << "\n--- Small Matrix Test (10,000 x 500) ---" << std::endl;
    Matrix matrix1_row(10000, 500);
    Matrix matrix1_col(500, 10000);
    
    matrix1_row.generateRandom();
    matrix1_col.generateRandom();
    
    std::cout << "Matrix shapes:" << std::endl;
    std::cout << "- matrix1_row: " << matrix1_row.getRows() << " x " << matrix1_row.getCols() << std::endl;
    std::cout << "- matrix1_col: " << matrix1_col.getRows() << " x " << matrix1_col.getCols() << std::endl;
    
    std::cout << "\n=== Multiple Operations Profiling ===" << std::endl;
    
    profileOperation("matrix1_row std", [&]() { matrix1_row.rowStd(); });
    profileOperation("matrix1_col std", [&]() { matrix1_col.colStd(); });
    profileOperation("matrix1_row mean", [&]() { matrix1_row.rowMean(); });
    profileOperation("matrix1_col mean", [&]() { matrix1_col.colMean(); });
    profileOperation("matrix1_row sum", [&]() { matrix1_row.rowSum(); });
    profileOperation("matrix1_col sum", [&]() { matrix1_col.colSum(); });
    profileOperation("matrix1_row transpose", [&]() { matrix1_row.transpose(); });
    profileOperation("matrix1_col transpose", [&]() { matrix1_col.transpose(); });
    profileOperation("matrix1_row reshape", [&]() { matrix1_row.reshape(50000, 100); });
    profileOperation("matrix1_col reshape", [&]() { matrix1_col.reshape(50000, 100); });
    
    // Test with larger matrices
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "\n--- Large Matrix Test (100,000 x 500) ---" << std::endl;
    Matrix matrix2_row(100000, 500);
    Matrix matrix2_col(500, 100000);
    
    matrix2_row.generateRandom();
    matrix2_col.generateRandom();
    
    std::cout << "Matrix shapes:" << std::endl;
    std::cout << "- matrix2_row: " << matrix2_row.getRows() << " x " << matrix2_row.getCols() << std::endl;
    std::cout << "- matrix2_col: " << matrix2_col.getRows() << " x " << matrix2_col.getCols() << std::endl;
    
    std::cout << "\n=== Multiple Operations Profiling ===" << std::endl;
    
    profileOperation("matrix2_row std", [&]() { matrix2_row.rowStd(); });
    profileOperation("matrix2_col std", [&]() { matrix2_col.colStd(); });
    profileOperation("matrix2_row mean", [&]() { matrix2_row.rowMean(); });
    profileOperation("matrix2_col mean", [&]() { matrix2_col.colMean(); });
    profileOperation("matrix2_row sum", [&]() { matrix2_row.rowSum(); });
    profileOperation("matrix2_col sum", [&]() { matrix2_col.colSum(); });
    profileOperation("matrix2_row transpose", [&]() { matrix2_row.transpose(); });
    profileOperation("matrix2_col transpose", [&]() { matrix2_col.transpose(); });
    profileOperation("matrix2_row reshape", [&]() { matrix2_row.reshape(500000, 100); });
    profileOperation("matrix2_col reshape", [&]() { matrix2_col.reshape(500000, 100); });
    
    return 0;
}
