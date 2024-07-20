# Introduction
Welcome to XIIA: Xavier Initialisation Image Application! 

<img src="Images/gracehopper.png" alt="before" style="display:inline-block; margin-right: 10px;width: 45%; height: auto;" /><img src="Output/7-edgedetection/prewitt/gracehopper.png" alt="Image 2" style="display:inline-block; margin-right: 10px;width: 45%; height: auto;" />

# How To Use
To utilize this tool effectively, users can follow the steps outlined below to navigate through the interfaces for 2D and 3D image processing. The executable files can be found in the `./release` folder.

```
.
├── release
│   ├── xavier_initialisation_application_mac
│   └── xavier_initialisation_application_win.exe
```

For macOS user, please granting executable permission before executing it.

```bash
cd ./release
ls -l xavier_initialisation_application_mac
chmod +x xavier_initialisation_application_mac
sudo ./xavier_initialisation_application_mac
```

## Environment 

- C++ Language Standard: Default (ISO C++ 17 Standard)

- For Windows Visual Studio user please set up the IDE following the instructions below:

  1. Open your Visual Studio project.

  2. Right-click the project you want to modify in the Solution Explorer and select "Properties".

  3. In the properties window that pops up, navigate to "Configuration Properties" > "C/C++" > "Preprocessor".

  4. In the "Preprocessor Definitions" section, click on the edit button.

  5. In the dialog that appears, add `_CRT_SECURE_NO_WARNINGS`.

  6. Click "OK" to save your changes.

## General Steps
1. **Start the Program**: Run the executable to start the program. This brings up the main menu.
2. **Choose between 2D and 3D**: At the main menu, select whether you wish to work with 2D or 3D images. Input `1` for 2D and `2` for 3D processing. To exit the program, enter `0`.

## For 2D Image Processing
1. **Enter the Path of the Input Image**: After selecting 2D processing, you'll be prompted to enter the path to your image file.
2. **Choose a Filter**: You will see a list of available filters (e.g., Grayscale, Brightness, Histogram Equalisation, etc.). Enter the number corresponding to the filter you wish to apply.
3. **Configure the Filter (if required)**: Some filters might require additional input, such as brightness value or kernel size. Follow the prompts to input these values.
4. **Save Processed Image**: After applying a filter, you'll be prompted to enter a path to save the processed image.
5. **Continue or Exit**: Decide if you want to apply another filter to the same image or finish processing. To add another filter, input `y` when asked; to finish, input `n`.

## For 3D Image Processing
1. **Enter the Path of the Input Volume**: After selecting 3D processing, you'll first be prompted to enter the path to your 3D volume data.
2. **Choose a Filter or Operation**: You will see a menu for selecting a 3D filter (e.g., Gaussian Filter, Median Filter) or operation (e.g., Projection, Slice). Select the desired option by entering its corresponding number.
3. **Configure the Filter or Operation (if required)**: Depending on your choice, you might need to specify additional parameters, such as filter size or sigma for Gaussian filtering, or select slices for projection.
4. **Save Processed Volume or Slices**: After applying a filter or operation, you'll be prompted to enter a path to save the output.
5. **Continue, Back, or Exit**: After each operation, you can choose to apply another filter or operation, go back to the previous menu, or exit the program. Select the appropriate option as per your needs.

## Exiting the Program
- **Exit at Any Time**: You can exit the program at any time by selecting the `Exit` or `Back` option available in the menus, eventually leading you back to the main menu where you can exit the program by entering `0`.

# CT Scans
Download CT Scan datasets here:
https://imperiallondon-my.sharepoint.com/:u:/g/personal/tmd02_ic_ac_uk/EafXMuNsbcNGnRpa8K62FjkBvIKvCswl1riz7hPDHpHdSQ

# Reference
<a id="[1]">[1]</a> GitHub and OpenAI, "GitHub Copilot," 2024. [Online]. Available: https://copilot.github.com.

<a id="[2]">[2]</a> [mraggi/tqdm-cpp: Easily display progress in C++17. Inspired by python's awesome tqdm library. (github.com)](https://github.com/mraggi/tqdm-cpp) 

