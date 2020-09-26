#pragma once

#include <climits>
#include <vector>

#define INF UINT_MAX
#define WIDTH 6

class Matrix
{
public:
    explicit Matrix(const std::vector<std::vector<std::size_t>>& v);
    ~Matrix();

    std::size_t& operator()(std::size_t row, std::size_t column);
    const std::size_t& operator()(std::size_t row, std::size_t column) const;

    void RemoveRowColumn(const std::size_t& row, const std::size_t& column);
    std::size_t Size() const;
    void Print() const;
    std::vector<std::size_t> GetRowIndices() const;
    std::vector<std::size_t> GetColumnIndices() const;

    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;

private:
    std::vector<std::vector<std::size_t>> items;
    std::vector<std::size_t> rowIndices;
    std::vector<std::size_t> columnIndices;
};
