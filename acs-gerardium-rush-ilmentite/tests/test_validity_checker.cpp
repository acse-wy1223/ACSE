#include <iostream>
#include "CCircuit.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <numeric>

/**
 * @brief A test class inheriting from Circuit to access private members for testing.
 */
class TestCircuit : public Circuit {
public:
    using Circuit::Circuit;

    /**
     * @brief Set units from a vector.
     * 
     * This function initializes the units in the circuit based on the provided vector.
     * 
     * @param circuit_vector The input vector representing the circuit configuration.
     */
    void set_units_from_vector(const std::vector<int>& circuit_vector) {
        int unit_count = (circuit_vector.size() - 1) / 3;
        units.resize(unit_count);
        for (int i = 0; i < unit_count; ++i) {
            int base_index = 1 + i * 3;
            units[i].conc_num = circuit_vector[base_index];
            units[i].inter_num = circuit_vector[base_index + 1];
            units[i].tails_num = circuit_vector[base_index + 2];
        }
    }

    /**
     * Tests the max value check of a circuit vector.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector A vector of integers representing the circuit.
     * @return A boolean indicating whether the max value check passed.
     */
    bool test_max_value_check(int vector_size, const std::vector<int>& circuit_vector) {
        return max_value_check(vector_size, const_cast<int*>(circuit_vector.data()));
    }

    /**
     * Tests the value check of a circuit vector.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector A vector of integers representing the circuit.
     * @return A boolean indicating whether the value check passed.
     */
    bool test_check_values(int vector_size, const std::vector<int>& circuit_vector) {
        return check_values(vector_size, const_cast<int*>(circuit_vector.data()));
    }

    /**
     * Tests the self-recycle check.
     *
     * @return A boolean indicating whether the self-recycle check passed.
     */
    bool test_self_recycle_check() {
        return self_recycle_check();
    }

    /**
     * Tests the same unit destination check.
     *
     * @return A boolean indicating whether the same unit destination check passed.
     */
    bool test_same_unit_dest_check() {
        return same_unit_dest_check();
    }

    /**
     * Tests the end of vector check.
     *
     * @return A boolean indicating whether the end of vector check passed.
     */
    bool test_end_of_vector_check() {
        return end_of_vector_check();
    }

    /**
     * Tests the concentrate tailing check.
     *
     * @return A boolean indicating whether the concentrate tailing check passed.
     */
    bool test_concentrate_tailing_check() {
        return concentrate_tailing_check();
    }

    /**
     * Tests the reachability check.
     *
     * @return A boolean indicating whether the reachability check passed.
     */
    bool test_is_reachable() {
        return is_reachable();
    }

    /**
     * Tests the tail percentage to concentrate outlet check.
     *
     * @return A boolean indicating whether the tail percentage to concentrate outlet check passed.
     */
    bool test_tail_percentage_to_concentrate_outlet_check() {
        return tail_percentage_to_concentrate_outlet_check();
    }

    /**
     * Tests the feed value check of a circuit vector.
     *
     * @param circuit_vector A vector of integers representing the circuit.
     * @return A boolean indicating whether the feed value check passed.
     */
    bool test_check_feed_value(const std::vector<int>& circuit_vector){
        return check_feed_value(const_cast<int*>(circuit_vector.data()));
    }

    /**
     * Tests all checks on a circuit vector and records the names of the checks that failed.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector A pointer to the circuit vector.
     * @param fails A vector of strings to store the names of the checks that failed.
     * @return A boolean indicating whether all checks passed.
     */
    bool test_all(int vector_size, const int* circuit_vector, std::vector<std::string>& fails) {
        bool result = true;
        if (!test_max_value_check(vector_size, std::vector<int>(circuit_vector, circuit_vector + vector_size))) {
            fails.push_back("max_value_check");
            result = false;
        }
        if (!test_check_values(vector_size, std::vector<int>(circuit_vector, circuit_vector + vector_size))) {
            fails.push_back("check_values");
            result = false;
        }
        if (!test_self_recycle_check()) {
            fails.push_back("self_recycle_check");
            result = false;
        }
        if (!test_same_unit_dest_check()) {
            fails.push_back("same_unit_dest_check");
            result = false;
        }
        if (!test_end_of_vector_check()) {
            fails.push_back("end_of_vector_check");
            result = false;
        }
        if (!test_concentrate_tailing_check()) {
            fails.push_back("concentrate_tailing_check");
            result = false;
        }
        if (!test_is_reachable()) {
            fails.push_back("is_reachable");
            result = false;
        }
        if (!test_tail_percentage_to_concentrate_outlet_check()) {
            fails.push_back("tail_percentage_to_concentrate_outlet_check");
            result = false;
        }
        if (!test_check_feed_value(std::vector<int>(circuit_vector, circuit_vector + vector_size))) {
            fails.push_back("check_feed_value");
            result = false;
        }
        return result;
    }
};

