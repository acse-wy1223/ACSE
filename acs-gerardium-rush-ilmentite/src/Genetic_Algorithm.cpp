#include <stdio.h>
#include <cmath>
#include <array>
#include <vector>
#include <iostream>
#include <random>
#include <numeric>
#include <limits>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <omp.h>
#include "Genetic_Algorithm.h"

namespace fs = std::filesystem;

int number_of_units = 1;

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60


/**
 * Prints the current progress of an operation to the console.
 * 
 * @param percentage The completion percentage of the operation.
 * @param performance A floating point value indicating current performance metrics.
 */
void printProgress(double percentage, double performance) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s] Item %.2f", val, lpad, PBSTR, rpad, "", performance);
    fflush(stdout);
}


/**
 * Finds the maximum value in a double array.
 * 
 * @param array Pointer to the first element of the double array.
 * @param size Size of the array.
 * @return The maximum value found in the array.
 */
double find_max_double(const double* array, int size) {
    double max_value = array[0];
    for (int i = 1; i < size; ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }
    return max_value;
}


/**
 * Generates a random number within a specified range.
 * 
 * @param min The lower bound of the range.
 * @param max The upper bound of the range.
 * @return A randomly generated number within the specified range.
 */
double generate_random_number(double min, double max) {
    static thread_local std::mt19937 generator(omp_get_thread_num());
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}


/**
 * Initializes a population for the genetic algorithm.
 * 
 * @param population_size The size of the population to initialize.
 * @param vector_size The size of each individual in the population.
 * @param initial_vector Initial values for the first individual in the population.
 * @param validity A function that checks the validity of an individual.
 * @param elitism_rate The rate of elitism to apply during evolution.
 * @return A vector of vectors containing the initialized population.
 */
std::vector<std::vector<int>> initialize_population(int population_size, int vector_size, const int* initial_vector, std::function<bool(int, int*)> validity, double elitism_rate) {
    std::vector<std::vector<int>> population;
    population.reserve(population_size);  // Reserve space to avoid reallocations

    std::vector<int> start_vector(initial_vector, initial_vector + vector_size);
    population.push_back(start_vector);

    number_of_units = *std::max_element(initial_vector, initial_vector + vector_size) + 1;

    #pragma omp parallel
    {
        std::random_device rd;
        std::mt19937 gen(rd() + omp_get_thread_num());  // Unique seed for each thread
        std::uniform_int_distribution<> distr(0, number_of_units - 1);

        #pragma omp for
        for (int i = 1; i < population_size; ++i) {
            std::vector<int> individual(vector_size);
            for (int j = 0; j < vector_size; ++j) {
                individual[j] = distr(gen);

                bool valid = true;
                if (j == 0) {
                    if (individual[j] == number_of_units - 2 || individual[j] == number_of_units - 3) {
                        valid = false;
                    }
                } else if ((j - 1) / 3 == individual[j]) {
                    valid = false;
                }

                if (!valid) {
                    j--;
                }
            }

            #pragma omp critical
            {
                if (validity(vector_size, individual.data()) || population.size() < population_size * 0.8) {
                    population.push_back(individual);
                } else {
                    --i;  // Retry this iteration
                }
            }
        }
    }

    return population;
}


/**
 * Selects individuals from the population based on their fitness.
 * 
 * @param population A reference to the current population.
 * @param fitness A vector containing fitness scores for each individual.
 * @return A vector of vectors containing the selected individuals.
 */
std::vector<std::vector<int>> select(const std::vector<std::vector<int>>& population, const std::vector<double>& fitness) {
    std::vector<std::vector<int>> selected;
    double total_fitness = std::accumulate(fitness.begin(), fitness.end(), 0.0);
    std::vector<double> probabilities;

    std::transform(fitness.begin(), fitness.end(), probabilities.begin(), [total_fitness](double f) {
        return f / total_fitness;
    });

    std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());
    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t i = 0; i < population.size(); ++i) {
        selected.push_back(population[distribution(gen)]);
    }

    return selected;
}


/**
 * Applies a non-uniform mutation to an individual in the population. Mutation depends on the current generation,
 * allowing for finer mutations as the number of generations increases.
 * 
 * @param individual A reference to the individual (vector of ints) to mutate.
 * @param mutation_rate The mutation rate to apply.
 * @param max_value The maximum value for any gene in the individual.
 * @param currentGeneration The current generation number in the genetic algorithm.
 * @param maxGenerations The maximum number of generations expected to run.
 */
