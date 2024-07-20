import pandas as pd
import matplotlib.pyplot as plt

# Sample CSV data

# Reading the CSV data into a DataFrame
df = pd.read_csv("results_2D_Conv_num_pixels.csv")

# Plotting
fig, ax = plt.subplots(figsize=(12, 8))

# Line styles for each filter type
line_styles = ['-', ':', '-.', '--']

# Colors for each image
colors = ['b', 'g', 'r']

# Looping over each filter type
for i, filter_type in enumerate(['box', 'gaussian', 'median']):
    for j, size in enumerate(['3_3', '5_5', '7_7', '9_9']):
        if ((j == 1) or (j==2)):
            alpha = .4
        else:
            alpha = 1.
        # Column name pattern
        col_name = f'{filter_type}_{size}_ms'
        # Plotting each line
        ax.plot(df['num pixels'], df[col_name], label=f'{filter_type} {size}', color=colors[i], linestyle=line_styles[j], marker='o', markersize=3, alpha=alpha)

ax.set_xlabel('Number of Pixels (h, w, c)', fontsize=8)
ax.set_ylabel('Time (ms)', fontsize=8)
ax.set_title('Apply 2D convolution time (ms) for varying image size and kernel size', fontsize=12)
ax.legend(loc="best")
plt.tight_layout()
plt.show()