/**
 * @brief Test function for max_value_check.
 * 
 * This function tests if the max_value_check method correctly identifies values 
 * in the circuit vector that exceed the valid range.
 */
void test_max_value_check() {
    TestCircuit circuit(3);
    // Vector with values exceeding the valid range
    std::vector<int> invalid_vector = {0, 10, 2, 2, 3, 0, 4, 0, 1, 1};
    circuit.set_units_from_vector(invalid_vector);
    bool result = circuit.test_max_value_check(invalid_vector.size(), invalid_vector);
    if (!result) {
        std::cout << "max_value_check: pass" << std::endl;
    } else {
        std::cout << "max_value_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for check_values.
 * 
 * This function tests if the check_values method correctly identifies missing 
 * values in the circuit vector.
 */
void test_check_values() {
    TestCircuit circuit(3);
    // Vector missing the value 2
    std::vector<int> invalid_vector = {0, 1, 1, 4, 3, 0, 0, 0, 1, 1};
    circuit.set_units_from_vector(invalid_vector);
    bool result = circuit.test_check_values(invalid_vector.size(), invalid_vector);
    if (!result) {
        std::cout << "check_values: pass" << std::endl;
    } else {
        std::cout << "check_values: fail" << std::endl;
    }
}

/**
 * @brief Test function for self_recycle_check.
 * 
 * This function tests if the self_recycle_check method correctly identifies 
 * self-recycling in the circuit vector.
 */
void test_self_recycle_check() {
    TestCircuit circuit(3);
    // Vector with self-recycle
    std::vector<int> vector = {0, 0, 2, 2, 3, 0, 4, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_self_recycle_check();
    if (!result) {
        std::cout << "self_recycle_check: pass" << std::endl;
    } else {
        std::cout << "self_recycle_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for same_unit_dest_check.
 * 
 * This function tests if the same_unit_dest_check method correctly identifies 
 * if all streams point to the same unit.
 */
void test_same_unit_dest_check() {
    TestCircuit circuit(3);
    // Vector where all streams point to the same unit
    std::vector<int> vector = {0, 1, 1, 1, 3, 0, 4, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_same_unit_dest_check();
    if (!result) {
        std::cout << "same_unit_dest_check: pass" << std::endl;
    } else {
        std::cout << "same_unit_dest_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for end_of_vector_check.
 * 
 * This function tests if the end_of_vector_check method correctly identifies 
 * invalid paths in the circuit vector.
 */
void test_end_of_vector_check() {
    TestCircuit circuit(3);
    // Vector with invalid paths
    std::vector<int> vector = {0, -1, 2, 2, 3, 0, 4, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_end_of_vector_check();
    if (!result) {
        std::cout << "end_of_vector_check: pass" << std::endl;
    } else {
        std::cout << "end_of_vector_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for concentrate_tailing_check.
 * 
 * This function tests if the concentrate_tailing_check method correctly identifies 
 * invalid concentrate or tailing streams in the circuit vector.
 */
void test_concentrate_tailing_check() {
    TestCircuit circuit(3);
    // Vector with invalid concentrate or tailing streams
    std::vector<int> vector = {0, 1, 2, 2, 4, 0, 3, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_concentrate_tailing_check();
    if (!result) {
        std::cout << "concentrate_tailing_check: pass" << std::endl;
    } else {
        std::cout << "concentrate_tailing_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for is_reachable.
 * 
 * This function tests if the is_reachable method correctly identifies if all 
 * units are reachable from the feed.
 */
void test_is_reachable() {
    TestCircuit circuit(3);
    // Vector with invalid paths, making some units unreachable (unit 2)
    std::vector<int> vector = {0, 1, 1, 4, 3, 0, 1, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_is_reachable();
    if (!result) {
        std::cout << "is_reachable: pass" << std::endl;
    } else {
        std::cout << "is_reachable: fail" << std::endl;
    }
}

/**
 * @brief Test function for tail_percentage_to_concentrate_outlet_check.
 * 
 * This function tests if the tail_percentage_to_concentrate_outlet_check method 
 * correctly identifies if the tailings percentage to the concentrate outlet is 
 * greater than 50%.
 */
void test_tail_percentage_to_concentrate_outlet_check() {
    TestCircuit circuit(3);
    // Test case where the tailings percentage to the concentrate outlet is greater than 50%
    std::vector<int> vector = {0, 2, 2, 1, 3, 0, 4, 0, 0, 1};  // Tailings to concentrate outlet too high
    circuit.set_units_from_vector(vector);
    bool result = circuit.test_tail_percentage_to_concentrate_outlet_check();
    if (!result) {
        std::cout << "percentage_to_concentrate_outlet_check: pass" << std::endl;
    } else {
        std::cout << "percentage_to_concentrate_outlet_check: fail" << std::endl;
    }
}

/**
 * @brief Test function for check_feed_value.
 * 
 * This function tests if the check_feed_value method correctly identifies if 
 * the feed value is not within the valid range.
 */
void test_check_feed_value() {
    TestCircuit circuit(3);
    // Test case where feed value is not within valid range
    std::vector<int> vector = {10, 1, 2, 2, 3, 0, 4, 0, 1, 1};
    circuit.set_units_from_vector(vector);
    bool result = circuit.check_feed_value(vector.data());
    if (!result) {
        std::cout << "check_feed_value: pass" << std::endl;
    } else {
        std::cout << "check_feed_value: fail" << std::endl;
    }
}

/**
 * Reads test cases from a file and separates them into valid and invalid categories.
 *
 * The input file is expected to have each line in the format:
 * "array_element,bool_element,string_element"
 * where `array_element` is a space-separated list of integers,
 * `bool_element` is either "valid" or "invalid", and `string_element` is a descriptive string.
 *
 * @param filePath The path to the input file.
 * @return A tuple containing:
 *         - A vector of vectors of integers representing valid arrays.
 *         - A vector of strings representing descriptions of valid arrays.
 *         - A vector of vectors of integers representing invalid arrays.
 *         - A vector of strings representing descriptions of invalid arrays.
 */
std::tuple<std::vector<std::vector<int>>, std::vector<std::string>,
           std::vector<std::vector<int>>, std::vector<std::string>>
read_testcases(const std::string& filePath) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return {};
    }

    std::vector<std::vector<int>> validArrayColumn;
    std::vector<std::vector<int>> invalidArrayColumn;
    std::vector<std::string> validStringColumn;
    std::vector<std::string> invalidStringColumn;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string arrayElement, boolElement, stringElement;

        std::getline(ss, arrayElement, ',');
        std::getline(ss, boolElement, ',');
        std::getline(ss, stringElement, ',');
        // to parse arrayElement into an int array
        std::vector<int> intArray;
        std::stringstream arrayStream(arrayElement);
        std::string number;
        while (std::getline(arrayStream, number, ' ')) {
            intArray.push_back(std::stoi(number));
        }

        bool isValid = (boolElement == "valid");
        if (isValid) {
            validArrayColumn.push_back(intArray);
            validStringColumn.push_back(stringElement);
        } else {
            invalidArrayColumn.push_back(intArray);
            invalidStringColumn.push_back(stringElement);
        }
    }

    file.close();
    return {validArrayColumn, validStringColumn, invalidArrayColumn, invalidStringColumn};
}

/**
 * Performs an extra validation check on invalid test cases.
 *
 * Uses a TestCircuit instance to run tests on each invalid array.
 * Outputs the result of each test and the reasons for failure if applicable.
 *
 * @param invalidArrayColumn A vector of vectors of integers representing invalid arrays.
 * @param invalidStringColumn A vector of strings representing descriptions of invalid arrays.
 * @return A boolean indicating whether all invalid arrays passed the extra invalid check (which should ideally be false).
 */
bool extra_invalid_check(std::vector<std::vector<int>>& invalidArrayColumn, std::vector<std::string>& invalidStringColumn) {
    TestCircuit circuit(3);
    bool allPass = true;
    for (size_t i = 0; i < invalidArrayColumn.size(); ++i) {
        bool allPass = true;
        std::vector<std::string> fails;
        circuit.set_units_from_vector(invalidArrayColumn[i]);
        bool result = circuit.test_all(invalidArrayColumn[i].size(), invalidArrayColumn[i].data(), fails);
        std::string fail_reasons = std::accumulate(fails.begin(), fails.end(), std::string(),
                [](const std::string& a, const std::string& b) {
                    return a.empty() ? b : a + ", " + b;
                });
        if (result) {
            std::cout << "[" << invalidStringColumn[i] <<"]: extra_invalid_check: fail - ";
            allPass = false;
        }else{
            std::cout << "["<<invalidStringColumn[i] <<"]: extra_invalid_check: pass - ";
        }
        std::cout << "Found invalid: "<< fail_reasons << std::endl;
    }
    return allPass;
}

/**
 * Performs an extra validation check on valid test cases.
 *
 * Uses a TestCircuit instance to run tests on each valid array.
 * Outputs the result of each test and the reasons for failure if applicable.
 *
 * @param validArrayColumn A vector of vectors of integers representing valid arrays.
 * @param validStringColumn A vector of strings representing descriptions of valid arrays.
 * @return A boolean indicating whether all valid arrays passed the extra valid check.
 */
bool extra_valid_check(std::vector<std::vector<int>>& validArrayColumn, std::vector<std::string>& validStringColumn) {
    TestCircuit circuit(3);
    bool allPass = true;
    for (size_t i = 0; i < validArrayColumn.size(); ++i) {
        std::vector<std::string> fails;
        circuit.set_units_from_vector(validArrayColumn[i]);
        bool result = circuit.test_all(validArrayColumn[i].size(), validArrayColumn[i].data(), fails);
        std::string fail_reasons = std::accumulate(fails.begin(), fails.end(), std::string(),
                [](const std::string& a, const std::string& b) {
                    return a.empty() ? b : a + ", " + b;
                });
        if (!result) {
            std::cout << "[" << validStringColumn[i] << "]: extra_valid_check: fail - ";
            std::cout << "Found invalid: " << fail_reasons << std::endl;
            allPass = false;
        } else {
            std::cout << "[" <<validStringColumn[i] << "]: extra_valid_check: pass" << std::endl;
        }
    }
    return allPass;
}


/**
 * @brief Main function to run all test cases.
 * 
 * This function runs all the test cases for the Circuit class to validate 
 * the different checks implemented.
 */
int main() {
    // Basic unit tests
    std::cout << "------Running basic unit tests------" << std::endl;
    test_max_value_check();
    test_check_values();
    test_self_recycle_check();
    test_same_unit_dest_check();
    test_end_of_vector_check();
    test_concentrate_tailing_check();
    test_is_reachable();
    test_tail_percentage_to_concentrate_outlet_check();
    test_check_feed_value();
    // Extra tests
    std::cout << "------Running extra tests------" << std::endl;
    auto [validArrayColumn, validStringColumn, invalidArrayColumn, invalidStringColumn] = read_testcases("../testcases/test_validity.csv");
    bool allInvalidTestsPass = extra_invalid_check(invalidArrayColumn, invalidStringColumn);
    bool allValidTestsPass = extra_valid_check(validArrayColumn, validStringColumn);
    if (allInvalidTestsPass && allValidTestsPass) {
        std::cout << "All extra tests passed!" << std::endl;
    } else {
        std::cout << "Some extra tests failed!" << std::endl;
    }
    return 0;
}