void NonUniform_Mutation(std::vector<int>& individual, double mutation_rate, int max_value, int currentGeneration, int maxGenerations) {

    for (int& gene : individual) {
        if (generate_random_number(0.0, 1.0) < mutation_rate) {
            double delta = (generate_random_number(0.0, 1.0) < 0.5) ? gene : max_value - gene;
            double b = 5;
            double r = generate_random_number(0.0, 1.0);

            double change = delta * (1 - pow(r, pow((1 - double(currentGeneration) / maxGenerations), b)));
            gene = (generate_random_number(0.0, 1.0) < 0.5) ? gene - static_cast<int>(change) : gene + static_cast<int>(change);

            if (gene < 0) gene = 0;
            if (gene > max_value) gene = max_value;
        }
    }
}


/**
 * Mutates a given vector with a specified mutation rate. Each element in the vector has a chance to be changed
 * based on the mutation rate.
 * 
 * @param vector The vector to mutate.
 * @param mutation_rate The probability of mutating each element of the vector.
 * @param max_unit The maximum value any element in the vector can take.
 */
void mutate_vector(std::vector<int>& vector, double mutation_rate, int max_unit) {
    
    for (int& value : vector) {
        if (generate_random_number(0.0, 1.0) < mutation_rate) {
            value = (value + static_cast<int>(generate_random_number(0, max_unit))) % (max_unit + 1);
        }
    }
}


/**
 * Performs a single-point crossover between two parent vectors.
 * 
 * @param parent1 The first parent vector.
 * @param parent2 The second parent vector.
 * @param crossover_rate The probability of performing a crossover.
 * @param max_value The maximum value for any gene in the vectors.
 */
void crossover(std::vector<int>& parent1, std::vector<int>& parent2, double crossover_rate, int max_value) {

    if (generate_random_number(0.0, 1.0) < crossover_rate) {
        std::uniform_int_distribution<> point_dist(1, parent1.size() - 2);
        int crossover_point = static_cast<int>(generate_random_number(1, parent1.size() - 2));

        for (int i = crossover_point; i < parent1.size(); ++i) {
            std::swap(parent1[i], parent2[i]);
        }
    }
}


/**
 * Selects an index for roulette wheel selection based on cumulative fitness scores.
 * 
 * @param cumulative_fitness A vector of cumulative fitness scores.
 * @return The selected index based on the random choice in the cumulative distribution.
 */
int select_index(const std::vector<double>& cumulative_fitness) {
    double rnd = generate_random_number(0.0, cumulative_fitness.back());
    return std::lower_bound(cumulative_fitness.begin(), cumulative_fitness.end(), rnd) - cumulative_fitness.begin();
}


/**
 * Regenerates the population by introducing new random vectors to replace the less fit individuals,
 * aiming to introduce diversity and prevent premature convergence.
 * 
 * @param population The population of vectors.
 * @param vector_size The size of each vector.
 * @param number_of_units The maximum value for any gene in the vectors.
 */
void regenerate_population(std::vector<std::vector<int>>& population, int vector_size, int number_of_units) {
    #pragma omp parallel for
    for (int i = (int) (population.size() * 0.2); i < population.size(); ++i) {  // Start from 1 to keep the first vector unchanged
        std::random_device rd;
        std::mt19937 gen(rd() + omp_get_thread_num()); // Ensuring unique seed per thread
        std::uniform_int_distribution<> distr(0, number_of_units - 1);

        for (int j = 0; j < vector_size; ++j) {
            population[i][j] = distr(gen);
        }
    }
}


/**
 * Conducts the entire genetic algorithm process, managing the population through multiple generations
 * and applying genetic operations like selection, crossover, and mutation to evolve solutions.
 * 
 * @param population The initial population of solutions.
 * @param func A function pointer to the fitness evaluation function.
 * @param validity A function to check the validity of individual solutions.
 * @param parameters Struct containing parameters for the genetic algorithm.
 * @return The maximum fitness achieved by the best solution in the population.
 */
