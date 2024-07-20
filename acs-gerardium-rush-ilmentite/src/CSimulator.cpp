#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"

#include <cmath>
#include <stdexcept>

/**
 * @brief Evaluates the performance of a circuit based on a given vector.
 *
 * This function simulates the operation of a circuit and calculates its performance.
 * Judge if the circuit has converged (using the difference between the old and new flow rates),
 * if not, set the performance to 90 * -750.
 *
 * @param vector_size The size of the circuit vector.
 * @param circuit_vector The array representing the circuit configuration.
 * @return The performance value of the circuit.
 */

double Evaluate_Circuit(int vector_size, int *circuit_vector)
{
  struct Circuit_Parameters default_circuit_parameters;
  struct Calculate_constants constants;
  struct Initial_flow init_flow;
  struct Economic_parameters eco;

  double Performance = 0.0;
  double Recovery = 0.0;
  double Grade = 0.0;

  // Calculate the number of units in the circuit
  int length = (vector_size - 1) / 3;
  std::vector<CUnit> units = vector_to_units(circuit_vector, length, init_flow);
  int i;
  for (i = 0; i < default_circuit_parameters.max_iterations; i++)
  {
    double concentrate_gerardium = 0.0;
    double concentrate_waste = 0.0;

    // Update the flow rates for the units
    #pragma omp parallel for reduction(+:concentrate_gerardium, concentrate_waste)
    for (int j = 0; j < length; j++)
    {
      double tau = calculate_residence_time(constants, units[j].old_flow_W, units[j].old_flow_G);
      struct Recovery recovery = calculate_recovery(constants, tau);
      std::vector<double> all_flow_rate = calculate_flow_rate(constants, recovery, units[j].old_flow_G, units[j].old_flow_W);

      if (units[j].conc_num < length)
      {
        #pragma omp atomic
        units[units[j].conc_num].new_flow_G += all_flow_rate[0];
        #pragma omp atomic
        units[units[j].conc_num].new_flow_W += all_flow_rate[1];
      }
      else if (units[j].conc_num == length) // If the unit points to the concentrate stream
      {
        concentrate_gerardium += all_flow_rate[0];
        concentrate_waste += all_flow_rate[1];
      }

      if (units[j].inter_num < length)
      {
        #pragma omp atomic
        units[units[j].inter_num].new_flow_G += all_flow_rate[2];
        #pragma omp atomic
        units[units[j].inter_num].new_flow_W += all_flow_rate[3];
      }

      if (units[j].tails_num < length)
      {
        #pragma omp atomic
        units[units[j].tails_num].new_flow_G += all_flow_rate[4];
        #pragma omp atomic
        units[units[j].tails_num].new_flow_W += all_flow_rate[5];
      }
    }

    // Add initial flow to the start unit
    int start = circuit_vector[0];
    units[start].new_flow_G += init_flow.init_Fg;
    units[start].new_flow_W += init_flow.init_Fw;

    // Judge if the circuit has converged
    bool converge = true;
    for (int j = 0; j < length; j++)
    {
      // Calculate the relative difference between the old and new flow rates
      double diff_fg = std::abs(units[j].new_flow_G - units[j].old_flow_G) / units[j].old_flow_G;
      double diff_fw = std::abs(units[j].new_flow_W - units[j].old_flow_W) / units[j].old_flow_W;

      if ((diff_fg > 1e-6 || diff_fw > 1e-6))
      {
        converge = false;
        break;
      }
    }

    if (converge)
    {
      // std::cout << i << std::endl;
      // std::cout << concentrate_gerardium << " " << concentrate_waste << std::endl;
      // You can remove the comments above to see the number of iterations and the final concentrate values
      Performance = get_performance(concentrate_gerardium, concentrate_waste, eco);
      break;
    }
    else
    {
      for (int j = 0; j < length; j++)
      {
        units[j].old_flow_G = units[j].new_flow_G;
        units[j].old_flow_W = units[j].new_flow_W;
        units[j].new_flow_G = 0.0;
        units[j].new_flow_W = 0.0;
      }
    }
    // Calculate the recovery and grade of the circuit
    Recovery = concentrate_gerardium / init_flow.init_Fg;
    Grade = concentrate_gerardium / (concentrate_gerardium + concentrate_waste);
  }

  // If the circuit does not converge, set the performance to 90 * -750
  if (i == 1000)
  {
    Performance = init_flow.init_Fw * eco.penalty;
  }

  try
  {
    // Write the performance, recovery, and grade to a file
    std::ofstream outFile("./output/performance.dat");

    outFile << Performance << "\n";
    outFile << Recovery << "\n";
    outFile << Grade << "\n";

    outFile.close();
  }
  catch (const std::exception &e)
  {
  }

  return Performance;
}

