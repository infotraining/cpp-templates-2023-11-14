#include "vector.hpp"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <sstream>

using namespace std;

SCENARIO("Policy based design for vector", "[Vector]")
{
    GIVEN("Vector with ThrowingErrorPolicy")
    {
        Vector<int, ThrowingRangeChecker> vec = {1, 2, 3};

        WHEN("index is out of range")
        {
            THEN("exception is thrown")
            {
                REQUIRE_THROWS_AS(vec.at(5), std::out_of_range);
            }
        }
    }

    GIVEN("Vector with logging error policy")
    {
        Vector<int, LoggingErrorRangeChecker, StdLock> vec = {1, 2, 3};
        stringstream mock_log;
        vec.set_log_file(mock_log);

        WHEN("index is out of range")
        {
            auto result = vec.at(5);

            THEN("error is logged into a file")
            {
                REQUIRE_THAT(mock_log.str(), Catch::Matchers::ContainsSubstring("Error: Index out of range."));
            }

            THEN("last item is returned")
            {
                REQUIRE(result == 3);
            }
        }
    }
}
