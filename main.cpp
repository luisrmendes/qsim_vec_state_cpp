#include <iostream>
#include "qvec_state.hpp"
#include <cmath>

using namespace std;

int main() {
    QStateVec state(2);
    state.pretty_print();

    complex<double> test1 = {1, 0};
    complex<double> test2 = {1, -1};

    cout << sqrt(pow(test1, 2)) << "\n";
    cout << sqrt(pow(test2, 2)) << "\n";
    return 0;
}