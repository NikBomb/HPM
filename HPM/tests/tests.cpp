#include "Matrix.h"
#include <gtest/gtest.h>
#include <iostream>

namespace
{
    TEST(IterationTest, ColumnMajor)
    {
        bombace::Matrix<int> A{std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 4, 3};
        int counter = 1;
        for (auto col = bombace::ColIterator(A).begin(); col != bombace::ColIterator(A).end(); ++col)
        {
            for (int i = 0; i < 4; i++)
            {
                EXPECT_EQ(col.at(i), counter);
                counter++;
            }
        }
    }

}