double genetic_algorithm(std::vector<std::vector<int>>& population, double (&func) (int, int*),
                         std::function<bool(int, int*)> validity,
                         const Algorithm_Parameters& parameters) {
    int population_size = population.size();
    int vector_size = population[0].size();
    std::vector<double> fitness(population_size);
    double max_fitness = std::numeric_limits<double>::lowest();
    int fitness_unchanged_count = 0;

    int elitism_count = static_cast<int>(population.size() * parameters.elitism_rate);

    for (int generation = 0; generation < parameters.max_iterations; ++generation) {
        // Evaluate fitness for each vector in the population
        #pragma omp parallel for
        for (int i = 0; i < population_size; ++i) {
            if (validity(vector_size, population[i].data())) {
                fitness[i] = func(vector_size, population[i].data());
            } else {
                fitness[i] = std::numeric_limits<double>::lowest();
            }
        }

        // Sort the population based on fitness
        std::vector<int> idx(population_size);
        std::iota(idx.begin(), idx.end(), 0);
        std::sort(idx.begin(), idx.end(), [&](int i1, int i2) { return fitness[i1] > fitness[i2]; });

        printProgress((double)generation / (parameters.max_iterations - 1), fitness[idx[0]]);

        // Implement elitism, save the best individuals
        std::vector<std::vector<int>> new_population;
        for (int i = 0; i < elitism_count; ++i) {
            new_population.push_back(population[idx[i]]);
        }

        // Create a cumulative fitness sum for roulette wheel selection
        std::vector<double> cumulative_fitness(population_size);
        std::partial_sum(fitness.begin(), fitness.end(), cumulative_fitness.begin());

        // #pragma omp parallel for
        for (int i = elitism_count; i < population.size(); i+=2) {
            std::vector<int> parent1 = population[select_index(cumulative_fitness)];
            std::vector<int> parent2 = population[select_index(cumulative_fitness)];

            crossover(parent1, parent2, parameters.crossover_rate, number_of_units);
            crossover(parent1, parent2, parameters.crossover_rate, number_of_units);

            // if (vector_size > 100) {
            //     while (!validity(vector_size, parent1.data()) && !validity(vector_size, parent2.data())) {
            //         crossover(parent1, parent2, parameters.crossover_rate, number_of_units);
            //     }
            // }

            NonUniform_Mutation(parent1, parameters.mutation_rate, number_of_units, generation, parameters.max_iterations);
            NonUniform_Mutation(parent2, parameters.mutation_rate, number_of_units, generation, parameters.max_iterations);
            double mutator = 0.0;

            if (fitness_unchanged_count > (parameters.max_iterations * 0.1)) {
                mutator = parameters.mutation_rate + (fitness_unchanged_count * 0.001);
                mutator = mutator < 0.5 ? mutator : 0.5;
            } else {

                mutator = parameters.mutation_rate;
            }
            mutate_vector(parent1, mutator, number_of_units);
            mutate_vector(parent2, mutator, number_of_units);

            new_population.push_back(parent1);
            if (new_population.size() < population_size) {
                new_population.push_back(parent2);
            }
        }

        double temp_fitness = find_max_double(fitness.data(), fitness.size());
        if (temp_fitness - max_fitness < 0.1) {
            fitness_unchanged_count++;
        }
        else{
            fitness_unchanged_count = 0;
        }
        if (temp_fitness > max_fitness) {
          fs::path dir("./output");
          if (!fs::exists(dir)) {
              fs::create_directories(dir);
          }
          std::ofstream vector_file("./output/vector.dat");
          if (vector_file.is_open()) {
              for (int i = 0; i < vector_size; i++) {
                  vector_file << population[0][i] << " ";
              }
              vector_file.close();
          }

        }
        population = new_population;
        if (fitness_unchanged_count > 50) {
            regenerate_population(population, vector_size, number_of_units);
            fitness_unchanged_count = 0;
        }
        #pragma omp barrier
        max_fitness = *std::max_element(fitness.begin(), fitness.end());
    }
    std::cout << std::endl;
    return max_fitness;
}


/**
 * Optimizes a vector using genetic algorithm principles. Initializes a population, runs the genetic algorithm,
 * and stores the best solution back into the original vector.
 * 
 * @param vector_size The size of the vector to be optimized.
 * @param vector The vector containing initial values, modified in-place to store the best solution found.
 * @param func A function pointer to the fitness evaluation function.
 * @param validity A function to check the validity of individual solutions.
 * @param parameters Struct containing parameters for the genetic algorithm.
 * @return Returns 0 on successful execution and optimization, -1 if file operation fails.
 */
int optimize(int vector_size, int *vector,
             double (&func) (int, int*),
             std::function<bool(int, int*)> validity,
             struct Algorithm_Parameters parameters) {
    // print the number of threads
    std::cout << "Number of threads: " << omp_get_max_threads() << std::endl;

    int unit_num = (vector_size - 1) / 3;

    for (int i = 0; i <= unit_num+1; ++i){
        vector[i] = i;
    }
    for (int i = unit_num+2; i < vector_size; ++i){
        vector[i] = 0;
    }

    std::vector<std::vector<int>> population = initialize_population(parameters.initial_pop, vector_size, vector, validity, parameters.elitism_rate);
    double max_fitness = genetic_algorithm(population, func, validity, parameters);
    std::copy(population[0].begin(), population[0].end(), vector);

    std::ofstream vector_file("./output/vector.dat");
    if (vector_file.is_open()) {
        for (int i = 0; i < vector_size; i++) {
            vector_file << vector[i] << " ";
        }
        vector_file.close();
    } else {
        return -1;
    }

    return 0;
}
