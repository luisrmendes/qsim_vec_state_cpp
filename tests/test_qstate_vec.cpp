#include <gtest/gtest.h>
#include "qstate_vec.hpp"

using namespace std;

// TEST(QStateVec, BadPauliX) {
//     QStateVec tst_sv(2); 
//     auto results = tst_sv.get_measured_qubits();

//     EXPECT_EQ(results[0], 0);
//     EXPECT_EQ(results[1], 0);
//     EXPECT_EQ(results.size(), 2);
// }

// Test normal Pauli X operation
TEST(QStateVec, PauliX) {
    QStateVec tst_sv(2); 
    tst_sv.pauli_x(0);
    tst_sv.pauli_x(1);
    auto results = tst_sv.get_measured_qubits();

    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 1);
}

// Test normal State Vector creation
TEST(QStateVec, StateVecCreation) {
    QStateVec tst_sv(2); 
    auto results = tst_sv.get_measured_qubits();

    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 0);
    EXPECT_EQ(results.size(), 2);
}
