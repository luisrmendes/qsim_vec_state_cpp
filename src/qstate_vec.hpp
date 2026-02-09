#ifndef QSTATEVEC_HPP
#define QSTATEVEC_HPP

#include <complex>
#include <cstdint>
#include <expected>
#include <functional>
#include <vector>

#define PRECISION_TYPE double

using StateVector = std::vector<std::complex<PRECISION_TYPE>>;

enum class Error : std::uint8_t { invalid_input };

inline std::string to_string(Error err) {
    switch (err) {
    case Error::invalid_input:
        return "Invalid Input";
    }
    return "Unknown Error";
}

inline std::ostream& operator<<(std::ostream& ostr, Error err) {
    return ostr << to_string(err);
}

class QStateVec {
  private:
    StateVector main;
    StateVector parity;
    int num_qubits;

    /**
     * @brief Moves the values in parity vector to the main vector.
     *  Resets the parity layer to zero.
     */
    void reset_parity_layer();

  public:
    /**
     * @brief Construct a qubit layer object.
     *
     * @param numQubits
     */
    QStateVec(int num_qubits);

    /**
     * @brief Pretty prints the main and parity state vectors.
     *  Adequate to display small size state vectors.
     */
    void pretty_print() const;

    /**
     * @brief Returns the result of the measured collapsed qubits
     */
    [[nodiscard]] std::vector<PRECISION_TYPE> get_measured_qubits() const;

    /**
     * @brief Executes the Pauli X operation
     */
    auto pauli_x(int target_qubit) -> std::expected<void, Error>;

    /**
     * @brief Executes the Pauli Y operation
     */
    auto pauli_y(int target_qubit) -> std::expected<void, Error>;

    // uint64_t& getGlobalStartIndex() { return this->globalLowerBound; }

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