/**
 * @file HistogramEqualisation.h
 * @author Zeqi Li (acse-zl123)
 * @brief Provides the HistogramEqualisation class for histogram equalization on images.
 * 
 * This header defines the HistogramEqualisation class which inherits from Filter2D. It implements
 * histogram equalization for enhancing the contrast of images. The class supports both gray and RGB in HSV/HSL color spaces
 * for equalization, allowing for flexible image processing according to the color model specified at instantiation.
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
#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_HISTOGRAM_EQUALISATION_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_HISTOGRAM_EQUALISATION_H

#include "Image.h"
#include "Filter2D.h"
#include <vector>
#include <cmath>
#include <algorithm>

/**
 * @class HistogramEqualisation
 * @brief Implements histogram equalization for image contrast enhancement.
 * 
 * HistogramEqualisation is a derived class of Filter2D designed to apply histogram equalization
 * on an Image object. This technique improves the contrast of images by stretching out the intensity
 * distribution. The class supports both the RGB and HSV/HSL color models for equalization.
 */
class HistogramEqualisation : Filter2D {
public:
    /**
     * @brief Constructs a new Histogram Equalisation filter object.
     * 
     * Initializes the filter with the choice of using HSV or HSL. When set to true, for RGB images, the equalization
     * will be performed on the V channel of the HSV color space. Otherwise, equalization will be applied
     * on the L channel of the HSL color space.
     * 
     * @param isHSV Boolean flag to determine the color space for equalization (true for HSV, false for HSL).
     */
    HistogramEqualisation(bool isHSV);
    /**
     * @brief Applies histogram equalization to the given image.
     * 
     * Modifies the given Image object by applying histogram equalization to enhance its contrast. The method
     * decides the color space for equalization based on the isHSV member variable.
     * 
     * @param image Reference to the Image object to be processed.
     */
    void apply(Image& image);
private:
    bool isHSV;

    /**
     * @brief Converts RGB color to HSV color space.
     * 
     * @param r Red component value.
     * @param g Green component value.
     * @param b Blue component value.
     * @param h Hue component value.
     * @param s Saturation component value.
     * @param v Value (brightness) component value.
     */
    void RGBToHSV(double r, double g, double b, double &h, double &s, double &v);

    /**
     * @brief Converts HSV color to RGB color space.
     * 
     * @param h Hue component value.
     * @param s Saturation component value.
     * @param v Value (brightness) component value.
     * @param r Red component value.
     * @param g Green component value.
     * @param b Blue component value.
     */
    void HSVToRGB(double h, double s, double v, double &r, double &g, double &b);

    /**
     * @brief Converts RGB color to HSL color space.
     * 
     * @param r Red component value.
     * @param g Green component value.
     * @param b Blue component value.
     * @param h Hue component value.
     * @param s Saturation component value.
     * @param l Lightness component value.
     */
    void RGBToHSL(double r, double g, double b, double &h, double &s, double &l);

    /**
     * @brief Converts HSL color to RGB color space.
     * 
     * @param h Hue component value.
     * @param s Saturation component value.
     * @param l Lightness component value.
     * @param r Red component value.
     * @param g Green component value.
     * @param b Blue component value.
     */
    void HSLToRGB(double h, double s, double l, double &r, double &g, double &b);

    /**
     * @brief Performs histogram equalization on image data.
     * 
     * @param data Pointer to the image data.
     * @param imageSize Size of the image data.
     */
    void equalise(unsigned char* data, int imageSize);

    /**
     * @brief Performs histogram equalization on a vector representing a color channel.
     * 
     * @param vChannel Reference to the vector representing the color channel.
     * @param vSize Size of the vector.
     */
    void equalise(std::vector<double>& vChannel, int vSize);
};


#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_HISTOGRAM_EQUALISATION_H