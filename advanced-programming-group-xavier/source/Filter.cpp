/**
 * @file Filter.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief The Filter class is an abstract base class for image/volume filters.
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

#include "../include/Filter.h"
#include "../include/Image.h"
#include "../include/Volume.h"

template <typename T>
Filter<T>::Filter() = default;

// explicit instantiation definitions.
template class Filter<Image>;
template class Filter<Volume>;



