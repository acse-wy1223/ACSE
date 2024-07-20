#include <cmath>
#include <iostream>

#include "CSimulator.h"

int main(int argc, char *argv[])
{
      // Test for function Evaluate_Circuit
      std::cout << "---------Test for function Evaluate_Circuit---------\n";
      // Test 1, dummy data from the slides
      int vec1[] = {0, 1, 3, 2, 4, 4, 3, 1, 3, 6, 1, 1, 0, 5, 1, 1};

      std::cout << "Evaluate_Circuit(16, vec1) close to 167.378:\n";
      double result = Evaluate_Circuit(16, vec1);
      std::cout << "Evaluate_Circuit(16, vec1) = " << result << "\n";

      if (std::fabs(result - 167.378) < 1.0e-3)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test 2, the data that can't converge
      int vec2[10] = {0, 1, 1, 2, 3, 0, 1, 1, 1, 1};
      std::cout << "\nEvaluate_Circuit(10, vec2) is -67500: \n";
      double result2 = Evaluate_Circuit(10, vec2);
      std::cout << "Evaluate_Circuit(10, vec2) = " << result2 << "\n";

      if (result2 == -67500.0)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test 3
      int vec3[7] = {0, 1, 0, 2, 3, 1, 2};
      std::cout << "\nEvaluate_Circuit(7, vec3) is 0: \n";
      double result3 = Evaluate_Circuit(7, vec3);
      std::cout << "Evaluate_Circuit(7, vec3) = " << result3 << "\n";
      if (result3 == 0.0)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test 4
      int vec4[31] = {1, 3, 5, 8, 0, 2, 4, 8, 3, 3, 4, 4, 0, 6, 0, 5, 10, 6, 6, 7, 7, 7, 2, 8, 2, 9, 9, 9, 5, 0, 11};
      std::cout << "\nEvaluate_Circuit(31, vec4) is close to -294.262: \n";
      double result4 = Evaluate_Circuit(31, vec4);
      std::cout << "Evaluate_Circuit(31, vec4) = " << result4 << "\n";
      if (abs(result4 - (-294.262)) < 1.0e-3)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test 5
      int vec5[16] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 1, 0, 2, 6, 5, 0, 6};
      std::cout << "\nEvaluate_Circuit(16, vec5) is close to -112.007: \n";
      double result5 = Evaluate_Circuit(16, vec5);
      std::cout << "Evaluate_Circuit(16, vec5) = " << result5 << "\n";
      if (abs(result5 - (-112.007)) < 1.0e-3)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test 6
      int vec6[76] = {3, 16, 14, 11, 2, 16, 2, 4, 1, 0, 1, 4, 4, 5, 5, 5, 7, 6, 6, 8, 8, 7, 6, 2, 8, 12, 9, 9, 10, 15, 10, 13, 7, 15, 14, 10, 12, 9, 11, 13,
                      11, 12, 14, 15, 13, 16, 24, 0, 18, 17, 17, 17, 18, 18, 26, 20, 24, 19, 21, 20, 20, 22, 4, 21, 25, 19, 22, 19, 21, 23, 0, 22, 24, 23, 23, 1};

      std::cout << "\nEvaluate_Circuit(76, vec6) is close to -292.084: \n";
      double result6 = Evaluate_Circuit(76, vec6);
      std::cout << "Evaluate_Circuit(76, vec6) = " << result6 << "\n";
      if (abs(result6 - (-292.084)) < 1.0e-3)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test for function calculate_residence_time
      std::cout
          << "\n---------Test for function calculate_residence_time---------\n";
      Calculate_constants constants;
      constants.rho = 3000.0;
      constants.phi = 0.1;
      constants.V = 10.0;
      double Fg = 10.0;
      double Fw = 90.0;
      double result7 = calculate_residence_time(constants, Fg, Fw);
      std::cout << "calculate_residence_time(constants, Fg, Fw) = " << result7 << "\n";
      if (result7 == 30)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test for function calculate_recovery
      std::cout << "\n---------Test for function calculate_recovery---------\n";
      Recovery recovery = calculate_recovery(constants, 30);
      std::cout << "calculate_recovery(constants, 30): \n"
                << "concentrate_gerardium: " << recovery.concentrate_gerardium << " concentrate_waste: " << recovery.concentrate_waste << "\ninter_gerardium: " << recovery.inter_gerardium << "\tinter_waste: " << recovery.inter_waste << "\n";
      if (abs(recovery.concentrate_gerardium - 0.104348) < 1e-6 &&
          abs(recovery.concentrate_waste - 0.00591133) < 1e-6 &&
          abs(recovery.inter_gerardium - 0.026087) < 1e-6 &&
          abs(recovery.inter_waste - 0.008867) < 1e-6)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test for function calculate_flow_rate
      std::cout << "\n---------Test for function calculate_flow_rate---------\n";
      std::vector<double> all_flow_rate = calculate_flow_rate(constants, recovery, Fg, Fw);
      std::cout << "calculate_flow_rate(constants, recovery, Fg, Fw):\n"
                << "all_flow_rate[0]: " << all_flow_rate[0] << "\tall_flow_rate[1]: " << all_flow_rate[1] << "\nall_flow_rate[2]: " << all_flow_rate[2] << "\tall_flow_rate[3]: " << all_flow_rate[3] << "\nall_flow_rate[4]: " << all_flow_rate[4] << "\tall_flow_rate[5]: " << all_flow_rate[5] << "\n";
      if (abs(all_flow_rate[0] - 1.04348) < 1e-5 &&
          abs(all_flow_rate[1] - 0.53202) < 1e-5 &&
          abs(all_flow_rate[2] - 0.26087) < 1e-5 &&
          abs(all_flow_rate[3] - 0.79803) < 1e-5 &&
          abs(all_flow_rate[4] - 8.69565) < 1e-5 &&
          abs(all_flow_rate[5] - 88.67) < 1e-2)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test for function get_performance
      std::cout << "\n---------Test for function get_performance---------\n";
      struct Economic_parameters eco;
      double result8 = get_performance(1, 1, eco);
      std::cout << "get_performance(1, 1, eco) = " << result8 << "\n";
      if (result8 == -650.0)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      // Test for function vector_to_units
      std::cout << "\n---------Test for function vector_to_units---------\n";
      int vec9[16] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 1, 0, 2, 6, 5, 0, 6};
      struct Initial_flow init_flow;
      std::vector<CUnit> units = vector_to_units(vec9, 16, init_flow);
      std::cout << "vector_to_units(vec9, 16, init_flow): \n";
      int n = sizeof(vec9) / sizeof(int);
      int unit = (n - 1) / 3;

      for (int i = 0; i < unit; i++)
      {
            std::cout << "units[" << i << "].conc_num: " << units[i].conc_num << "\tunits[" << i << "].inter_num: " << units[i].inter_num << "\tunits[" << i << "].tails_num: " << units[i].tails_num << "\n";
      }
      if (units[0].conc_num == 1 && units[0].inter_num == 1 && units[0].tails_num == 2 &&
          units[1].conc_num == 2 && units[1].inter_num == 3 && units[1].tails_num == 3 &&
          units[2].conc_num == 0 && units[2].inter_num == 4 && units[2].tails_num == 1 &&
          units[3].conc_num == 0 && units[3].inter_num == 2 && units[3].tails_num == 6 &&
          units[4].conc_num == 5 && units[4].inter_num == 0 && units[4].tails_num == 6)
      {
            std::cout << "pass\n";
      }
      else
      {
            std::cout << "fail\n";
            return 1;
      }

      return 0;
}
