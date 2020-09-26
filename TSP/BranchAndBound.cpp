
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include "BranchAndBound.h"

BranchAndBound::BranchAndBound(const Matrix& matrix) :
	originalMatrix(matrix),
	record(INF)
{
}

BranchAndBound::~BranchAndBound()
{
}

void BranchAndBound::SolveTSP()
{
	std::cout << "Solving TSP using the Branch and Bound method" << std::endl << std::endl;
	std::cout << "Distance matrix" << std::endl << std::endl;
	originalMatrix.Print();
	std::cout << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl << std::endl;

	SolveTSPRec(originalMatrix, optimalPath, 0);

	std::vector<std::size_t> solution;
	solution.push_back(optimalPath.front().first);

	while (!optimalPath.empty())
	{
		auto it = optimalPath.begin();

		while (it != optimalPath.end())
		{
			if (it->first == solution.back())
			{
				solution.push_back(it->second);
				it = optimalPath.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	std::cout << "Optimal path: ";

	std::string delim = "";
	for (auto& i : solution)
	{
		std::cout << delim << i + 1;
		delim = " -> ";
	}
	std::cout << std::endl;

	std::cout << "Cost: " << record << std::endl << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl;
}

void BranchAndBound::SolveTSPRec(const Matrix& matrix, const Path& path, std::size_t lowerBound)
{
	if (matrix.Size() == 2)
	{
		matrix.Print();
		std::cout << std::endl;

		Path currentPath(path.begin(), path.end());

		for (auto& i : matrix.GetRowIndices())
		{
			for (auto& j : matrix.GetColumnIndices())
			{
				if (matrix(i, j) != INF)
				{
					currentPath.push_back(std::make_pair(i, j));
				}
			}
		}

		std::cout << "Current path contains: ";
		std::string delim = "";
		for (auto& i : currentPath)
		{
			std::cout << delim << "(" << i.first << ", " << i.second << ")";
			delim = ", ";
		}
		std::cout << std::endl;

		std::size_t cost = Cost(currentPath);
		std::cout << "Current cost: " << cost << std::endl;

		if (cost < record)
		{
			std::cout << "New record" << std::endl;

			optimalPath = currentPath;
			record = cost;
		}

		std::cout << std::endl;
		std::cout << "------------------------------------------------------------------" << std::endl << std::endl;

		return;
	}

	Matrix reducedMatrix = matrix;
	lowerBound += ReduceMatrix(reducedMatrix);

	if (reducedMatrix.Size() == (originalMatrix).Size())
	{
		std::cout << "Reduced matrix" << std::endl << std::endl;
	}	

	reducedMatrix.Print();
	std::cout << std::endl;

	if (lowerBound > record)
	{
		std::cout << "Lower bound (" << lowerBound << ") exceeds current record (" << record << ")" << std::endl;
		std::cout << "Discarding this branch" << std::endl << std::endl;
		std::cout << "------------------------------------------------------------------" << std::endl << std::endl;
		return;
	}

	Edge edge = MaximumPenaltyEdge(reducedMatrix);
	std::cout << "Branching by the edge (" << edge.first << ", " << edge.second << ")" << std::endl << std::endl;

	Matrix containsEdgeMatrix = reducedMatrix;
	containsEdgeMatrix.RemoveRowColumn(edge.first, edge.second);
	AddInfinity(containsEdgeMatrix);

	Path containsEdgePath = path;
	containsEdgePath.push_back(edge);	

	SolveTSPRec(containsEdgeMatrix, containsEdgePath, lowerBound);

	Matrix doesNotContainEdgeMatrix = matrix;
	ReduceMatrix(doesNotContainEdgeMatrix);
	doesNotContainEdgeMatrix(edge.first, edge.second) = INF;

	Path doesNotcontainEdgePath;

	std::cout << "Branching by not edge (" << edge.first << ", " << edge.second << ")" << std::endl << std::endl;

	SolveTSPRec(doesNotContainEdgeMatrix, doesNotcontainEdgePath, lowerBound);
}

std::size_t BranchAndBound::Cost(const Path& path)
{
	std::size_t cost = 0;

	for (auto& i : path)
	{
		cost += originalMatrix(i.first, i.second);
	}

	return cost;
}

std::size_t BranchAndBound::ReduceMatrix(Matrix& reducedMatrix)
{
	return ReduceRows(reducedMatrix) + ReduceColumns(reducedMatrix);
}

std::size_t BranchAndBound::ReduceRows(Matrix& reducedMatrix)
{
	std::size_t lowerBound = 0;

	for (auto& i : reducedMatrix.GetRowIndices())
	{
		std::size_t min = INF;

		for (auto& j : reducedMatrix.GetColumnIndices())
		{
			if (reducedMatrix(i, j) < min)
			{
				min = reducedMatrix(i, j);
			}
		}

		lowerBound += min;

		for (auto& j : reducedMatrix.GetColumnIndices())
		{
			if (reducedMatrix(i, j) != INF)
			{
				reducedMatrix(i, j) -= min;
			}
		}
	}

	return lowerBound;
}

std::size_t BranchAndBound::ReduceColumns(Matrix& reducedMatrix)
{
	std::size_t lowerBound = 0;

	for (auto& j : reducedMatrix.GetColumnIndices())
	{
		std::size_t min = INF;

		for (auto& i : reducedMatrix.GetRowIndices())
		{
			if (reducedMatrix(i, j) < min)
			{
				min = reducedMatrix(i, j);
			}
		}

		lowerBound += min;

		for (auto& i : reducedMatrix.GetRowIndices())
		{
			if (reducedMatrix(i, j) != INF)
			{
				reducedMatrix(i, j) -= min;
			}
		}
	}

	return lowerBound;
}

BranchAndBound::Edge BranchAndBound::MaximumPenaltyEdge(const Matrix& reducedMatrix)
{
	std::map<Edge, std::size_t> penalties;

	for (auto& i : reducedMatrix.GetRowIndices())
	{
		for (auto& j : reducedMatrix.GetColumnIndices())
		{
			if (reducedMatrix(i, j) == 0)
			{
				penalties[std::make_pair(i, j)] = Penalty(reducedMatrix, std::make_pair(i, j));
			}
		}
	}

	Edge maximumPenaltyEdge;
	std::size_t maxPenalty = 0;

	for (auto it = penalties.begin(); it != penalties.end(); ++it)
	{
		if (it->second > maxPenalty)
		{
			maximumPenaltyEdge = it->first;
			maxPenalty = it->second;
		}
	}
	
	return maximumPenaltyEdge;
}

std::size_t BranchAndBound::Penalty(const Matrix& reducedMatrix, const Edge& zero)
{
	std::size_t minR = INF;

	for (auto& i : reducedMatrix.GetColumnIndices())
	{
		if (reducedMatrix(zero.first, i) < minR && i != zero.second)
		{
			minR = reducedMatrix(zero.first, i);
		}
	}

	std::size_t minC = INF;

	for (auto& i : reducedMatrix.GetRowIndices())
	{
		if (reducedMatrix(i, zero.second) < minC && i != zero.first)
		{
			minC = reducedMatrix(i, zero.second);
		}
	}

	return minR + minC;
}

void BranchAndBound::AddInfinity(Matrix& containsEdgeMatrix)
{
	std::size_t row = INF;

	for (auto& i : containsEdgeMatrix.GetRowIndices())
	{
		bool flag = false;

		for (auto& j : containsEdgeMatrix.GetColumnIndices())
		{
			if (containsEdgeMatrix(i, j) == INF)
			{
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			row = i;
		}
	}

	std::size_t column = INF;

	for (auto& j : containsEdgeMatrix.GetColumnIndices())
	{
		bool flag = false;

		for (auto& i : containsEdgeMatrix.GetRowIndices())
		{
			if (containsEdgeMatrix(i, j) == INF)
			{
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			column = j;
		}
	}

	if (row != INF && column != INF)
	{
		containsEdgeMatrix(row, column) = INF;
	}
}
