#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Matrix.h"

Matrix::Matrix(const std::vector<std::vector<std::size_t>>& v) :
    items(v.size(), std::vector<std::size_t>(v.size())),
    rowIndices(v.size()),
    columnIndices(v.size())
{
    for (std::size_t i = 0; i < rowIndices.size(); ++i)
    {
        rowIndices.at(i) = i;

        for (std::size_t j = 0; j < columnIndices.size(); ++j)
        {
            items.at(i).at(j) = v.at(i).at(j);
        }
    }

    columnIndices = rowIndices;
}

Matrix::~Matrix()
{
}

std::size_t& Matrix::operator()(std::size_t row, std::size_t column)
{
    auto itR = std::find(rowIndices.begin(), rowIndices.end(), row);
    auto itC = std::find(columnIndices.begin(), columnIndices.end(), column);

    return items.at(std::distance(rowIndices.begin(), itR)).at(std::distance(columnIndices.begin(), itC));
}

const std::size_t& Matrix::operator()(std::size_t row, std::size_t column) const
{
    auto itR = std::find(rowIndices.begin(), rowIndices.end(), row);
    auto itC = std::find(columnIndices.begin(), columnIndices.end(), column);

    return items.at(std::distance(rowIndices.begin(), itR)).at(std::distance(columnIndices.begin(), itC));
}

void Matrix::RemoveRowColumn(const std::size_t& row, const std::size_t& column)
{
    auto itR = std::find(rowIndices.begin(), rowIndices.end(), row);
    std::size_t distR = std::distance(rowIndices.begin(), itR);
    auto itC = std::find(columnIndices.begin(), columnIndices.end(), column);
    std::size_t distC = std::distance(columnIndices.begin(), itC);

    rowIndices.erase(itR);
    columnIndices.erase(itC);

    items.erase(items.begin() + distR);

    for (std::size_t i = 0; i < items.size(); ++i)
    {
        items.at(i).erase(items.at(i).begin() + distC);
    }
}

std::size_t Matrix::Size() const
{
    return rowIndices.size();
}

void Matrix::Print() const
{
    for (auto& i : rowIndices)
    {
        for (auto& j : columnIndices)
        {
            std::cout << std::setw(WIDTH);
            (*this)(i, j) == INF ? std::cout << "INF" : std::cout << (*this)(i, j);
        }
 
        std::cout << std::endl;
    }
}

std::vector<std::size_t> Matrix::GetRowIndices() const
{
    return rowIndices;
}

std::vector<std::size_t> Matrix::GetColumnIndices() const
{
    return columnIndices;
}
