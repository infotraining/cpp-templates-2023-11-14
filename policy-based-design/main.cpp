#include "vector.hpp"
#include <iostream>
#include <mutex>

using namespace std;

int main()
{
    Vector<int, LoggingErrorRangeChecker, std::mutex> vec = {1, 2, 3};

    //Vector<int, ThrowingRangeChecker> vec2 = vec;

    vec.set_log_file(std::cout);

    cout << "vec: ";
    for (size_t i = 0; i < vec.size(); ++i)
        cout << vec.at(i) << " ";
    cout << endl;

    vec.at(10);
}
