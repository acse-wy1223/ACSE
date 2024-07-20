/**
 * @file HistogramEqualisation.cpp
 * @author Zeqi Li (acse-zl123)
 * @brief Implements the HistogramEqualisation class for histogram equalization on images.
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
#include "../include/HistogramEqualisation.h"

HistogramEqualisation::HistogramEqualisation(bool HSV) : Filter2D(), isHSV(HSV) {}

void HistogramEqualisation::RGBToHSV(double r, double g, double b, double &h, double &s, double &v) {
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

void HistogramEqualisation::HSVToRGB(double h, double s, double v, double &r, double &g, double &b) {
    double c = v * s;
    double x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double rPrime, gPrime, bPrime;

    if (h >= 0 && h < 60) {
        rPrime = c;
        gPrime = x;
        bPrime = 0;
    } else if (h >= 60 && h < 120) {
        rPrime = x;
        gPrime = c;
        bPrime = 0;
    } else if (h >= 120 && h < 180) {
        rPrime = 0;
        gPrime = c;
        bPrime = x;
    } else if (h >= 180 && h < 240) {
        rPrime = 0;
        gPrime = x;
        bPrime = c;
    } else if (h >= 240 && h < 300) {
        rPrime = x;
        gPrime = 0;
        bPrime = c;
    } else {
        rPrime = c;
        gPrime = 0;
        bPrime = x;
    }

    r = (rPrime + m) * 255;
    g = (gPrime + m) * 255;
    b = (bPrime + m) * 255;
}

void HistogramEqualisation::RGBToHSL(double r, double g, double b, double &h, double &s, double &l) {
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

void HistogramEqualisation::HSLToRGB(double h, double s, double l, double &r, double &g, double &b) {
    auto hueToRGB = [](double p, double q, double t) {
        if (t < 0) t += 1;
        if (t > 1) t -= 1;
        if (t < 1.0 / 6.0) return p + (q - p) * 6 * t;
        if (t < 1.0 / 2.0) return q;
        if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6;
        return p;
    };

    if (s == 0) {
        r = g = b = l;
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hueToRGB(p, q, h / 360 + 1.0 / 3.0);
        g = hueToRGB(p, q, h / 360);
        b = hueToRGB(p, q, h / 360 - 1.0 / 3.0);
    }

    r *= 255.0f;
    g *= 255.0f;
    b *= 255.0f;
}

void HistogramEqualisation::equalise(unsigned char* data, int imageSize) {
    const int histogramSize = 256;
    std::vector<int> histogram(histogramSize, 0);
    std::vector<double> cdf(histogramSize, 0);

    for (int i = 0; i < imageSize; i++) {
        histogram[data[i]]++;
    }

    cdf[0] = histogram[0];
    for (int i = 1; i < histogramSize; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }
    double minCdf = *std::min_element(cdf.begin(), cdf.end(), [](double a, double b) {
        return (a > 0 && a < b) || b == 0;
    });
    double maxCdf = cdf.back();
    std::transform(cdf.begin(), cdf.end(), cdf.begin(), [minCdf, maxCdf](double &c) {
        return (c - minCdf) / (maxCdf - minCdf);
    });

    for (int i = 0; i < imageSize; i++) {
        // Normalize the pixel values to 0-255
        data[i] = static_cast<unsigned char>(255 * cdf[data[i]]);
    }
}

void HistogramEqualisation::equalise(std::vector<double>& vChannel, int vSize) {
    const int histogramSize = 256;
    std::vector<int> histogram(histogramSize, 0);
    for (int i = 0; i < vSize; ++i) {
        int vIndex = static_cast<int>(vChannel[i] * 255);
        histogram[vIndex]++;
    }

    std::vector<double> cdf(histogramSize, 0);
    cdf[0] = histogram[0];
    for (int i = 1; i < histogramSize; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }
    double cdfMin = *std::min_element(cdf.begin(), cdf.end(), [](double a, double b) { return a > 0 && a < b; });
    double cdfRange = cdf.back() - cdfMin;

    for (int i = 0; i < vSize; ++i) {
        int vIndex = static_cast<int>(vChannel[i] * 255);
        double equalizedV = (cdf[vIndex] - cdfMin) / cdfRange;
        vChannel[i] = equalizedV;
    }
}

void HistogramEqualisation::apply(Image& image) {
    unsigned char*& data = image.getDataPtr();
    int width = image.getWidth();
    int height = image.getHeight();
    int channels = image.getChannels();
    if (channels == 1)
    {
        equalise(data, width * height);
    }
    else if(isHSV)
    {
        std::vector<double> vChannel(width * height);
        for (int i = 0; i < width * height; i++) {
            double r = data[i * channels];
            double g = data[i * channels + 1];
            double b = data[i * channels + 2];

            double h, s, v;
            RGBToHSV(r, g, b, h, s, v);
            
            vChannel[i] = v;
        }

        equalise(vChannel, width * height);

        for (int i = 0; i < width * height; i++) {
            double r, g, b;
            double h, s, v;

            RGBToHSV(data[i * channels], data[i * channels + 1], data[i * channels + 2], h, s, v);
            
            v = vChannel[i];
            
            HSVToRGB(h, s, v, r, g, b);

            data[i * channels] = static_cast<unsigned char>(r);
            data[i * channels + 1] = static_cast<unsigned char>(g);
            data[i * channels + 2] = static_cast<unsigned char>(b);
        }
    }
    else
    {
        std::vector<double> lChannel(width * height);
        for (int i = 0; i < width * height; i++) {
            double r = data[i * channels];
            double g = data[i * channels + 1];
            double b = data[i * channels + 2];

            double h, s, l;
            RGBToHSL(r, g, b, h, s, l);
            
            lChannel[i] = l;
        }

        equalise(lChannel, width * height);

        for (int i = 0; i < width * height; i++) {
            double r, g, b;
            double h, s, l;

            RGBToHSL(data[i * channels], data[i * channels + 1], data[i * channels + 2], h, s, l);
            
            l = lChannel[i];
            
            HSLToRGB(h, s, l, r, g, b);

            data[i * channels] = static_cast<unsigned char>(r);
            data[i * channels + 1] = static_cast<unsigned char>(g);
            data[i * channels + 2] = static_cast<unsigned char>(b);
        }
    }
}