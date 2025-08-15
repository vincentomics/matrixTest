#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <cstdio> // for std::remove
#include <map>
#include <tuple>

class Matrix {
private:
    std::vector<std::vector<int>> data;
    size_t rows, cols;

public:
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(rows, std::vector<int>(cols));
    }

    void generateRandom(int min_val = 0, int max_val = 100, unsigned int seed = 42) {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(min_val, max_val);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                data[i][j] = dis(gen);
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    int& operator()(size_t i, size_t j) { return data[i][j]; }
    const int& operator()(size_t i, size_t j) const { return data[i][j]; }

    std::vector<int> rowSum() const {
        std::vector<int> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            int sum = 0;
            for (size_t j = 0; j < cols; ++j) sum += data[i][j];
            result[i] = sum;
        }
        return result;
    }

    std::vector<int> colSum() const {
        std::vector<int> result(cols);
        for (size_t j = 0; j < cols; ++j) {
            int sum = 0;
            for (size_t i = 0; i < rows; ++i) sum += data[i][j];
            result[j] = sum;
        }
        return result;
    }

    std::vector<double> rowMean() const {
        std::vector<double> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < cols; ++j) sum += data[i][j];
            result[i] = sum / cols;
        }
        return result;
    }

    std::vector<double> colMean() const {
        std::vector<double> result(cols);
        for (size_t j = 0; j < cols; ++j) {
            double sum = 0.0;
            for (size_t i = 0; i < rows; ++i) sum += data[i][j];
            result[j] = sum / rows;
        }
        return result;
    }

    std::vector<double> rowStd() const {
        std::vector<double> result(rows);
        for (size_t i = 0; i < rows; ++i) {
            double sum = 0.0, sum_sq = 0.0;
            for (size_t j = 0; j < cols; ++j) {
                double val = static_cast<double>(data[i][j]);
                sum += val;
                sum_sq += val * val;
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
                double val = static_cast<double>(data[i][j]);
                sum += val;
                sum_sq += val * val;
            }
            double mean = sum / rows;
            double variance = (sum_sq / rows) - (mean * mean);
            result[j] = std::sqrt(variance);
        }
        return result;
    }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result(j, i) = data[i][j];
        return result;
    }

    Matrix reshape(size_t new_rows, size_t new_cols) const {
        if (rows * cols != new_rows * new_cols)
            throw std::invalid_argument("Invalid reshape dimensions");

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
    }

    static Matrix readFromFile(const std::string& filename, size_t rows, size_t cols) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Unable to open file for reading");
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                file >> result(i, j);
        return result;
    }
};

// Timer and statistics
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;

public:
    void start() { start_time = std::chrono::high_resolution_clock::now(); }
    double elapsed() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        return duration.count() / 1e9;
    }
};

struct Statistics {
    double mean;
    double std_dev;
};

Statistics calculateStats(const std::vector<double>& times) {
    double sum = 0.0, sum_sq = 0.0;
    for (double t : times) {
        sum += t;
        sum_sq += t * t;
    }
    double mean = sum / times.size();
    double variance = (sum_sq / times.size()) - (mean * mean);
    return {mean, std::sqrt(variance)};
}

template<typename Func>
Statistics profileOperation(const std::string& name, Func operation, int n_iter = 1000) {
    std::vector<double> times;
    times.reserve(n_iter);
    for (int i = 0; i < n_iter; ++i) {
        Timer timer;
        timer.start();
        operation();
        times.push_back(timer.elapsed());
    }
    Statistics stats = calculateStats(times);
    std::cout << std::left << std::setw(25) << name
              << ": " << std::fixed << std::setprecision(6)
              << stats.mean << " ± " << stats.std_dev
              << " seconds (mean ± std over " << n_iter << " runs)" << std::endl;
    return stats;
}

