import pandas as pd
import matplotlib.pyplot as plt
from io import StringIO

# Provided CSV data
csv_data = """
kernel size,median_mergesort (time/ms),median_insertsort (time/ms),median_quicksort (time/ms),median_quicksort_midpointPivot (time/ms),box (time/ms),gaussian (time/ms)
2x2,3325,1153,1166,1064,1100,1079
3x3,7736,1536,1602,1382,1391,1384
4x4,14157,2119,2283,1738,1740,1727
5x5,21866,2889,3195,2238,2143,2126
6x6,54894,4130,4607,2883,2730,2710
7x7,43754,5653,6213,3401,3277,3260
8x8,57101,7813,8248,4099,3996,3947
9x9,302262,10584,10847,4824,4665,4657
"""

# Reading CSV data into DataFrame
df = pd.read_csv(StringIO(csv_data))

# Plotting
fig, ax = plt.subplots(figsize=(10, 6))

# Line styles and colors
median_line_styles = ['-', ':', '-.', '--']
box_gaussian_line_style = '-'
colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k']
median_color = 'b'

# Plotting Median lines with different line styles
for i, column in enumerate(['median_quicksort (time/ms)', 'median_mergesort (time/ms)', 'median_insertsort (time/ms)', 'median_quicksort_midpointPivot (time/ms)']):
    if column == 'median_quicksort (time/ms)':
        linestyle = '-'
    else:
        linestyle = median_line_styles[i]
    ax.plot(df['kernel size'], df[column], label=column[:-9], color=median_color, linestyle=linestyle, marker='o', markersize=3)

# Plotting Box and Gaussian lines
ax.plot(df['kernel size'], df['box (time/ms)'], label='box', color='r', linestyle=box_gaussian_line_style, marker='o',linewidth=2, markersize=3)
ax.plot(df['kernel size'], df['gaussian (time/ms)'], label='gaussian', color='g', linestyle=box_gaussian_line_style, linewidth=1, marker='o', markersize=3, alpha=1)

ax.set_xlabel('Kernel Size', fontsize=14)
ax.set_ylabel('Time (ms)', fontsize=14)
ax.set_title('Comparison of Filtering Times by Kernel Size', fontsize=16)
ax.legend()
plt.grid(True)
plt.show()
