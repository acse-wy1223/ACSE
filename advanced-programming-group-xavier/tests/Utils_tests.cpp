#include "Utils_tests.h"
#include "../include/Utils.h"
#include <vector>

void testGetVectorSumNotEmpty() {
    std::vector<double> initVec{2., 1., 4., 4., 8., 7.};
    int desired = 26;
    int actual = getVectorSum(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector sum error..");
    }
}

void testGetVectorSumNotEmptyFloats() {
    std::vector<double> initVec{2.1, 1.1, 4.1, 4.1, 8.1, 7.1};
    int desired = 26;  // 26.6 --> 26 (rounding to int).
    int actual = getVectorSum(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector sum on empty list error..");
    }
}

void testGetVectorSumEmpty() {
    std::vector<double> initVec{};
    int desired = 0;
    int actual = getVectorSum(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector sum on empty list error..");
    }
}

void testGetVectorMidpointNotEmptyEven() {
    std::vector<double> initVec{2., 1., 4., 5., 8., 7.};
    int desired = 4;  // (4+5)/9 --> 4 cast to int.
    int actual = getVectorMidpoint(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector (even num elements) midpoint error..");
    }
}

void testGetVectorMidpointNotEmptyOdd() {
    std::vector<double> initVec{2., 1., 4., 5., 8., 7., 9.};
    quickSort(initVec, 0, initVec.size()-1);
    int desired = 5;
    int actual = getVectorMidpoint(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector (odd num elements) midpoint error..");
    }
}

void testGetVectorMidpointEmpty() {
    std::vector<double> initVec{};
    int desired = 0;
    int actual = getVectorMidpoint(initVec);
    if (actual != desired) {
        throw std::runtime_error("Vector (empty) midpoint error..");
    }
}

void testQuicksort() {
    std::vector<double> initVec{2., 1., 4., 4., 8., 7.};
    std::vector<double> desired{1., 2., 4., 4., 7., 8.};
    quickSort(initVec, 0, 6-1);
    if (initVec != desired) {
        throw std::runtime_error("Quicksort error..");
    }
}

void testQuicksortSingleElementList() {
    std::vector<double> initVec{2.};
    std::vector<double> desired{2.};
    quickSort(initVec, 0, 1-1);
    if (initVec != desired) {
        throw std::runtime_error("Quicksort on length 1 input list error.");
    }
}

void testMedianOfThree() {
    std::vector<double> initVec{5., 4., 3., 2., 1.};
    int low = 0, high = initVec.size()-1;
    int actualIdx = medianOfThree(initVec, low, high);
    int desiredIdx = 4;
    if (actualIdx != desiredIdx) {
        throw std::runtime_error("Median of 3 error.");
    }

}