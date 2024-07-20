/**
 * @file Genetic_Algorithm.h
 * @brief Header for the genetic algorithm and related functions.
 *
 * This header defines the genetic algorithm and its related functions and structures.
 */

#pragma once

#include <vector>
#include <functional>  // Include this for std::function

/**
 * @struct Algorithm_Parameters
 * @brief Parameters for the genetic algorithm.
 */
struct Algorithm_Parameters {
    int max_iterations;  ///< Maximum number of iterations.
    double crossover_rate;  ///< Population crossover rate.
    double mutation_rate;   ///< Population mutation rate.
    double elitism_rate;    ///< Population elitism rate.
    double initial_pop;     ///< Initial population size.
    // other parameters for your algorithm
};

/**
 * @def DEFAULT_ALGORITHM_PARAMETERS
 * @brief Default parameters for the genetic algorithm.
 */
#define DEFAULT_ALGORITHM_PARAMETERS Algorithm_Parameters{1000, 0.8, 0.1, 0.1, 100}

/**
 * @brief Checks if all elements in the vector are true.
 * 
 * @param vector_size Size of the vector.
 * @param vector Pointer to the vector.
 * @return True if all elements are true, false otherwise.
 */
bool all_true(int vector_size, int *vector);

/**
 * @brief Performs a genetic algorithm optimization.
 * 
 * @param population The population of solutions.
 * @param func The objective function.
 * @param validity The validity function.
 * @param parameters The parameters for the genetic algorithm.
 * @return The best performance value found.
 */
double genetic_algorithm(std::vector<std::vector<int>> &population, 
                         double (&func)(int, int *),
                         std::function<bool(int, int *)> validity,
                         const Algorithm_Parameters &parameters);

/**
 * @brief Optimizes a vector using the genetic algorithm.
 * 
 * @param vector_size Size of the vector.
 * @param vector Pointer to the vector.
 * @param func The objective function.
 * @param validity The validity function.
 * @param parameters The parameters for the genetic algorithm.
 * @return The index of the best solution found.
 */
int optimize(int vector_size, int *vector,
             double (&func)(int, int *),
             std::function<bool(int, int *)> validity,
             struct Algorithm_Parameters parameters = DEFAULT_ALGORITHM_PARAMETERS);

double find_max_double(const double *array, int size);

std::vector<std::vector<int>> initialize_population(int population_size, int vector_size, const int* initial_vector, std::function<bool(int, int*)> validity, double elitism_rate);

void NonUniform_Mutation(std::vector<int>& individual, double mutation_rate, int max_value, int currentGeneration, int maxGenerations);

void mutate_vector(std::vector<int>& vector, double mutation_rate, int max_unit);

void crossover(std::vector<int>& parent1, std::vector<int>& parent2, double crossover_rate, int max_value);

int select_index(const std::vector<double>& cumulative_fitness);

