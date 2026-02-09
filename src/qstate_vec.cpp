#include "qstate_vec.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

#define MASK(N) (0x1ull << N)
#define PRINT(var_name, var)                                                                       \
    std::cerr << "[DEBUG] " << __FILE__ << ":" << __LINE__ << ": " << (var_name) << ": " << (var)  \
              << std::endl

using namespace std;

auto QStateVec::pauli_y(const int target_qubit) -> expected<void, Error> {
    if (target_qubit > this->num_qubits || target_qubit <= 0) {
        return unexpected(Error::invalid_input);
    }

    for (size_t i = 0; i < this->main.size(); i++) {
        if (abs(this->main[i]) != 0) {
            // if |0>, scalar 1i applies to |1>
            // if |1>, scalar -1i applies to |0>
            auto target_state = i ^ MASK((target_qubit - 1));

            if ((target_state & MASK((target_qubit - 1))) != 0) {
                this->parity[target_state] = this->main[i] * 1i;
            } else {
                this->parity[target_state] = this->main[i] * -1i;
            }
        }
    }

    this->reset_parity_layer();
    return {};
}

auto QStateVec::pauli_x(const int target_qubit) -> expected<void, Error> {
    if (target_qubit > this->num_qubits || target_qubit <= 0) {
        return unexpected(Error::invalid_input);
    }

    for (size_t i = 0; i < this->main.size(); i++) {
        if (abs(this->main[i]) != 0) {
            auto target_state = i ^ MASK((target_qubit - 1));
            this->parity[target_state] = this->main[i];
        }
    }

    this->reset_parity_layer();
    return {};
}

void QStateVec::reset_parity_layer() {
    this->main = this->parity;
    std::ranges::fill(this->parity, std::complex<PRECISION_TYPE>{0.0, 0.0});
}

vector<PRECISION_TYPE> QStateVec::get_measured_qubits() const {
    auto measured_qubits = vector<PRECISION_TYPE>(this->num_qubits, 0.0);

    for (size_t i = 0; i < this->main.size(); i++) {
        if (abs(this->main[i]) == 0) {
            continue;
        }

        for (int j = 0; j < this->num_qubits; j++) {
            // Does the current state represent part of the qubit counted by j?
            if (i & MASK(j)) {
                measured_qubits[j] += norm(this->main[i]);
            }
        }
    }

    return measured_qubits;
}

void QStateVec::pretty_print() const {
    stringstream print_buf;
    print_buf << "Main:   ";

    for (const auto& qubit : this->main) {
        print_buf << qubit;
    }
    print_buf << "\n";

    print_buf << "Parity: ";
    for (const auto& qubit : this->parity) {
        print_buf << qubit;
    }
    print_buf << "\n";

    cout << print_buf.str() << "\n";
}

QStateVec::QStateVec(const int num_qubits) {
    this->num_qubits = num_qubits;
    uint64_t state_vec_size = 2 * static_cast<uint64_t>(pow(2, num_qubits));

    this->main = StateVector(state_vec_size, complex<PRECISION_TYPE>{0.0, 0.0});
    this->main[0] = complex<PRECISION_TYPE>{1.0, 0.0};
    this->parity = StateVector(state_vec_size, complex<PRECISION_TYPE>{0.0, 0.0});
}

// void QStateVec::calculateStateProbabilities() {
//     for (size_t i = 0; i < this->states.size(); i += 2)
//         this->states[i] = pow(abs(this->states[i]), 2);
// }

// void QStateVec::measureQubits(PRECISION_TYPE* results) {
//     uint64_t localStartIndex;
//     if (this->rank == 0)
//         localStartIndex = 0;
//     else
//         localStartIndex = this->layerLimits[this->rank - 1];

//     for (size_t i = 0; i < this->states.size(); i += 2) {
//         if (this->states[i] == 0i) {
//             localStartIndex += 1;
//             continue;
//         }
//         for (unsigned int j = 0; j < this->numQubits; j++) {
//             if (localStartIndex & MASK(j)) {
//                 results[j] += this->states[i].real();
//             }
//         }
//         localStartIndex += 1;
//     }
// }

// bool QStateVec::checkStateOOB(const uint64_t state) {
//     return state < this->globalLowerBound || state > this->globalUpperBound;
// }

// void QStateVec::toffoli(const int controlQubit1, const int controlQubit2,
//                             const int targetQubit) {
//     // Executes pauliX if both control qubits are set to |1>
//     auto applyReceivedOpsPauliX = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] = receivedOps[i + 1];
//         }
//     };

//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     uint64_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsPauliX);

//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             if (state & MASK(controlQubit1) && state & MASK(controlQubit2)) {
//                 state = state ^ MASK(targetQubit);

//                 // if a state is OOB, store tuple (state, intended_value) to a vector
//                 if (!checkStateOOB(state)) {
//                     size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                     this->states[2 * localIndex + 1] = this->states[2 * i];
//                 } else {
//                     // pair (state, intended_value)
//                     statesOOB.push_back({state, this->states[2 * i]});
//                 }
//             } else {
//                 this->states[2 * i + 1].real(this->states[2 * i].real());
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsPauliX);

