/**
 * @file Image.h
 * @author Daniel Seal (edsml-ds423)
 * @brief The Image class is used for loading, manipulating, and saving images.
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

#ifndef ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_IMAGE_H
#define ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_IMAGE_H
#include <iostream>
#include <vector>

/**
 * @class Image
 * @brief Class for handling image loading, processing, and saving.
 * The Image class used the STB image libraries are used to load, manipulate, and save images.
 */
class Image {
public:
    /** * @brief Overloads the << operator for easy printing of Image objects. */
    friend std::ostream& operator << (std::ostream& s, Image const& m);

    /**
     * @brief Constructor that loads an image from a file path.
     * @param filepath The path to the image file to be loaded.
     */
    explicit Image(std::string &filepath);

    /**
     * @brief Constructor that loads an image from a filepath with the desired number of channels to work with.
     * @param filepath Path to the image file.
     * @param c Desired image channels.
     */
    Image(std::string &filepath, int& c);

    /**
     * @brief Constructor that loads an image from a filepath with the desired height, width and channels to work with.
     * @param filepath Path to the image file.
     * @param h Desired image height.
     * @param w Desired image width.
     * @param c Desired image channels.
     */
    Image(std::string &filepath, int& h, int& w, int& c);
    /** * @brief default constructor. */
    Image();
    /** * @brief copy constructor. */
    Image(const Image& imageCopy);
    /** * @brief destructor. */
    ~Image();

    /**
     * @brief Override the () operator to access pixel value at specified height (x), width(y) and channel(z).
     * @param x X-coordinate (height).
     * @param y Y-coordinate (width).
     * @param z Channel index.
     * @return Reference to the pixel value.
     */
    int& operator()(int x, int y, int z);

    /**
     * @brief Load image from file.
     * @return 'unsigned char *' which points to the pixel data in a 1D buffer.
     */
    void readImage();
    /** * @brief Initialize image data into a x, y, z tensor structure. */
    void createImageData();
    /** * @brief Overwrite the .data attr with a padded version of the raw image. */
    void createPaddedImageData(int& pad);
    /**
     * @brief Save image to file.
     * @param outputFilePath Path to save the image.
     */
    void outputImage(const std::string& outputFilePath);

    // setters.
    void setPixel(int &h, int &w, int& c, int& value);
    void setHeight(int &h);
    void setWidth(int &w);
    void setDataPtr(unsigned char*& p);
    void setChannels(int& c);
    void setDesiredChannels(int& dc);

    // getters.
    unsigned char& getPixel(int& h, int& w, int& c);
    const int& getHeight() const;
    const int& getWidth() const;
    const int& getRawHeight() const;
    const int& getRawWidth() const;
    const int& getChannels() const;
    const int& getDesiredChannels() const;
    unsigned char*& getDataPtr();

private:
    std::string filepath;                              ///< Path to the image file.
    unsigned char* dataPtr;                            ///< Pointer to image data.
    std::vector<std::vector<std::vector<int>>> data;   ///< Image data stored as a 3D vector.
    int width;                                         ///< Image width.
    int height;                                        ///< Image height.
    int channels;                                      ///< Number of image channels.
    int rawWidth;                                      ///< Raw image width, may differ from transformed width.
    int rawHeight;                                     ///< Raw image height, may differ from transformed height.
    int desiredChannels;                               ///< Desired number of channels for image processing.
};

#endif //ADVANCED_PROGRAMMING_GROUP_XAVIER_INITIALIZATION_IMAGE_H