#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>


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

    void writeToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Unable to open file for writing");

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                file << data[i][j];
                if (j < cols - 1) file << "\t";
            }
            file << "\n";
        }
        file.close();
    }

    static Matrix readFromFile(const std::string& filename, size_t rows, size_t cols) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Unable to open file for reading");

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                file >> result(i, j);
            }
        }
        file.close();
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

    std::vector<std::pair<std::string, std::pair<size_t, size_t>>> sizes = {
        {"Small",  {10'000, 100}},
        {"Medium", {100'000, 500}},
        {"Large",  {1'000'000, 500}}
    };

    for (const auto& [label, dims] : sizes) {
        size_t nrow = dims.first;
        size_t ncol = dims.second;

        std::cout << "\n--- " << label << " Matrix Test (" << nrow << " x " << ncol << ") ---" << std::endl;

        Matrix mat_row(nrow, ncol);
        Matrix mat_col(ncol, nrow);
        mat_row.generateRandom();
        mat_col.generateRandom();

        std::cout << "Matrix shapes:\n";
        std::cout << "- mat_row: " << mat_row.getRows() << " x " << mat_row.getCols() << std::endl;
        std::cout << "- mat_col: " << mat_col.getRows() << " x " << mat_col.getCols() << std::endl;

        std::cout << "\n=== Multiple Operations Profiling ===" << std::endl;

        profileOperation("row sum", [&]() { mat_row.rowSum(); });
        profileOperation("col sum", [&]() { mat_col.colSum(); });
        profileOperation("row mean", [&]() { mat_row.rowMean(); });
        profileOperation("col mean", [&]() { mat_col.colMean(); });
        profileOperation("row std", [&]() { mat_row.rowStd(); });
        profileOperation("col std", [&]() { mat_col.colStd(); });
        profileOperation("row reshape", [&]() { mat_row.reshape(nrow, 50); });
        profileOperation("col reshape", [&]() { mat_col.reshape(ncol, 50); });
        profileOperation("row transpose", [&]() { mat_row.transpose(); });
        profileOperation("col transpose", [&]() { mat_col.transpose(); });
        profileOperation("row write to file", [&]() { mat_row.writeToFile("temp_" + label + "_row.txt"); });
        profileOperation("col write to file", [&]() { mat_col.writeToFile("temp_" + label + "_col.txt"); });
        profileOperation("row read from file", [&]() { Matrix::readFromFile("temp_" + label + "_row.txt", nrow, ncol); });
        profileOperation("col read from file", [&]() { Matrix::readFromFile("temp_" + label + "_col.txt", ncol, nrow); });

        std::cout << std::string(60, '-') << std::endl;
    }

    return 0;
}
