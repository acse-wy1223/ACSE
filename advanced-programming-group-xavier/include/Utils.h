/**
 * @file Utils.h
 * @author Daniel Seal (edsml-ds423)
 * @brief This file contains utility functions for various operations.
 * 
 * @copyright Xavier-Initialization (2024)
 * Daniel Seal (edsml-ds423)
 * Yongwen Chen (acse-yc3321)
 * Zeqi Li (acse-zl123)
 * Jing-Han Huang (edsml-jh123)
 * Wenbo Yu (acse-wy1223)
 * Ning Guan (edsml-ng323)
 * 
 */

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UTILS_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UTILS_H

#include <vector>
#include "Kernel.h"

/**
 * @brief Calculates the sum of values in a vector.
 * @param values The input vector.
 * @return The sum of values in the vector.
 */
int getVectorSum(const std::vector<double>& values);

/**
 * @brief Calculates the midpoint value of a vector.
 * @param values The input vector.
 * @return The midpoint value of the vector.
 */
int getVectorMidpoint(const std::vector<double>& values);

/**
 * @brief Finds the median of three values in a vector.
 * @param vec The input vector.
 * @param low The starting index of the subvector.
 * @param high The ending index of the subvector.
 * @return The index of the median value.
 */
int medianOfThree(std::vector<double>& vec, int& low, int& high);

/**
 * @brief Partitions a vector around a pivot value.
 * @param vec The input vector.
 * @param low The starting index of the subvector.
 * @param high The ending index of the subvector.
 * @return The index of the pivot value after partitioning.
 */
int partition(std::vector<double>& vec, int& low, int& high);

/**
 * @brief Performs quicksort on a vector.
 * @param vec The input vector to be sorted.
 * @param low The starting index of the subvector.
 * @param high The ending index of the subvector.
 */
void quickSort(std::vector<double>& vec, int low, int high);

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_UTILS_H