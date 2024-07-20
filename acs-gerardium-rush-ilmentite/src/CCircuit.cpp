
#include <vector>
#include <stdio.h>
#include <CUnit.h>
#include <CCircuit.h>
#include <iostream>

using namespace std;

/**
 * @brief Check the validity of the circuit based on given criteria.
 * 
 * This function creates an instance of the Circuit class and uses it to 
 * validate the given circuit configuration.
 * 
 * @param vector_size The size of the input vector.
 * @param circuit_vector The input vector representing the circuit configuration.
 * @return true if the circuit is valid, false otherwise.
 */
bool Check_Validity(int vector_size, int *circuit_vector){
    Circuit circuitInstance(1);
    return circuitInstance.Check_Validity(vector_size, circuit_vector);
}

/**
 * @brief Construct a new Circuit object with the specified number of units.
 * 
 * @param num_units The number of units in the circuit.
 */
Circuit::Circuit(int num_units) {
    // Initialize the circuit with a specified number of units.
    this->units.resize(num_units);
}

/**
 * @brief Check the validity of the circuit based on given criteria.
 * 
 * @param vector_size The size of the input vector.
 * @param circuit_vector The input vector representing the circuit configuration.
 * @return true if the circuit is valid, false otherwise.
 */
bool Circuit::Check_Validity(int vector_size, int *circuit_vector) {
    // Calculate the size of the vector in terms of number of elements.
    int unit_size = 0;

    // Determine the number of units and resize the units vector accordingly.
    if ((vector_size - 1) % 3 == 0) {
        this->units.resize(vector_size / 3);
        unit_size = vector_size / 3;
    } else {
        this->units.resize(vector_size / 3 + 1);
        unit_size = vector_size / 3 + 1;
    }

    // Assign the values from the circuit_vector to the respective units.
    for (int i = 0; i < unit_size; ++i) {
        this->units[i].conc_num = circuit_vector[3 * i + 1];
        if (vector_size > 3 * i + 2)
            this->units[i].inter_num = circuit_vector[3 * i + 2];
        if (vector_size > 3 * i + 3)
            this->units[i].tails_num = circuit_vector[3 * i + 3];
        this->units[i].mark = false;
    }

    // Check if all required values are present in the circuit vector.
    if (!this->check_values(vector_size, circuit_vector)) {
        return false;
    }

    // Check if all units are reachable from the feed.
    if (!this->is_reachable()) {
        return false;
    }

    // Check if the concentrate and tailing streams are valid.
    if (!this->concentrate_tailing_check()) {
        return false;
    }

    // Check for self-recycles.
    if (!this->self_recycle_check()) {
        return false;
    }

    // Check if all product streams of a unit do not point to the same unit.
    if (!this->same_unit_dest_check()) {
        return false;
    }

    // Check if all units have a path to all outlets.
    if (!this->end_of_vector_check()) {
        return false;
    }

    // Check if the maximum value constraints are met.
    if (!this->max_value_check(vector_size, circuit_vector)) {
        return false;
    }

    // Check if the tailings percentage to the concentrate outlet is greater than 50%.
    if (!this->tail_percentage_to_concentrate_outlet_check()) {
        return false;
    }

    // Check if the feed value is within the valid range.
    if (!this->check_feed_value(circuit_vector)) {
        return false;
    }

    return true;
}

// Variables to track if each outlet has been reached.
bool conc_outlet_reached, inter_outlet_reached, tails_outlet_reached;

/**
 * @brief Mark units as reachable starting from a specific unit.
 * 
 * @param unit_num The starting unit number.
 */
void Circuit::mark_units(int unit_num) {
    // If the unit is already marked, return.
    if (this->units[unit_num].mark)
        return;

    // Mark the current unit.
    this->units[unit_num].mark = true;

    // Recursively mark units reachable from conc_num.
    if (this->units[unit_num].conc_num < this->units.size()) {
        mark_units(this->units[unit_num].conc_num);
    } else {
        conc_outlet_reached = true;
    }

    // Recursively mark units reachable from inter_num.
    if (this->units[unit_num].inter_num < this->units.size()) {
        mark_units(this->units[unit_num].inter_num);
    } else {
        inter_outlet_reached = true;
    }

    // Recursively mark units reachable from tails_num.
    if (this->units[unit_num].tails_num < this->units.size()) {
        mark_units(this->units[unit_num].tails_num);
    } else {
        tails_outlet_reached = true;
    }
}

/**
 * @brief Check if all units are reachable from the feed.
 * 
 * @return true if all units are reachable, false otherwise.
 */
