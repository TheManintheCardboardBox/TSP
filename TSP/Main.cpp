#include "BranchAndBound.h"

int main()
{
    std::vector<std::vector<std::size_t>> v =
    {
        { INF,  7,   5,   3,   1,   5,   2,   6,   7,   5  },
        {  7,  INF,  2,   4,   5,   4,   6,   1,   3,   4  },
        {  5,   2,  INF,  4,   7,   7,   5,   3,   6,   1  },
        {  3,   4,   4,  INF,  6,   7,   1,   5,   7,   3  },
        {  1,   5,   7,   6,  INF,  2,   3,   6,   5,   7  },
        {  5,   4,   7,   7,   2,  INF,  4,   5,   3,   8  },
        {  2,   6,   5,   1,   3,   4,  INF,  7,   7,   6  },
        {  6,   1,   3,   5,   6,   5,   7,  INF,  2,   4  },
        {  7,   3,   6,   7,   5,   3,   7,   2,  INF,  5  },
        {  5,   4,   1,   3,   7,   8,   6,   4,   5,  INF }
    };

    Matrix m(v);
    BranchAndBound solver(m);
    solver.SolveTSP();

    return 0;
}