//     updateStates();
// }

// void QStateVec::controlledX(const int controlQubit, const int targetQubit) {
//     // Executes pauliX if control qubit is |1>
//     auto applyReceivedOpsPauliX = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] = receivedOps[i + 1];
//         }
//     };
//     // vector of (stateOOB, value) pairs
//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     uint64_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsPauliX);

//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             if (state & MASK(controlQubit)) {
//                 state = state ^ MASK(targetQubit);

//                 // if a state is OOB, store tuple (state, intended_value) to a vector
//                 if (!checkStateOOB(state)) {
//                     size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                     this->states[2 * localIndex + 1] = this->states[2 * i];
//                 } else {
//                     // pair (state, intended_value)
//                     statesOOB.push_back({state, this->states[2 * i]});
//                 }
//             } else {
//                 this->states[2 * i + 1].real(this->states[2 * i].real());
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsPauliX);
//     updateStates();
// }

// void QStateVec::controlledZ(const int controlQubit, const int targetQubit) {
//     // Executes pauliZ if control qubit is |1>
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             if (state & MASK(controlQubit)) {
//                 state& MASK(targetQubit) ? this->states[2 * i + 1] = -this->states[2 * i]
//                                          : this->states[2 * i + 1] = this->states[2 * i];
//             } else {
//                 this->states[2 * i + 1].real(this->states[2 * i].real());
//             }
//         }
//     }
//     updateStates();
// }

// void QStateVec::rotationX(const int targetQubit, const double angle) {
//     // PRECISION_TYPE hadamard_const = 1 / sqrt(2);
//     PRECISION_TYPE rotationX_const1 = cos(angle);
//     complex<PRECISION_TYPE> rotationX_const2 = -1i * sin(angle);

//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     auto applyReceivedOpsRotationX = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] += rotationX_const2 * receivedOps[i + 1];
//         }
//     };

//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             (state & MASK(targetQubit))
//                 ? this->states[2 * i + 1] -= rotationX_const1 * this->states[2 * i]
//                 : this->states[2 * i + 1] += rotationX_const1 * this->states[2 * i];
//         }
//     }

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsRotationX);
//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             state = state ^ MASK(targetQubit);

//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 this->states[2 * localIndex + 1] += rotationX_const2 * this->states[2 * i];
//             } else {
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsRotationX);

//     updateStates();
// }

// void QStateVec::sqrtPauliX(const int targetQubit) {
//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     static const PRECISION_TYPE halfConst = (PRECISION_TYPE)1 / (PRECISION_TYPE)2;
//     static const complex<PRECISION_TYPE> localConst = {halfConst, halfConst};
//     static const complex<PRECISION_TYPE> remoteConst = {halfConst, -halfConst};

//     auto applyReceivedOpsSqrtPauliX = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] += remoteConst * receivedOps[i + 1];
//         }
//     };

//     /**
//      * TODO: experimentar meter esta operação num único loop
//      */
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             this->states[2 * i + 1] += localConst * this->states[2 * i];
//         }
//     }

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsSqrtPauliX);
//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             state = state ^ MASK(targetQubit);

//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 this->states[2 * localIndex + 1] += remoteConst * this->states[2 * i];
//             } else {
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsSqrtPauliX);

//     updateStates();
// }

// void QStateVec::sqrtPauliY(const int targetQubit) {
//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     static const PRECISION_TYPE halfConst = (PRECISION_TYPE)1 / (PRECISION_TYPE)2;
//     static const complex<PRECISION_TYPE> auxConst1 = {halfConst, halfConst};
//     static const complex<PRECISION_TYPE> auxConst2 = {-halfConst, -halfConst};

//     auto applyReceivedOpsSqrtPauliY = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             uint64_t state = this->globalLowerBound + receivedOps[i].real();

//             (state & MASK(targetQubit))
//                 ? this->states[2 * receivedOps[i].real() + 1] += auxConst1 * receivedOps[i + 1]
//                 : this->states[2 * receivedOps[i].real() + 1] += auxConst2 * receivedOps[i + 1];
//         }
//     };

//     /**
//      * TODO: experimentar meter esta operação num único loop
//      */
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             this->states[2 * i + 1] += auxConst1 * this->states[2 * i];
//         }
//     }

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsSqrtPauliY);
//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             state = state ^ MASK(targetQubit);

//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 (state & MASK(targetQubit))
//                     ? this->states[2 * localIndex + 1] += auxConst1 * this->states[2 * i]
//                     : this->states[2 * localIndex + 1] += auxConst2 * this->states[2 * i];
//             } else {
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsSqrtPauliY);

//     updateStates();
// }

// void QStateVec::sGate(const int targetQubit) {
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;

