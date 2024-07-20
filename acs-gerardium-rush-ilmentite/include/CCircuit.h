/**
 * @file Circuit.h
 * @brief Header for the circuit struct.
 *
 * This header defines the circuit struct and its associated functions.
 */

#pragma once

#include "CUnit.h"
#include <vector>
#include <stack>

/**
 * @brief Checks the validity of a given circuit vector.
 *
 * @param vector_size The size of the circuit vector.
 * @param circuit_vector The circuit vector.
 * @return True if the circuit vector is valid, false otherwise.
 */
bool Check_Validity(int vector_size, int *circuit_vector);

/**
 * @brief Represents a circuit of units.
 */
struct Circuit
{
    /**
     * @brief Constructs a Circuit with a given number of units.
     *
     * @param num_units The number of units in the circuit.
     */
    Circuit(int num_units);

    /**
     * @brief Checks the validity of a given circuit vector.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector The circuit vector.
     * @return True if the circuit vector is valid, false otherwise.
     */
    bool Check_Validity(int vector_size, int *circuit_vector);

    /**
     * @brief Checks if all units are reachable.
     *
     * @return True if all units are reachable, false otherwise.
     */
    bool is_reachable();

    /**
     * @brief Checks if the concentrate and tailing outlets are properly defined.
     *
     * @return True if the outlets are properly defined, false otherwise.
     */
    bool concentrate_tailing_check();

    /**
     * @brief Checks for self-recycling units.
     *
     * @return True if no self-recycling units are found, false otherwise.
     */
    bool self_recycle_check();

    /**
     * @brief Checks if the same unit has different destinations.
     *
     * @return True if no unit has different destinations, false otherwise.
     */
    bool same_unit_dest_check();

    /**
     * @brief Checks if the values in the circuit vector are valid.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector The circuit vector.
     * @return True if the values are valid, false otherwise.
     */
    bool check_values(int vector_size, int *circuit_vector);

    /**
     * @brief Checks if the end of the vector is reached correctly.
     *
     * @return True if the end of the vector is reached correctly, false otherwise.
     */
    bool end_of_vector_check();

    /**
     * @brief Checks if the maximum values in the circuit vector are valid.
     *
     * @param vector_size The size of the circuit vector.
     * @param circuit_vector The circuit vector.
     * @return True if the maximum values are valid, false otherwise.
     */
    bool max_value_check(int vector_size, int *circuit_vector);

    /**
     * @brief Checks if the tail percentage to the concentrate outlet is within limits.
     *
     * @return True if the tail percentage is within limits, false otherwise.
     */
    bool tail_percentage_to_concentrate_outlet_check();

    /**
     * @brief Checks the feed value in the circuit vector.
     *
     * @param circuit_vector The circuit vector.
     * @return True if the feed value is valid, false otherwise.
     */
    bool check_feed_value(int *circuit_vector);

    std::vector<CUnit> units; /**< Vector of units in the circuit. */

private:
    /**
     * @brief Marks units for reachability check.
     *
     * @param unit_num The unit number to start marking from.
     */
    void mark_units(int unit_num);
};
