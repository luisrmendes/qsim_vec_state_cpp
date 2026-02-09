#include "qstate_vec.hpp"
#include <gtest/gtest.h>

using namespace std;

// Test more extensive Pauli Y operation
TEST(QStateVec, PauliY2) {
    QStateVec tst_sv(5);
    tst_sv.pauli_y(1);
    tst_sv.pauli_y(2);
    tst_sv.pauli_y(3);
    tst_sv.pauli_y(4);
    tst_sv.pauli_y(5);

    auto results = tst_sv.get_measured_qubits();

    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 1);
    EXPECT_EQ(results[2], 1);
    EXPECT_EQ(results[3], 1);
    EXPECT_EQ(results[4], 1);
}

// Test bad Pauli Y operation and the returns of the op
TEST(QStateVec, BadPauliY) {
    QStateVec tst_sv(2);
    auto res = tst_sv.pauli_y(-1);
    ASSERT_FALSE(res);
    if (!res) {
        EXPECT_EQ("Invalid Input", to_string(res.error()));
    }

    auto results = tst_sv.get_measured_qubits();

    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 0);
}

// Test normal Pauli Y operation and the returns of the op
TEST(QStateVec, PauliY) {
    QStateVec tst_sv(2);
    auto res = tst_sv.pauli_y(1);
    auto results = tst_sv.get_measured_qubits();

    ASSERT_TRUE(res);
    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 0);
}

// Test bad inputs on Pauli X and the return error
TEST(QStateVec, BadPauliX) {
    QStateVec tst_sv(2);
    tst_sv.pauli_x(-1);
    tst_sv.pauli_x(3);
    auto results = tst_sv.get_measured_qubits();
    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 0);

    auto res = tst_sv.pauli_x(0);
    ASSERT_FALSE(res);
    if (!res) {
        EXPECT_EQ("Invalid Input", to_string(res.error()));
    }
}

// Test normal Pauli X operation and the returns of the op
TEST(QStateVec, PauliX) {
    QStateVec tst_sv(2);
    tst_sv.pauli_x(1);
    auto res = tst_sv.pauli_x(2);
    auto results = tst_sv.get_measured_qubits();

    ASSERT_TRUE(res);
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
