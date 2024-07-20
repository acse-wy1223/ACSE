/**
 * @file Thresholding.cpp
 * @author Zeqi Li (acse-zl123)
 * @brief Implements the Thresholding class for image thresholding.
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
#include "../include/Thresholding.h"

Thresholding::Thresholding(double threshold, bool isHSV) : Filter2D(), threshold(threshold), isHSV(isHSV) {}

void Thresholding::RGBToHSV(double r, double g, double b, double &h, double &s, double &v) {
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;

    double max = std::max({r, g, b});
    double min = std::min({r, g, b});
    double delta = max - min;

    if (delta == 0) {
        h = 0;
    } else if (max == r) {
        h = 60 * fmod(((g - b) / delta), 6);
    } else if (max == g) {
        h = 60 * (((b - r) / delta) + 2);
    } else if (max == b) {
        h = 60 * (((r - g) / delta) + 4);
    }

    if (h < 0) {
        h += 360.0f;
    }

    s = (max == 0) ? 0 : (delta / max);
    v = max;
}

void Thresholding::RGBToHSL(double r, double g, double b, double &h, double &s, double &l) {
    r /= 255.0f;
    g /= 255.0f;
    b /= 255.0f;

    double maxColor = std::max({r, g, b});
    double minColor = std::min({r, g, b});
    l = (maxColor + minColor) / 2;

    if (maxColor == minColor) {
        h = 0;
        s = 0;
    } else {
        double delta = maxColor - minColor;
        s = l > 0.5 ? delta / (2 - maxColor - minColor) : delta / (maxColor + minColor);

        if (maxColor == r) {
            h = (g - b) / delta + (g < b ? 6 : 0);
        } else if (maxColor == g) {
            h = (b - r) / delta + 2;
        } else if (maxColor == b) {
            h = (r - g) / delta + 4;
        }
        h *= 60;
    }
}


void Thresholding::apply(Image& image) {
    unsigned char*& data = image.getDataPtr();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    if (channels == 1)
    {
        for (int i = 0; i < width * height; ++i) {
            data[i] = data[i] < threshold ? 0 : 255;
        }
    }
    else if (isHSV)
    {
        for (int i = 0; i < width * height; ++i) {
            double r = data[i * channels];
            double g = data[i * channels + 1];
            double b = data[i * channels + 2];

            double h, s, v;
            RGBToHSV(r, g, b, h, s, v);

            if (v * 255 < threshold) {
                data[i * channels] = 0;
                data[i * channels + 1] = 0;
                data[i * channels + 2] = 0;
            } else {
                data[i * channels] = 255;
                data[i * channels + 1] = 255;
                data[i * channels + 2] = 255;
            }
        }
    }
    else
    {
        for (int i = 0; i < width * height; ++i) {
            double r = data[i * channels];
            double g = data[i * channels + 1];
            double b = data[i * channels + 2];

            double h, s, l;
            RGBToHSL(r, g, b, h, s, l);

            if (l * 255 < threshold) {
                data[i * channels] = 0;
                data[i * channels + 1] = 0;
                data[i * channels + 2] = 0;
            } else {
                data[i * channels] = 255;
                data[i * channels + 1] = 255;
                data[i * channels + 2] = 255;
            }
        }
    }
}