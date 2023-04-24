#include "Matrix.h"
#include <gtest/gtest.h>
#include <iostream>

namespace
{
    TEST(IterationTest, ColumnMajor)
    {
        bombace::Matrix<int> A{std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, 4, 3};
        int counter = 1;
        
        for (auto dim = A.dimItBegin(); dim != A.dimItEnd(); ++dim) {
		    for (auto el = dim.vecItBegin(); el != dim.vecItEnd(); ++el) {
			    EXPECT_EQ(*el, counter);
                counter++;
		    }
	    }
    }
}