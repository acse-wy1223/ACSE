/** @file Circuit_Simulator.h
 *  @brief Header file for the circuit simulator.
 *
 *  This header file defines the function and structures that will be used to evaluate the circuit.
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "CUnit.h"

#pragma once

/**
 * @struct Circuit_Parameters
 * @brief Parameters for the circuit simulator.
 */
struct Circuit_Parameters{
    double tolerance = 0.1;          /**< Tolerance for the simulation convergence */
    int max_iterations = 1000;       /**< Maximum number of iterations */
    // other parameters for your circuit simulator       
};

/**
 * @struct Economic_parameters
 * @brief Economic parameters for evaluating the circuit performance.
 */
struct Economic_parameters{
    double price = 100.0;            /**< Price of the concentrate */
    double penalty = -750;           /**< Penalty for failing to meet requirements */
};

/**
 * @struct Initial_flow
 * @brief Initial flow rates for the simulation.
 */
struct Initial_flow{
    double init_Fg = 10;             /**< Initial flow rate of gerardium */
    double init_Fw = 90;             /**< Initial flow rate of waste */
};

/**
 * @struct Calculate_constants
 * @brief Constants used in the calculation of the circuit.
 */
struct Calculate_constants{
    double rho = 3000.0;             /**< Density */
    double phi = 0.1;                /**< Solids Volume Fraction */
    double V = 10.0;                 /**< Volume */
    double k_concentrate_gerardium = 0.004; /**< Rate constant for concentrate gerardium */
    double k_inter_gerardium = 0.001;       /**< Rate constant for intermediate gerardium */
    double k_concentrate_waste = 0.0002;    /**< Rate constant for concentrate waste */
    double k_inter_waste = 0.0003;          /**< Rate constant for intermediate waste */
};

/**
 * @struct Recovery
 * @brief Recovery rates of the materials.
 */
struct Recovery{
    double concentrate_gerardium;    /**< Recovery rate of concentrate gerardium */
    double concentrate_waste;        /**< Recovery rate of concentrate waste */
    double inter_gerardium;          /**< Recovery rate of intermediate gerardium */
    double inter_waste;              /**< Recovery rate of intermediate waste */
};

/**
 * @brief Evaluates the circuit performance.
 *
 * @param vector_size The size of the circuit vector.
 * @param circuit_vector The circuit vector.
 * @return The performance value.
 */
double Evaluate_Circuit(int vector_size, int *circuit_vector);

/**
 * @brief Calculates the residence time.
 *
 * @param constants The constants used in the calculation.
 * @param Fg Flow rate of gerardium.
 * @param Fw Flow rate of waste.
 * @return The residence time.
 */
double calculate_residence_time(const Calculate_constants& constants, double Fg, double Fw);

/**
 * @brief Calculates the recovery rates.
 *
 * @param constants The constants used in the calculation.
 * @param tau The residence time.
 * @return The recovery rates.
 */
struct Recovery calculate_recovery(const Calculate_constants& constants, double tau);

/**
 * @brief Calculates the flow rates.
 *
 * @param constants The constants used in the calculation.
 * @param recovery The recovery rates.
 * @param init_Fg Initial flow rate of gerardium.
 * @param init_Fw Initial flow rate of waste.
 * @return A vector containing the flow rates.
 */
std::vector<double> calculate_flow_rate(const Calculate_constants& constants, Recovery& recovery, double init_Fg, double init_Fw);

/**
 * @brief Gets the performance of the circuit.
 *
 * @param cg Concentrate gerardium.
 * @param cw Concentrate waste.
 * @param eco The economic parameters.
 * @return The performance value.
 */
double get_performance(double cg, double cw, const Economic_parameters &eco);

/**
 * @brief Converts a vector to a vector of CUnit objects.
 *
 * @param vector The input vector.
 * @param size The size of the input vector.
 * @param init_flow The initial flow rates.
 * @return A vector of CUnit objects.
 */
std::vector<CUnit> vector_to_units(int* vector, int size, const Initial_flow &init_flow);