void profile_operations(const Matrix& mat_row, const Matrix& mat_col, const std::string& label, int n_iter, const std::string& filename_prefix) {
    std::cout << "--- " << label << " Test (" << mat_row.getRows() << " x " << mat_row.getCols() << ") ---\n";
    std::cout << "Matrix shapes:\n";
    std::cout << "- row-major: " << mat_row.getRows() << " x " << mat_row.getCols() << "\n";
    std::cout << "- col-major: " << mat_col.getRows() << " x " << mat_col.getCols() << "\n\n";

    // Define operations and their lambdas - use vector to maintain order
    std::vector<std::pair<std::string, std::function<void()>>> operations = {
        {"row-major sum",    [&]() { mat_row.rowSum(); }},
        {"col-major sum",    [&]() { mat_col.colSum(); }},
        {"row-major mean",   [&]() { mat_row.rowMean(); }},
        {"col-major mean",   [&]() { mat_col.colMean(); }},
        {"row-major std",    [&]() { mat_row.rowStd(); }},
        {"col-major std",    [&]() { mat_col.colStd(); }},
        {"row-major transpose", [&]() { mat_row.transpose(); }},
        {"col-major transpose", [&]() { mat_col.transpose(); }},
        {"row-major reshape",    [&]() { 
            size_t new_cols = std::min(50UL, mat_row.getCols());
            size_t new_rows = (mat_row.getRows() * mat_row.getCols()) / new_cols;
            mat_row.reshape(new_rows, new_cols); 
        }},
        {"col-major reshape",    [&]() { 
            size_t new_cols = std::min(50UL, mat_col.getCols());
            size_t new_rows = (mat_col.getRows() * mat_col.getCols()) / new_cols;
            mat_col.reshape(new_rows, new_cols); 
        }},
        {"row-major write to txt",   [&]() { mat_row.writeToFile(filename_prefix + "_row.txt"); }},
        {"col-major write to txt",   [&]() { mat_col.writeToFile(filename_prefix + "_col.txt"); }},
        {"row-major read from txt",  [&]() { Matrix::readFromFile(filename_prefix + "_row.txt", mat_row.getRows(), mat_row.getCols()); }},
        {"col-major read from txt",  [&]() { Matrix::readFromFile(filename_prefix + "_col.txt", mat_col.getRows(), mat_col.getCols()); }},
    };

    std::cout << "=== Multiple Operations Profiling ===\n";
    for (const auto& [name, op] : operations) {
        int runs = (name.find("read") != std::string::npos || name.find("write") != std::string::npos) ? n_iter / 10 : n_iter;
        profileOperation(name, op, runs);
    }

    // Cleanup files
    std::remove((filename_prefix + "_row.txt").c_str());
    std::remove((filename_prefix + "_col.txt").c_str());

    std::cout << std::string(60, '-') << "\n";
}

int main() {
    auto time_start = std::chrono::high_resolution_clock::now();
    std::cout << "=== C++ Matrix Operations Profiling ===\n\n";

    std::vector<std::pair<std::string, std::tuple<size_t, size_t, size_t>>> sizes = {
        {"Tiny",  {100, 100, 100000}},
        {"Small",  {1000, 1000, 10000}},
        {"Medium", {10000, 1000, 1000}},
        {"Large",  {100000, 1000, 100}},
    };

    for (const auto& [label, dims] : sizes) {
        size_t nrow = std::get<0>(dims);
        size_t ncol = std::get<1>(dims);
        size_t n_iter = std::get<2>(dims);

        Matrix mat_row(nrow, ncol);
        Matrix mat_col(ncol, nrow);

        // Use fixed seed for reproducibility, like numpy RNG in Python
        mat_row.generateRandom(0, 100, 42);
        mat_col.generateRandom(0, 100, 42);

        profile_operations(mat_row, mat_col, label, n_iter, "temp_" + label + "_matrix");
        std::cout << "\n\n";
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    std::cout << "\n=== Test time taken: " << std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start).count() << " seconds ===\n";

    return 0;
}
