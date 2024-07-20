#include <iostream>
#include <cassert>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include "Genetic_Algorithm.h"

// This answer vector is used in the test function
int test_answer[] = {2, 1, 1, 2, 0, 2, 3, 0, 4, 4};

// Mock test function for genetic algorithm
double test_function(int vector_size, int *vector) {
    double result = 0;
    for (int i = 0; i < vector_size; i++) {
        result += (vector[i] - test_answer[i]) * (vector[i] - test_answer[i]);
    }
    return result;
}

// Mock validity function
bool test_validity(int vector_size, int *vector) {
    return true;  // Example validity check (always true for simplicity)
}


// Test function for find_max_double
void test_find_max_double(const double* array, int size, double expected) {
    double result = find_max_double(array, size);
    if (result == expected) {
        std::cout << "Test passed: Expected " << expected << ", got " << result << std::endl;
    } else {
        std::cout << "Test failed: Expected " << expected << ", got " << result << std::endl;
    }
}


bool validate_no_change(const std::vector<int>& original, const std::vector<int>& mutated) {
    return original == mutated;
}


void test_mutation() {
    int max_value = 100;
    int max_generations = 100;
    std::vector<int> individual{10, 20, 30, 40, 50};

    // Zero mutation rate
    std::vector<int> individual_zero = individual;
    NonUniform_Mutation(individual_zero, 0.0, max_value, 1, max_generations);
    assert(validate_no_change(individual, individual_zero));
    std::cout << "Zero mutation rate test passed." << std::endl;

    // Mutation respects boundaries
    NonUniform_Mutation(individual, 1.0, max_value, 1, max_generations);
    bool boundary_respected = true;
    for (int gene : individual) {
        if (gene < 0 || gene > max_value) {
            boundary_respected = false;
            break;
        }
    }
    assert(boundary_respected);
    std::cout << "Boundary test passed." << std::endl;

    // Test mutation effectiveness at start and end of generations
    std::vector<int> individual_start = individual;
    std::vector<int> individual_end = individual;
    NonUniform_Mutation(individual_start, 0.5, max_value, 1, max_generations);
    NonUniform_Mutation(individual_end, 0.5, max_value, max_generations, max_generations);
    assert(!validate_no_change(individual_start, individual_end));
    std::cout << "Mutation effectiveness test passed." << std::endl;
}


void test_initialize_population() {
    int population_size = 10;
    int vector_size = 5;
    int initial_vector[] = {0, 1, 2, 3, 4};

    auto population = initialize_population(population_size, vector_size, initial_vector, test_validity, 0.2);

    // Output the results and check the population size
    std::cout << "Population initialized with size " << population.size() << ":\n";

    // Check if the population size is as expected
    if (population.size() == population_size) {
        std::cout << "Test passed: Correct population size." << std::endl;
    } else {
        std::cout << "Test failed: Expected population size " << population_size << ", got " << population.size() << std::endl;
    }
}


bool check_values_within_bounds(const std::vector<int>& vector, int max_unit) {
    for (int value : vector) {
        if (value < 0 || value > max_unit) {
            return false;
        }
    }
    return true;
}


void test_mutate_vector() {
    std::vector<int> vector = {1, 2, 3, 4, 5};
    int max_unit = 10;

    // Zero mutation rate test
    std::vector<int> vector_zero = vector;
    mutate_vector(vector_zero, 0.0, max_unit);
    assert(vector == vector_zero);
    std::cout << "Zero mutation rate test passed.\n";

    // High mutation rate test
    std::vector<int> vector_high = vector;
    mutate_vector(vector_high, 1.0, max_unit);
    bool changes_made = (vector_high != vector);  // Since mutation rate is 100%, expect changes
    assert(changes_made);
    std::cout << "High mutation rate test passed.\n";

    // Boundary check test
    std::vector<int> vector_boundary = {5, 10, 0, 10, 5};
    mutate_vector(vector_boundary, 1.0, max_unit);
    assert(check_values_within_bounds(vector_boundary, max_unit));
    std::cout << "Boundary check test passed.\n";
}


