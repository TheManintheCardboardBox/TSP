#pragma once

#include <climits>
#include <memory>
#include <vector>
#include <utility>
#include "Matrix.h"

class BranchAndBound
{
	using Edge = std::pair<std::size_t, std::size_t>;
	using Path = std::vector<Edge>;

public:
	BranchAndBound(const Matrix& matrix);
	~BranchAndBound();

	void SolveTSP();

private:
	void SolveTSPRec(const Matrix& matrix, const Path& path, std::size_t lowerBound);
	std::size_t Cost(const Path& path);
	std::size_t ReduceMatrix(Matrix& reducedMatrix);
	std::size_t ReduceRows(Matrix& reducedMatrix);
	std::size_t ReduceColumns(Matrix& reducedMatrix);
	Edge MaximumPenaltyEdge(const Matrix& reducedMatrix);
	std::size_t Penalty(const Matrix& reducedMatrix, const Edge& zero);
	void AddInfinity(Matrix& containEdgeMatrix);

	Matrix originalMatrix;
	std::size_t record;
	Path optimalPath;
};
