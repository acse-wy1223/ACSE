/**
 * @file Image.cpp
 * @author Daniel Seal (edsml-ds423)
 * @brief Implements the Image class used for loading, manipulating, and saving images.
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
#include <fstream>
#include <vector>
// stb_impl.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include "../include/Image.h"

// constructor 1 (filepath).
Image::Image(std::string &filepath):
    filepath(filepath) {
    readImage(); // read in image on instantiation. initialises dataPtr, h, c and w.
    // explicitly set after readImage().
    desiredChannels = channels;
    rawHeight = height;
    rawWidth = width;
    createImageData();
}

// constructor 2 (filepath and desired channels).
Image::Image(std::string &filepath, int& c):
    filepath(filepath), desiredChannels(c) {
    readImage(); // read in image on instantiation. initialises dataPtr.
    rawHeight = height;
    rawWidth = width;
    createImageData();
}

// constructor 3 (filepath + desired height, width and channels).
Image::Image(std::string &filepath, int& h, int& w, int& c):
    filepath(filepath), desiredChannels(c) {
    readImage(); // read in image on instantiation. initialises dataPtr.
    // hard set h and w after reading in image to work with specific dims.
    rawHeight = height;
    rawWidth = width;
    // overwrite h, w with desired.
    height = h;
    width = w;
    createImageData();
}

// default.
Image::Image():
    dataPtr(nullptr),
    width(0),
    height(0),
    channels(0) {
    desiredChannels = channels;
    rawHeight = height;
    rawWidth = width;
}

// destructor.
Image::~Image() {
    delete[] dataPtr;
}

// copy constructor 1.
Image::Image(const Image& imageCopy):
    width(imageCopy.width),
    height(imageCopy.height),
    channels(imageCopy.channels),
    rawWidth(imageCopy.rawWidth),
    rawHeight(imageCopy.rawHeight),
    desiredChannels(imageCopy.desiredChannels)
{
    size_t dataSize = rawHeight * rawWidth * channels;
    dataPtr = new unsigned char[dataSize]; // Allocate new memory with h*w*c size.
    std::copy(imageCopy.dataPtr, imageCopy.dataPtr + dataSize, dataPtr);
}

// override the << operator to access basic infor quickly.
std::ostream& operator << (std::ostream& s, Image const& m) {
    return s << "** Matrix Info (h (rawH), w (rawW), c (rawC): ("
             << m.height << " (" << m.rawHeight << ")" << ", "
             << m.width << " (" << m.rawWidth << ")" << ", "
             << m.desiredChannels << " (" << m.channels << ")"
             << ") **" << std::endl;
}

int& Image::operator()(int x, int y, int z) {
    if (x >= height || y >= width || z >= channels) { throw std::out_of_range("IndexError: Index out of bounds"); }
    return data[x][y][z];
}

void Image::readImage() {
    dataPtr = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    if (dataPtr == nullptr) { std::cerr << "Error loading image." << std::endl; }
}

void Image::outputImage(const std::string& outputFilePath) {
    int h=getHeight(),
        w=getWidth(),
        c=getDesiredChannels(),
        rawH=getRawHeight(),
        rawW=getRawWidth(),
        rawC=getChannels();

    int success;
    if ((h == rawH) && (w == rawW) && (c == rawC)) {
        success = stbi_write_png(outputFilePath.c_str(),
                                 getWidth(),
                                 getHeight(),
                                 getChannels(),
                                 getDataPtr(),
                                 0);
    } else {
        std::cerr << "** making new modified memory to output image **" << std::endl;
        std::vector<unsigned char> pixels;
        pixels.reserve(w * h * c);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                for (int k = 0; k < c; ++k) {
                    pixels.emplace_back(data[i][j][k]);
                }
            }
        }
        success = stbi_write_png(outputFilePath.c_str(), w, h, c, pixels.data(), w * c);
    }
    if (!success) { std::cerr << "Failed to write image to " << outputFilePath << "." << std::endl; }
}

void Image::createImageData() { // has access to private.
    data.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(desiredChannels)));
    int idx;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < desiredChannels; k++) {
                idx = ((i * rawWidth) + j) * channels + k;  // need to use raw dims to create the correct pointer.
                data[i][j][k] = static_cast<int>(dataPtr[idx]);
            }
        }
    }
}

void Image::setPixel(int& h, int& w, int& c, int& value) {
    dataPtr[((h * rawWidth) + w) * channels + c] = value;
}
void Image::setHeight(int &h) {height = h;}
void Image::setWidth(int &w) {width = w;}
void Image::setChannels(int& c) {channels = c;}
void Image::setDesiredChannels(int& dc) {desiredChannels = dc;}
void Image::setDataPtr(unsigned char*& p) {dataPtr = p;}
void Image::createPaddedImageData(int& pad) { // has access to private.
    // resize data to include padding. Padding with 0s by default.
    data.resize(height+(2*pad), std::vector<std::vector<int>>(width+(2*pad), std::vector<int>(desiredChannels)));
    int idx;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < desiredChannels; k++) {
                idx = ((i * rawWidth) + j) * channels + k;  // need to use raw dims to create the correct pointer.
                data[i+pad][j+pad][k] = static_cast<int>(dataPtr[idx]);
            }
        }
    }
    // update the height and width.
    int newHeight = height+(2*pad);
    setHeight(newHeight);

    int newWidth = width+(2*pad);
    setWidth(newWidth);
}

unsigned char& Image::getPixel(int& h, int& w, int& c) {
    return dataPtr[((h * rawWidth) + w) * channels + c];
}
const int& Image::getHeight()  const { return height; }
const int& Image::getWidth() const { return width; }
const int& Image::getRawHeight()  const { return rawHeight; }
const int& Image::getRawWidth() const { return rawWidth; }
const int& Image::getChannels() const { return channels; }
const int& Image::getDesiredChannels() const { return desiredChannels; }
unsigned char*& Image::getDataPtr() { return dataPtr; }