/**
 * @brief Calculates the residence time of materials in a unit.
 *
 * @param constants The constants used for calculation.
 * @param Fg Flow rate of gerardium.
 * @param Fw Flow rate of waste.
 * @return The calculated residence time.
 */
double calculate_residence_time(const Calculate_constants &constants, double Fg, double Fw)
{
  double total_mass_flow_rate = Fw + Fg;
  total_mass_flow_rate = std::max(total_mass_flow_rate, 1e-10);
  double volume_flow_rate = total_mass_flow_rate / constants.rho;
  double tau = constants.phi * constants.V / volume_flow_rate;
  return tau;
};

/**
 * @brief Calculates the recovery of materials in a unit.
 *
 * @param constants The constants used for calculation.
 * @param tau The residence time of materials in the unit.
 * @return The calculated recovery.
 */
struct Recovery calculate_recovery(const Calculate_constants &constants, double tau)
{
  double rcg = (constants.k_concentrate_gerardium * tau) / (1 + (constants.k_concentrate_gerardium + constants.k_inter_gerardium) * tau);
  double rcw = (constants.k_concentrate_waste * tau) / (1 + (constants.k_concentrate_waste + constants.k_inter_waste) * tau);
  double rig = (constants.k_inter_gerardium * tau) / (1 + (constants.k_concentrate_gerardium + constants.k_inter_gerardium) * tau);
  double riw = (constants.k_inter_waste * tau) / (1 + (constants.k_concentrate_waste + constants.k_inter_waste) * tau);

  struct Recovery recoveries = {rcg, rcw, rig, riw};
  return recoveries;
};

/**
 * @brief Calculates the flow rates of materials in a unit.
 *
 * @param constants The constants used for calculation.
 * @param recovery The recovery of materials in the unit.
 * @param init_Fg Initial flow rate of gerardium.
 * @param init_Fw Initial flow rate of waste.
 * @return The calculated flow rates.
 */
std::vector<double> calculate_flow_rate(const Calculate_constants &constants, Recovery &recovery, double init_Fg, double init_Fw)
{
  double cg = init_Fg * recovery.concentrate_gerardium;
  double cw = init_Fw * recovery.concentrate_waste;
  double ig = init_Fg * recovery.inter_gerardium;
  double iw = init_Fw * recovery.inter_waste;
  double tg = init_Fg - cg - ig;
  double tw = init_Fw - cw - iw;

  return {cg, cw, ig, iw, tg, tw};
};

/**
 * @brief Calculates the performance of a circuit.
 *
 * @param cg Concentrate grade of gerardium.
 * @param cw Concentrate grade of waste.
 * @param eco Economic parameters.
 * @return The calculated performance.
 */
double get_performance(double cg, double cw, const Economic_parameters &eco)
{
  double result = cg * eco.price + cw * eco.penalty;
  return result;
};

/**
 * @brief Converts a vector to a vector of units.
 *
 * @param vector The vector to be converted.
 * @param n The size of the vector.
 * @param init_flow The initial flow rates.
 * @return The vector of units.
 */
std::vector<CUnit> vector_to_units(int *vector, int n, const Initial_flow &init_flow)
{
  std::vector<CUnit> units(n);
  for (int i = 0; i < n; i++)
  {
    units[i].old_flow_G = init_flow.init_Fg;
    units[i].old_flow_W = init_flow.init_Fw;
    units[i].new_flow_G = 0;
    units[i].new_flow_W = 0;
    units[i].conc_num = vector[3 * i + 1];
    units[i].inter_num = vector[3 * i + 2];
    units[i].tails_num = vector[3 * i + 3];
  }
  return units;
};