bool Circuit::is_reachable() {

    // Reset the mark status for all units.
    for (auto& unit : this->units) {
        unit.mark = false;
    }

    // Mark units starting from the feed.
    mark_units(0);

    // Check if all units are marked as reachable.
    for (const auto& unit : this->units) {
        if (!unit.mark) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Check if all units have a path to all outlets.
 * 
 * @return true if all units have a path to all outlets, false otherwise.
 */
bool Circuit::end_of_vector_check() {
    // Reset the mark status for all units.
    for (auto& unit : this->units) {
        unit.mark = false;
    }

    // Mark units starting from the feed.
    mark_units(0);

    // Check if any unit has an invalid outlet path.
    for (auto& unit : this->units) {
        if (unit.conc_num == -1 ||
            unit.inter_num == -1 ||
            unit.tails_num == -1) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Check for self-recycles in the circuit.
 * 
 * @return true if no self-recycles exist, false otherwise.
 */
bool Circuit::self_recycle_check() {
    // Ensure no self-recycle exists.
    for (size_t i = 0; i < this->units.size(); ++i) {
        const auto& unit = this->units[i];
        // Check if any product stream points to the unit itself.
        if (unit.conc_num == i || 
            unit.inter_num == i || 
            unit.tails_num == i) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check if all product streams of a unit do not point to the same unit.
 * 
 * @return true if the streams do not all point to the same unit, false otherwise.
 */
bool Circuit::same_unit_dest_check() {
    // Ensure the destinations for the three product streams of each unit are not all the same unit.
    for (const auto& unit : this->units) {
        // Check if all three product streams point to the same unit.
        if (unit.conc_num == unit.inter_num &&
            unit.inter_num == unit.tails_num) {
            return false;  // If they all point to the same unit, return false indicating invalid circuit.
        }
    }

    return true;  // If no such condition is found, return true indicating valid circuit.
}

/**
 * @brief Check if all required values are present in the circuit vector.
 * 
 * @param vector_size The size of the input vector.
 * @param circuit_vector The input vector representing the circuit configuration.
 * @return true if all required values are present, false otherwise.
 */
bool Circuit::check_values(int vector_size, int *circuit_vector){
    // Find the max in the vector
    int max = 0;
    for (int i = 0; i < vector_size; i++){
        if (circuit_vector[i] > max){
            max = circuit_vector[i];
        }
    }

    // Check if the 0-max values appear in the vector
    for (int i = 0; i < max; i++){
        bool found = false;
        for (int j = 0; j < vector_size; j++){
            if (circuit_vector[j] == i){
                found = true;
                break;
            }
        }
        if (!found){
            return false;
        }
    }

    return true;
}

/**
 * @brief Check if the concentrate and tailing streams are valid.
 * 
 * @return true if the concentrate and tailing streams are valid, false otherwise.
 */
bool Circuit::concentrate_tailing_check() {
    int num_units = units.size(); // Get the number of units
    bool has_concentrate = false; // Flag to check if at least one unit outputs to concentrate
    bool has_tailings = false;    // Flag to check if at least one unit outputs to tailings

    for (const auto& unit : units) {
        // Ensure intermediate or tailings streams do not output to concentrate
        if (unit.inter_num == num_units || unit.tails_num == num_units) {
            return false;
        }
        // Ensure concentrate or intermediate streams do not output to tailings
        if (unit.conc_num == num_units + 1 || unit.inter_num == num_units + 1) {
            return false;
        }
        // Check if any unit's concentrate stream outputs to concentrate
        if (unit.conc_num == num_units) {
            has_concentrate = true;
        }
        // Check if any unit's tailings stream outputs to tailings
        if (unit.tails_num == num_units + 1) {
            has_tailings = true;
        }
    }

    // Ensure at least one unit outputs to concentrate and tailings
    if (!has_concentrate || !has_tailings) {
        return false;
    }

    return true; // All checks passed
}

/**
 * @brief Check if the maximum value constraints are met.
 * 
 * @param vector_size The size of the input vector.
 * @param circuit_vector The input vector representing the circuit configuration.
 * @return true if the maximum value constraints are met, false otherwise.
 */
bool Circuit::max_value_check(int vector_size, int *circuit_vector){
    // Find the max in the vector
    int cnt = this->units.size();

    // Check some value exceeds the max
    for (const auto& unit : units) {
        if (unit.conc_num > cnt || unit.inter_num > cnt - 1 || unit.tails_num > cnt + 1) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Check if the tailings percentage to the concentrate outlet is greater than 50%.
 * 
 * @return true if the tailings percentage is within the limit, false otherwise.
 */
bool Circuit::tail_percentage_to_concentrate_outlet_check() {
    // Check if the tailings percentage is greater than 50%
    int max = this->units.size();
    int unit_conc = 0;

    for (const auto& unit : units) {
        if (unit.conc_num == max) {
            int cnt_tails = 0;
            int cnt = 0;

            for (const auto& unit1 : units) {
                if (unit1.tails_num == unit_conc) {
                    cnt_tails++;
                    cnt++;
                }
                if (unit1.conc_num == unit_conc) {
                    cnt++;
                }
                if (unit1.inter_num == unit_conc) {
                    cnt++;
                }
            }

            if (cnt_tails > cnt * 0.5) {
                return false;
            }
        }
        unit_conc++;
    }
    return true;
}

/**
 * @brief Check if the feed value is within the valid range (0 to units.size() - 1).
 * 
 * @param circuit_vector The input vector representing the circuit configuration.
 * @return true if the feed value is within the valid range, false otherwise.
 */
bool Circuit::check_feed_value(int *circuit_vector) {
    // Check if the feed value is within the valid range (0 to units.size() - 1)

    int max = this->units.size();  // Get the number of units in the circuit
    if (circuit_vector[0] < 0 || circuit_vector[0] >= max) {  // Check if the first value is within the range
        return false;  // If not within the range, return false
    }
    return true;  // If within the range, return true
}
