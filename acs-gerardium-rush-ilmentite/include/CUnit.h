/**
 * @file CUnit.h
 * @brief Header for the unit class.
 *
 * This header defines the CUnit class, which represents a unit in the circuit.
 */

#pragma once

/**
 * @class CUnit
 * @brief Represents a unit in the circuit.
 */
class CUnit
{
public:
  /**
   * @brief Index of the unit to which this unit’s concentrate stream is connected.
   */
  int conc_num;

  /**
   * @brief Index of the unit to which this unit’s intermediate stream is connected.
   */
  int inter_num;

  /**
   * @brief Index of the unit to which this unit’s tailings stream is connected.
   */
  int tails_num;

  /**
   * @brief A Boolean that is changed to true if the unit has been seen.
   */
  bool mark;

  /**
   * @brief Total old input flow of gerardium.
   */
  double old_flow_G;

  /**
   * @brief Total old input flow of waste.
   */
  double old_flow_W;

  /**
   * @brief Total new input flow of gerardium.
   */
  double new_flow_G;

  /**
   * @brief Total new input flow of waste.
   */
  double new_flow_W;

  /**
   * @brief Default constructor for CUnit.
   *
   * Initializes the unit with default values.
   */
  CUnit() : conc_num(-1), inter_num(-1), tails_num(-1), mark(false), old_flow_G(0.0), old_flow_W(0.0), new_flow_G(0.0), new_flow_W(0.0) {}

  /*
   * ...other member functions and variables of CUnit
   */
};
