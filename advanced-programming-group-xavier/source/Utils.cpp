/**
 * @file Utils.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief This file implements utility functions for various operations.
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
#include <vector>
#include <iostream>

int getVectorSum(const std::vector<double>& values) {
    if (values.empty()) return 0;
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    return sum;
}

int getVectorMidpoint(const std::vector<double>& values) {
    if (values.empty()) return 0;
    int medianVal = 0;
    int size = values.size();
    if (size % 2 == 0) {
        // even num elements, return average of the two middle values.
        int l = (size / 2) - 1;
        int r = size / 2;
        medianVal = (values[l] + values[r]) / 2;
    } else {
        // odd num elements, return the middle value.
        int c = size / 2;
        medianVal = values[c];
    }
    return medianVal;
}

int medianOfThree(std::vector<double>& vec, int& low, int& high) {
    int mid = low + (high - low) / 2;
    if (vec[mid] < vec[low])
        std::swap(vec[low], vec[mid]);
    if (vec[high] < vec[low])
        std::swap(vec[low], vec[high]);
    if (vec[mid] < vec[high])
        std::swap(vec[mid], vec[high]);
    return high;  // The pivot is now at 'high'
}

int partition(std::vector<double>& vec, int& low, int& high) {
    // median of 3 for pivot.
    int pivotIndex = medianOfThree(vec, low, high);
    std::swap(vec[pivotIndex], vec[high]);
    double pivot = vec[high];

    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (vec[j] < pivot) {
            i++;  // increment index of smaller element.
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return i + 1;
}

void quickSort(std::vector<double>& vec, int low, int high) {
    if (low < high) {
        // pi is partitioning index, vec[pi] is now at right place. break condition.
        int pi = partition(vec, low, high);
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}