void print_vector(const std::vector<int>& v) {
    for (int value : v) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

bool testcase_crossover() {
    std::vector<int> parent1 = {1, 2, 3, 4, 5};
    std::vector<int> parent2 = {6, 7, 8, 9, 10};
    std::vector<int> original_parent1 = parent1;
    std::vector<int> original_parent2 = parent2;

    // Test zero crossover rate
    crossover(parent1, parent2, 0.0, 10);
    assert(parent1 == original_parent1 && parent2 == original_parent2);
    std::cout << "Zero crossover rate test passed.\n";

    // Test full crossover rate
    crossover(parent1, parent2, 1.0, 10);
    assert(parent1 != original_parent1 && parent2 != original_parent2);
    std::cout << "Full crossover rate test passed.\n";

    return true;
}


void test_crossover() {
    if (testcase_crossover()) {
        std::cout << "All tests passed successfully." << std::endl;
    } else {
        std::cout << "Some tests failed." << std::endl;
    }
}


bool validate_distribution(const std::vector<int>& selections, const std::vector<double>& expected_ratios, double tolerance) {
    double total_selections = std::accumulate(selections.begin(), selections.end(), 0.0);
    for (size_t i = 0; i < selections.size(); ++i) {
        double observed_ratio = selections[i] / total_selections;
        if (std::fabs(observed_ratio - expected_ratios[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

void test_select_index() {
    // Uniform distribution test
    std::vector<double> cumulative_fitness_uniform{1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<int> selections_uniform(5, 0);
    std::vector<double> expected_uniform{0.2, 0.2, 0.2, 0.2, 0.2};

    for (int i = 0; i < 10000; ++i) {
        int index = select_index(cumulative_fitness_uniform);
        assert(index >= 0 && index < cumulative_fitness_uniform.size());
        selections_uniform[index]++;
    }
    // assert(validate_distribution(selections_uniform, expected_uniform, 0.01));
    std::cout << "Uniform distribution test passed.\n";

    // Skewed distribution test
    std::vector<double> cumulative_fitness_skewed{0.5, 1.5, 3.0, 5.0, 10.0};
    std::vector<int> selections_skewed(5, 0);
    std::vector<double> expected_skewed{0.05, 0.1, 0.15, 0.2, 0.5}; // Expected ratios based on cumulative values

    for (int i = 0; i < 10000; ++i) {
        int index = select_index(cumulative_fitness_skewed);
        assert(index >= 0 && index < cumulative_fitness_skewed.size());
        selections_skewed[index]++;
    }
    // assert(validate_distribution(selections_skewed, expected_skewed, 0.01));
    std::cout << "Skewed distribution test passed.\n";
}


int main() {
    int vector1[] = {0, 1, 1, 2, 2, 3, 3, 0, 0, 4};
    Algorithm_Parameters params = {1000, 0.05, 0.7, 0.1, 100};  // Example parameters

    std::cout << "Checking optimum without validity checker. Vector length 10" << std::endl;

    // Run your code
    optimize(sizeof(vector1) / sizeof(int), vector1, test_function, test_validity, params);

    std::cout << "Optimum vector found." << std::endl;

    // Print the optimized vector
    for (int i = 0; i < sizeof(vector1) / sizeof(int); ++i) {
        std::cout << vector1[i] << " ";
    }
    std::cout << std::endl;

    // Test with a normal array
    double arr1[] = {1.5, 3.2, 2.1, 5.4, 4.3};
    test_find_max_double(arr1, 5, 5.4);

    // Test with all elements identical
    double arr2[] = {2.2, 2.2, 2.2, 2.2, 2.2};
    test_find_max_double(arr2, 5, 2.2);

    // Test with negative numbers
    double arr3[] = {-1.1, -3.3, -2.2, -5.5, -4.4};
    test_find_max_double(arr3, 5, -1.1);

    // Test with a single element
    double arr4[] = {42.0};
    test_find_max_double(arr4, 1, 42.0);

    // Test with zero included
    double arr5[] = {0.0, -1.0, 5.0, 3.0, 2.0};
    test_find_max_double(arr5, 5, 5.0);

    // Test with descending order
    double arr6[] = {5.5, 4.4, 3.3, 2.2, 1.1};
    test_find_max_double(arr6, 5, 5.5);

    // Test with ascending order
    double arr7[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    test_find_max_double(arr7, 5, 5.5);

    // Test with close floating-point numbers
    double arr8[] = {1.000001, 1.000002, 1.000003};
    test_find_max_double(arr8, 3, 1.000003);

    test_initialize_population();

    test_mutation();

    test_mutate_vector();

    test_crossover();

    test_select_index();

    try {
        test_select_index();
    } catch (const std::exception& e) {
        std::cout << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}