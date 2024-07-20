import os
import re
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import argparse

def parse_log_file(filepath):
    with open(filepath, 'r') as file:
        lines = file.readlines()
    
    execution_time = None
    parallel_efficiency = None
    speedup_rate = None
    
    for line in lines:
        if "Execution Time:" in line:
            execution_time = float(re.search(r'Execution Time: (\d+\.?\d*) ms\.', line).group(1))
        elif "Parallel Efficiency:" in line:
            parallel_efficiency = float(re.search(r'Parallel Efficiency: (\d+\.?\d*)', line).group(1))
        elif "Speedup Rate:" in line:
            speedup_rate = float(re.search(r'Speedup Rate: (\d+\.?\d*)', line).group(1))
    
    return execution_time, parallel_efficiency, speedup_rate

def extract_log_suffix(filepath, prefix):
    filename = os.path.basename(filepath)
    match = re.search(rf'{prefix}-Circuit_Optimizer-(\d+)\.log', filename)
    return int(match.group(1)) if match else None

def logs_to_dataframe(log_files):
    execution_times = []
    parallel_efficiencies = []
    speedup_rates = []
    log_suffixes = []

    for log_file in log_files:
        suffix = extract_log_suffix(log_file, time_prefix)
        execution_time, parallel_efficiency, speedup_rate = parse_log_file(log_file)
        if execution_time is not None and parallel_efficiency is not None and speedup_rate is not None:
            execution_times.append(execution_time)
            parallel_efficiencies.append(parallel_efficiency)
            speedup_rates.append(speedup_rate)
            log_suffixes.append(suffix)

    data = {
        'Log Index': log_suffixes,
        'Execution Time (ms)': execution_times,
        'Parallel Efficiency': parallel_efficiencies,
        'Speedup Rate': speedup_rates
    }
    return pd.DataFrame(data)

def plot_data(df, save_path, filename):
    sns.set(style="whitegrid")
    fig, ax1 = plt.subplots(figsize=(10, 6))

    sns.lineplot(x='Log Index', y='Execution Time (ms)', data=df, marker='o', ax=ax1, color='b')
    ax1.set_xlabel('Number of Threads')
    ax1.set_ylabel('Execution Time (ms)', color='b')
    ax1.tick_params(axis='y', labelcolor='b')

    ax2 = ax1.twinx()
    sns.lineplot(x='Log Index', y='Parallel Efficiency', data=df, marker='o', ax=ax2, color='r')
    ax2.set_ylabel('Parallel Efficiency', color='r')
    ax2.tick_params(axis='y', labelcolor='r')

    plt.title('Execution Time and Parallel Efficiency')

    # Add vertical dashed lines
    for log_index in df['Log Index']:
        ax1.axvline(x=log_index, color='grey', linestyle='--', linewidth=0.7)

    # Set x-ticks to correspond to Log Index values
    ax1.set_xticks(df['Log Index'])
    ax1.set_xticklabels(df['Log Index'])

    fig.tight_layout()

    # Save the plot
    if save_path:
        os.makedirs(save_path, exist_ok=True)
        plt.savefig(os.path.join(save_path, filename))
    else:
        plt.savefig(filename)

def main(time_prefix, log_path, save_path):
    log_files = [os.path.join(log_path, f) for f in os.listdir(log_path) if f.startswith(time_prefix) and f.endswith('.log')]
    log_files.sort(key=lambda f: extract_log_suffix(f, time_prefix))

    if not log_files:
        print(f"No log files found with prefix {time_prefix} in {log_path}")
        return

    print(f"Found log files: {log_files}")

    df = logs_to_dataframe(log_files)
    df.sort_values('Log Index', inplace=True)
    print("Data extracted from logs:")
    print(df)

    filename = f'{time_prefix}_visualization.png'
    plot_data(df, save_path, filename)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Visualize log data.')
    parser.add_argument('time', type=str, help='Time prefix for the log files (e.g., 20240521-152531)')
    parser.add_argument('log_path', type=str, help='Path to the log files')
    parser.add_argument('--save_path', type=str, default='', help='Path to save the visualization image')

    args = parser.parse_args()
    time_prefix = args.time
    main(args.time, args.log_path, args.save_path)
