#include "qstate_vec.hpp"
#include <cmath>
#include <iostream>

#define MASK(N) (0x1ull << (N))

using namespace std;

void print_results(vector<PRECISION_TYPE> results) {
    const int centi_to_percentile = 100;
    for (size_t i = 0; i < results.size(); ++i) {
        cout << "Qubit " << i + 1 << ": " << results[i] * centi_to_percentile << "%\n";
    }
}

int main() {
    QStateVec test_state_vector(2);
    test_state_vector.pretty_print();
    // test_state_vector.pauli_x(0);
    test_state_vector.pauli_y(1);
    // test_state_vector.pauli_x(2);
    test_state_vector.pretty_print();

    auto results = test_state_vector.get_measured_qubits();

    print_results(results);

    return 0;
}