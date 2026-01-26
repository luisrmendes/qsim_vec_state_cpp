#ifndef QSTATEVEC_HPP
#define QSTATEVEC_HPP

#include <complex>
#include <cstdint>
#include <functional>
#include <vector>

#define PRECISION_TYPE double

typedef std::vector<std::complex<PRECISION_TYPE>> StateVector;

class QStateVec {
  private:
    StateVector main;
    StateVector parity;
    unsigned int num_qubits;

    /**
     * @brief Returns a vector with the size of state vector segment allocation
     * per each process represented in the index. Takes in account the input and
     * output states.
     *
     * @return std::vector<size_t>
     */
    // std::vector<size_t> calculateLayerAlloc();

  public:
    /**
     * @brief Construct a qubit layer object.
     *
     * @param numQubits
     */
    QStateVec(const unsigned int num_qubits);

    /**
     * @brief Pretty prints the main and parity state vectors.
     *  Adequate to display small size state vectors.
     */
    void pretty_print();

    /**
     * @brief Returns the result of the measured collapsed qubits
     */
    std::vector<PRECISION_TYPE> get_measured_qubits();

    // uint64_t& getGlobalStartIndex() { return this->globalLowerBound; }

    // void pauliX(const int targetQubit);
    // void pauliY(const int targetQubit);
    // void pauliZ(const int targetQubit);
    // void hadamard(const int targetQubit);
    // void controlledZ(const int controlQubit, const int targetQubit);
    // void controlledX(const int controlQubit, const int targetQubit);
    // void toffoli(const int controlQubit1, const int controlQubit2, const int targetQubit);
    // void rotationX(const int targetQubit, const double angle);
    // void sqrtPauliX(const int targetQubit);
    // void sqrtPauliY(const int targetQubit);
    // void sGate(const int targetQubit);
    // void tGate(const int targetQubit);

    // void clearStates() {
    //     this->states.clear();
    //     this->states.shrink_to_fit();
    // }

    /**
     * @brief copies values from output state to input state;
     * sets output states to 0i
     */
    // void updateStates();

    /**
     * @brief Measures qubit probabilities of state vector segment.
     * Must receive an array from which the return values are placed.
     * C-style array is used because MPI.
     *
     * @param result
     */
    // void measureQubits(PRECISION_TYPE* result);
};

#endif