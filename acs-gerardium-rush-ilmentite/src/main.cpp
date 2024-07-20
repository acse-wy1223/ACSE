#include <iostream>
#include <chrono>
#include "CUnit.h"
#include "CCircuit.h"
#include "CSimulator.h"
#include "Genetic_Algorithm.h"

using namespace std;

int main(int argc, char *argv[]) {

    int units = 42;

    int vector[(units*3)+1];
    int n = sizeof(vector) / sizeof(int);

    // Adjust the parameters as needed
    Algorithm_Parameters params = {1000, 0.9, 0.01, 0.1, 500};


    // Measure time for optimize function
    auto start_optimize = chrono::high_resolution_clock::now();
    optimize(n, vector, Evaluate_Circuit, Check_Validity, params);
    auto end_optimize = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_optimize = end_optimize - start_optimize;
    cout << "Time taken for optimization: " << duration_optimize.count() << " seconds" << endl;

    // Measure time for Evaluate_Circuit function
    double evaluation_result = Evaluate_Circuit(n, vector);

    // Generate final output, save to file, etc.
    cout << "Evaluation result: " << evaluation_result << endl;

    return 0;
}