//             state& MASK(targetQubit) ? this->states[2 * i + 1] = 1i * this->states[2 * i]
//                                      : this->states[2 * i + 1] = this->states[2 * i];
//         }
//     }
//     updateStates();
// }

// void QStateVec::tGate(const int targetQubit) {
//     static const complex<PRECISION_TYPE> tConst =
//         exp((1i * (PRECISION_TYPE)M_PI) / (PRECISION_TYPE)4);

//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;

//             state& MASK(targetQubit) ? this->states[2 * i + 1] = tConst * this->states[2 * i]
//                                      : this->states[2 * i + 1] = this->states[2 * i];
//         }
//     }
//     updateStates();
// }

// void QStateVec::hadamard(const int targetQubit) {
//     // PRECISION_TYPE hadamard_const = 1 / sqrt(2);
//     static const PRECISION_TYPE hadamard_const = 1 / sqrt(2);

//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     auto applyReceivedOpsHadamard = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] += hadamard_const * receivedOps[i + 1];
//         }
//     };

//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             (state & MASK(targetQubit))
//                 ? this->states[2 * i + 1] -= hadamard_const * this->states[2 * i]
//                 : this->states[2 * i + 1] += hadamard_const * this->states[2 * i];
//         }
//     }

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsHadamard);
//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             state = state ^ MASK(targetQubit);

//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 this->states[2 * localIndex + 1] += hadamard_const * this->states[2 * i];
//             } else {
//                 // #ifdef HADAMARD_DEBUG_LOGS
//                 // 				dynamic_bitset state2 = state;
//                 // 				appendDebugLog("Hadamard: State |",
//                 // 							   state2.printBitset(),
//                 // 							   "> ",
//                 // 							   state,
//                 // 							   " out of bounds!\n");
//                 // #endif
//                 // pair (state, intended_value)
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsHadamard);

//     updateStates();
// }

// void QStateVec::pauliZ(const int targetQubit) {
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;

//             state& MASK(targetQubit) ? this->states[2 * i + 1] = -this->states[2 * i]
//                                      : this->states[2 * i + 1] = this->states[2 * i];
//         }
//     }
//     updateStates();
// }

// void QStateVec::pauliY(const int targetQubit) {
//     // vector of (stateOOB, value) pairs
//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;

//     auto applyReceivedOpsPauliY = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1].real() == 0
//                 ? this->states[2 * receivedOps[i].real() + 1] = receivedOps[i + 1] * 1i
//                 : this->states[2 * receivedOps[i].real() + 1] = receivedOps[i + 1] * -1i;
//         }
//     };

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; i < this->states.size() / 2; i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsPauliY);

//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             // if |0>, scalar 1i applies to |1>
//             // if |1>, scalar -1i aclear();
//             // probabily room for optimization here
//             state = state ^ MASK(targetQubit);

//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 state& MASK(targetQubit)
//                     ? this->states[2 * localIndex + 1] = this->states[2 * i] * 1i
//                     : this->states[2 * localIndex + 1] = this->states[2 * i] * -1i;
//             } else {
//                 // pair (state, intended_value)
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsPauliY);

//     updateStates();
// }

// void QStateVec::pauliX(const int targetQubit) {
//     // vector of (stateOOB, amplitude) pairs
//     vector<tuple<uint64_t, complex<PRECISION_TYPE>>> statesOOB;
//     // statesOOB.reserve(LOCKSTEP_THRESHOLD);

//     auto applyReceivedOpsPauliX = [&](const vector<complex<PRECISION_TYPE>>& receivedOps) {
//         for (size_t i = 0; i < receivedOps.size(); i += 2) {
//             this->states[2 * receivedOps[i].real() + 1] = receivedOps[i + 1];
//         }
//     };

//     size_t limit = LOCKSTEP_THRESHOLD;
//     for (size_t i = 0; (i < this->states.size() / 2); i++) {
//         if (i == limit) {
//             manageDistr(statesOOB, applyReceivedOpsPauliX);
//             statesOOB.clear();
//             limit += LOCKSTEP_THRESHOLD;
//         }
//         if (checkZeroState(i)) {
//             uint64_t state = this->globalLowerBound + i;
//             state = state ^ MASK(targetQubit);

//             // if a state is OOB, store tuple (state, intended_value) to a vector
//             if (!checkStateOOB(state)) {
//                 size_t localIndex = getLocalIndexFromGlobalState(state, this->rank);
//                 this->states[2 * localIndex + 1] = this->states[2 * i];
//             } else {
//                 // pair (state, amplitude) ATENCAO
//                 statesOOB.push_back({state, this->states[2 * i]});
//             }
//         }
//     }

//     manageDistr(statesOOB, applyReceivedOpsPauliX);

//     updateStates();
// }

// bool QStateVec::checkZeroState(const size_t i) {
//     return this->states[i * 2] != 0i;
// }

// void QStateVec::updateStates() {
//     for (size_t i = 0; i < this->states.size(); i += 2) {
//         this->states[i] = this->states[i + 1];
//         this->states[i + 1] = 0i;
//     }
// }
