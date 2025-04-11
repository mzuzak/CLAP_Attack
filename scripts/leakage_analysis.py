import json
import os
import re

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statsmodels.api as sm
from scipy.optimize import curve_fit

file_structure = {
    'b14_BA16_gate_level_final.v.bench': {'circuit': 'b14', 'locking_method': 'AntiSAT'},
    'b14_locked_40_40_gate_level_final.v.bench': {'circuit': 'b14', 'locking_method': 'Full-Lock'},
    'b14_SFLL_277_gate_level_final.v.benchfix': {'circuit': 'b14', 'locking_method': 'SFLL'},
    'b14_SLL.bench': {'circuit': 'b14', 'locking_method': 'SLL'},
    'c1908_BA16_gate_level_final.v.bench': {'circuit': 'c1908', 'locking_method': 'AntiSAT'},
    'c1908_locked_32_32_gate_level_final.v.bench': {'circuit': 'c1908', 'locking_method': 'Full-Lock'},
    'c1908_33_gate_level_final.v.bench': {'circuit': 'c1908', 'locking_method': 'SFLL'},
    'c1908_SLL.bench': {'circuit': 'c1908', 'locking_method': 'SLL'},
    'c5315_BA16_gate_level_final.v.bench': {'circuit': 'c5315', 'locking_method': 'AntiSAT'},
    'c5315_locked_40_40_gate_level_final.v.bench': {'circuit': 'c5315', 'locking_method': 'Full-Lock'},
    'c5315_SFLL_178_gate_level_final.v.bench': {'circuit': 'c5315', 'locking_method': 'SFLL'},
    'c5315_sll.bench': {'circuit': 'c5315', 'locking_method': 'SLL'},
    'des_BA16_gate_level_final.v.bench': {'circuit': 'des', 'locking_method': 'AntiSAT'},
    'des_locked_40_40_gate_level_final.v.bench': {'circuit': 'des', 'locking_method': 'Full-Lock'},
    'des_SFLL_256_gate_level_final.v.benchfix': {'circuit': 'des', 'locking_method': 'SFLL'},
    'des_SLL.bench': {'circuit': 'des', 'locking_method': 'SLL'}
}


####################################################################################################
# Identifying Prior Circuits with No Results
#################################################################################################### 

def find_no_results_entries():
    df = pd.read_csv('./results.csv')
    filtered_df = df[df['Full Key Leakage'] == 'No results found']
    unique_prior_circuits = filtered_df['Prior Circuit'].drop_duplicates()
    return unique_prior_circuits.tolist()


def print_no_results_entries():
    no_result_circuits = find_no_results_entries()
    print("Prior Circuits with 'No results found' for Full Key Leakage:")
    for circuit in no_result_circuits:
        print(circuit)


# these were found to have no results
exclude_prior_circuits = ['modified_8x_b05.bench', 'b05.bench', 'modified_5x_b05.bench']

####################################################################################################
# Idenitfying Universal Prior Circuits
####################################################################################################

def normalize_prior_circuit_name(name):
    """Normalize prior circuit names by removing a varying prefix ending with '.bench'."""
    if pd.isna(name):
        return None

    normalized_name = re.sub(r'^modified_[^_]+_', '', name)

    # print(f"Original: {name} - Normalized: {normalized_name}")
    return normalized_name

def find_compatible_priors(df):
    """Find prior circuits compatible with all locked circuits"""
    # Group by 'Locked Circuit' and collect normalized priors
    compatible = None
    groups = df.groupby('Locked Circuit')
    for name, group in groups:
        current_priors = set(group['Normalized Prior Circuit'].dropna().unique())
        if compatible is None:
            compatible = current_priors
        else:
            compatible.intersection_update(current_priors)

    compatible = sorted(compatible)

    return compatible

def find_universal_priors_main():
    df = pd.read_csv('./results.csv')
    df = preprocess_data(df)
    compatible_priors = find_compatible_priors(df)
    print("Prior circuits compatible with all locked circuits:")
    print(compatible_priors)

####################################################################################################
# Data Collection
####################################################################################################

def extract_leakage_info(leakage_str):
    """ Extract the number of leaked bits, total keys, and probes from the leakage string. """
    numbers = re.findall(r'\b\d+\b', leakage_str)
    if len(numbers) >= 3:
        found_keys, total_keys, probes = map(int, numbers[:3])
        return found_keys, total_keys, probes
    return None, None, None


def preprocess_data(df):
    # Ensure that we are not assigning to a copy of a slice from a DataFrame
    df = df.copy()

    # Remove rows with prior circuits that have no results (invalid data)
    df = df[~df['Prior Circuit'].isin(exclude_prior_circuits)]

    df['Normalized Circuit'] = df['Locked Circuit'].apply(lambda x: file_structure[x]['circuit'] if x in file_structure else 'Unknown')
    df['Normalized Locking Method'] = df['Locked Circuit'].apply(lambda x: file_structure[x]['locking_method'] if x in file_structure else 'Unknown')
    df['Normalized Prior Circuit'] = df['Prior Circuit'].apply(normalize_prior_circuit_name)

    # Unpack the tuple returned by extract_leakage_info into separate columns
    extracted_data = df['Full Key Leakage'].apply(extract_leakage_info)
    df['Found Keys'], df['Total Keys'], df['Probes'] = zip(*extracted_data)
    
    # Calculate the leakage ratio safely
    df['Leakage Ratio'] = df['Found Keys'] / df['Total Keys'].replace(0, np.nan)
    
    # Convert Unroll Factor to numeric type
    df['Unroll Factor'] = pd.to_numeric(df['Unroll Factor'], errors='coerce')
    
    # Fill NA and strip spaces from 'Prior Circuit'
    df['Prior Circuit'] = df['Prior Circuit'].fillna('').str.strip()
    
    # Apply mapping for circuit and locking method using loc
    df.loc[:, 'Circuit'] = df['Locked Circuit'].apply(lambda x: file_structure[x]['circuit'])
    df.loc[:, 'Locking Method'] = df['Locked Circuit'].apply(lambda x: file_structure[x]['locking_method'])

    return df


def analyze_trends(group):
    results = {}
    prior_circuits = group['Prior Circuit'].unique()
    for circuit in prior_circuits:
        circuit_data = group[group['Prior Circuit'] == circuit]
        for unroll_factor in circuit_data['Unroll Factor'].unique():
            subset = circuit_data[circuit_data['Unroll Factor'] == unroll_factor]
            if len(subset) > 1 and len(subset['Unroll Factor'].unique()) > 1:
                model = sm.OLS(subset['Leakage Ratio'], sm.add_constant(subset['Unroll Factor'])).fit()
                if len(model.params) > 1:
                    results[(circuit, unroll_factor)] = model.params[1]
                else:
                    results[(circuit, unroll_factor)] = np.nan
            else:
                results[(circuit, unroll_factor)] = np.nan
    return results


def analyze_leakage_changes(group):
    changes = {}
    prior_circuits = group['Prior Circuit'].unique()
    for prior_circuit in prior_circuits:
        if prior_circuit == '':
            continue
        circuit_data = group[group['Prior Circuit'] == prior_circuit].sort_values(by='Unroll Factor')
        first_keys_leaked = None
        for _, row in circuit_data.iterrows():
            if first_keys_leaked is None:
                first_keys_leaked = row['Found Keys']
            if first_keys_leaked is not None and row['Found Keys'] > first_keys_leaked:
                additional_bits_leaked = row['Found Keys'] - first_keys_leaked
                key = (row['Locked Circuit'], row['Locking Method'], prior_circuit)
                if key not in changes or changes[key]['Additional_Bits_Leaked'] < additional_bits_leaked:
                    changes[key] = {
                        "Locked Circuit": row['Locked Circuit'],
                        "Method": row['Locking Method'],
                        "Circuit": prior_circuit,
                        "Additional_Bits_Leaked": additional_bits_leaked
                    }
    sorted_changes = sorted(changes.values(), key=lambda x: x['Additional_Bits_Leaked'], reverse=True)
    return sorted_changes[:5]


def plot_trends(data):
    unique_prior_circuits = data['Prior Circuit'].unique()
    plt.figure(figsize=(12, 8))
    for circuit in unique_prior_circuits:
        subset = data[data['Prior Circuit'] == circuit]
        plt.plot(subset['Unroll Factor'], subset['Leakage Ratio'], marker='o', label=circuit)
    
    plt.title('Leakage Ratio vs. Unroll Factor')
    plt.xlabel('Unroll Factor')
    plt.ylabel('Leakage Ratio')
    plt.legend()
    plt.grid(True)
    plt.show()


def adjust_leakage_values(row, total_keys):
    # Calculate non-leaked keys ensuring it does not go below zero
    non_leaked_keys = total_keys - (row['Max Leakage'] + row['Max Partial Leakage'])
    if non_leaked_keys < 0:
        # If the sum of Max Leakage and Max Partial Leakage exceeds total keys,
        # adjust Max Partial Leakage to ensure the sum equals total keys
        row['Max Partial Leakage'] -= abs(non_leaked_keys)
        non_leaked_keys = 0
    return row['Max Leakage'], row['Max Partial Leakage'], non_leaked_keys


def plot_leakage_histograms(control_data, test_data):
    # Assume total_keys is a constant value across all rows of control_data
    total_keys = control_data['Total Keys'].iloc[0]

    # Calculate the maximum number of found keys and partial key leakage
    max_leakage = test_data.groupby('Prior Circuit')['Found Keys'].max()
    max_partial_leakage = test_data.groupby('Prior Circuit')['Partial Key Leakage'].max()

    # Creating DataFrame for easier manipulation
    df = pd.DataFrame({
        'Max Leakage': max_leakage,
        'Max Partial Leakage': max_partial_leakage
    }).reset_index()

    # Adjust Max Leakage and Max Partial Leakage values
    df[['Max Leakage', 'Max Partial Leakage', 'Non-Leaked Keys']] = df.apply(
        lambda row: adjust_leakage_values(row, total_keys), axis=1, result_type="expand"
    )

    # Adding control data to DataFrame, adjusted as well
    control_row = adjust_leakage_values({
        'Max Leakage': control_data['Found Keys'].max(),
        'Max Partial Leakage': control_data['Partial Key Leakage'].max()
    }, total_keys)
    
    df.loc[-1] = ['Control'] + list(control_row)
    df.index = df.index + 1
    df = df.sort_index()

    # Sort labels alphanumerically and fix the 'Prior Circuit' values
    df['Prior Circuit'] = df['Prior Circuit'].apply(lambda x: re.search(r'(b\d+\.bench)$', x).group(1) if re.search(r'(b\d+\.bench)$', x) else x)
    df.sort_values('Prior Circuit', inplace=True)

    # Insert position for the average bars just after the control bar
    average_index = df.index[df['Prior Circuit'] == 'Control'][0] + 1

    # Calculate average for non-control data
    non_control_data = df[df['Prior Circuit'] != 'Control']
    avg_full_leakage = non_control_data['Max Leakage'].mean()
    avg_partial_leakage = non_control_data['Max Partial Leakage'].mean()
    avg_non_leaked = total_keys - (avg_full_leakage + avg_partial_leakage)

    # Insert the average data into the DataFrame
    df = pd.concat([
        df.iloc[:average_index],
        pd.DataFrame({
            'Prior Circuit': ['Average'],
            'Max Leakage': [avg_full_leakage],
            'Max Partial Leakage': [avg_partial_leakage],
            'Non-Leaked Keys': [avg_non_leaked]
        }),
        df.iloc[average_index:]
    ]).reset_index(drop=True)

    # Adjusting plot parameters
    plt.figure(figsize=(12, 8))
    x = np.arange(len(df))  # the label locations
    width = 0.4  # the width of the bars

    # Stacked bar plot for full and partial key leakage
    plt.bar(x, df['Max Leakage'], width, label='Max Full Leakage', color='green')
    plt.bar(x, df['Max Partial Leakage'], width, bottom=df['Max Leakage'], label='Max Partial Leakage', color='yellow')
    plt.bar(x, df['Non-Leaked Keys'], width, bottom=df['Max Leakage'] + df['Max Partial Leakage'], label='Non-Leaked Keys', color='red')

    plt.xlabel('Prior Sequential Circuit', fontsize=16)
    plt.ylabel('# of key bits', fontsize=16)
    plt.title(f'Comparison of Obtained Full and Partial Key Leakage\n{control_data.iloc[0]["Circuit"]} - {control_data.iloc[0]["Locking Method"]}, Key Length: {total_keys}', fontsize=18)
    plt.xticks(x, df['Prior Circuit'], rotation=45, fontsize=14)
    plt.legend(loc='upper right')

    plt.ylim(top=total_keys * 1.1)

    plt.tight_layout()
    plt.savefig(f'./figures/{control_data.iloc[0]["Circuit"]}_{control_data.iloc[0]["Locking Method"]}_leakage_comparison_plot.png', dpi=300, bbox_inches='tight')  # Save as PNG file with high resolution



def plot_grouped_leakage_histograms(grouped_data, group_title, filename):
    # Determine the number of subplots needed
    num_subplots = len(grouped_data)
    cols = 2
    rows = num_subplots // cols + (num_subplots % cols > 0)
    
    fig, axs = plt.subplots(rows, cols, figsize=(15, rows * 5))
    axs = axs.flatten()  # Flatten in case we have a grid

    for idx, (group_key, data) in enumerate(grouped_data.items()):
        # Calculate max leakage for each group
        max_leakage = data.groupby('Prior Circuit')['Found Keys'].max()
        max_partial_leakage = data.groupby('Prior Circuit')['Partial Key Leakage'].max()
        total_keys = data['Total Keys'].max()
        
        # Creating DataFrame for easier manipulation
        df = pd.DataFrame({'Max Leakage': max_leakage, 'Max Partial Leakage': max_partial_leakage}).reset_index()
        df['Prior Circuit'] = df['Prior Circuit'].apply(lambda x: re.search(r'(b\d+\.bench)$', x).group(1) if re.search(r'(b\d+\.bench)$', x) else x)
        
        # Plotting
        axs[idx].bar(df['Prior Circuit'], df['Max Leakage'], width=0.4, label='Max Full Leakage', color='skyblue')
        axs[idx].bar(df['Prior Circuit'], df['Max Partial Leakage'], width=0.4, label='Max Partial Leakage', color='orange', bottom=df['Max Leakage'])
        axs[idx].set_title(f'{group_title} {group_key}')
        axs[idx].set_xlabel('Prior Sequential Circuit')
        axs[idx].set_ylabel('# of key bits')
        axs[idx].tick_params(labelrotation=45)
        axs[idx].legend()
    
    # Remove empty subplots
    for ax in axs[num_subplots:]:
        ax.remove()
    
    plt.tight_layout()
    plt.savefig(f'./figures/{filename}.png', dpi=300)
    plt.close()


def analyze_leakage_stabilization(df):

    # Filter out rows where 'Probe Resolution' is a number
    df = df[pd.to_numeric(df['Probe Resolution'], errors='coerce').isna()]

    df = df.dropna(subset=['Found Keys', 'Partial Key Leakage', 'Unroll Factor'])

    # Prepare data structures to store the stabilization points
    stabilization_full = []
    stabilization_partial = []

    grouped = df.groupby(['Locked Circuit', 'Prior Circuit'])
    count = 0
    for (locked_circuit, prior_circuit), group in grouped:
        count += 1
        # print(f"Analyzing {locked_circuit} - {prior_circuit} ({count}/{len(grouped)})...")

        group = group.sort_values(by='Unroll Factor')
        last_full_leakage = None
        last_partial_leakage = None
        full_stable_at = None
        partial_stable_at = None

        for i, row in group.iterrows():

            current_full_leakage = row['Found Keys']
            current_partial_leakage = row['Partial Key Leakage']
            current_unroll_factor = row['Unroll Factor']

            if full_stable_at is None:
                full_stable_at = current_unroll_factor

            if partial_stable_at is None:
                partial_stable_at = current_unroll_factor

            if last_full_leakage is not None:
                if last_full_leakage != current_full_leakage:
                    full_stable_at = current_unroll_factor

            if last_partial_leakage is not None:
                if last_partial_leakage != current_partial_leakage:
                    partial_stable_at = current_unroll_factor



            last_full_leakage = current_full_leakage
            last_partial_leakage = current_partial_leakage

        # Only append to the list if a stabilization point was found and did not change afterwards
        if full_stable_at is not None:
            stabilization_full.append(full_stable_at)
        if partial_stable_at is not None:
            stabilization_partial.append(partial_stable_at)

    return stabilization_full, stabilization_partial


def find_max_with_buffer(data, buffer_size=2):
    """ Find the maximum value in data with a specified buffer (additional bins). """
    if data:
        max_value = max(data)
        return max_value + buffer_size
    return buffer_size


def plot_stabilization_histogram(stabilization_full, stabilization_partial):

    def exp_fit(x, a, b, c):
        return a * np.exp(-b * x) + c


    def plot_histogram(data, color, title, filename):
        # Define bins such that each integer value falls in the middle of a bin
        bins = np.arange(1.5, max(data) + 2.5, 1)
        # Plot histogram without density to show actual counts
        plt.figure(figsize=(12, 6))
        counts, _, _ = plt.hist(data, bins=bins, color=color, alpha=0.6, edgecolor='black', align='mid')

        # Add a title and labels
        plt.yscale('log')
        plt.title(title)
        plt.xlabel('Unroll Factor at Which Key Leakage Stabilizes')
        plt.ylabel('# of Key Bits with Leakage')
        plt.xticks(np.arange(2, max(data) + 2, 1))
        # plt.grid(True)

        # Annotate with actual counts above each bar
        for i, count in enumerate(counts):
            plt.text(bins[i] + 0.5, count, str(int(count)), ha='center', va='bottom')

        # Save the plot
        plt.savefig(f'./figures/{filename}.png', dpi=300)
        plt.close()

    plot_histogram(stabilization_full, 'skyblue', 'Full Key Leakage Stabilization Points', 'full_leakage_stabilization')
    plot_histogram(stabilization_partial, 'orange', 'Partial Key Leakage Stabilization Points', 'partial_leakage_stabilization')


def create_combined_figure(image_paths, output_path):
    fig, axes = plt.subplots(2, 2, figsize=(8, 6))  # Create a 2x2 grid of subplots
    axes = axes.flatten()  # Flatten the 2D array of axes for easy iteration

    for ax, image_path in zip(axes, image_paths):
        if os.path.exists(image_path):  # Check if the image file exists
            image = plt.imread(image_path)
            ax.imshow(image)
            ax.axis('off')  # Hide the axis
        else:
            ax.axis('off')  # Hide the axis if image does not exist

    plt.tight_layout()  # Adjust layout to prevent overlap
    plt.savefig(output_path, dpi=300)  # Save the combined figure
    plt.close(fig)  # Close the figure to free memory


def synthesize_group_figures():
    input_dir = "./figures/"
    output_dir = "./figures/grouped/"
    os.makedirs(output_dir, exist_ok=True)
    postfix = "_leakage_comparison_plot.png"
    circuits = ["b14", "c1908", "c5315", "des"]
    locking_methods = ["AntiSAT", "Full-Lock", "SFLL", "SLL"]

    # Create a figure for each circuit
    for circuit in circuits:
        image_paths = [f"{input_dir}{circuit}_{method}{postfix}" for method in locking_methods]
        output_path = f'{output_dir}{circuit}_grouped_leakage_comparison.png'
        print(f"Generating {output_path}...")
        create_combined_figure(image_paths, output_path)

    # Create a figure for each locking method
    for method in locking_methods:
        image_paths = [f"{input_dir}{circuit}_{method}{postfix}" for circuit in circuits]
        output_path = f'{output_dir}{method}_grouped_leakage_comparison.png'
        print(f"Generating {output_path}...")
        create_combined_figure(image_paths, output_path)


def generate_probe_res_plot(df):
    df = df.copy()

    # Filter out rows without a probe resolution, and those with a prior circuit
    df = df[df['Probe Resolution'].notnull()]
    df = df[df['Unroll Factor'].isnull()]

    df['Probe Resolution'] = pd.to_numeric(df['Probe Resolution'], errors='coerce')

    # Aggregate total leakage for each probe resolution
    total_leakage = df.groupby('Probe Resolution')['Found Keys'].sum()
    total_partial_leakage = df.groupby('Probe Resolution')['Partial Key Leakage'].sum()

    # Plotting
    plt.figure(figsize=(12, 8))
    plt.plot(total_leakage.index, total_leakage, marker='o', linestyle='-', color='blue', label='Total Full Leakage')
    plt.plot(total_partial_leakage.index, total_partial_leakage, marker='o', linestyle=':', color='orange', label='Total Partial Leakage')

    plt.title('Total Key Leakage vs Probe Size')
    plt.xlabel('Probe Size')
    plt.ylabel('Total Number of Key Bits Leaked')
    # ticks every 2
    plt.xticks(np.arange(0, max(df['Probe Resolution']) + 1, 2))
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Ensure the figures directory exists
    os.makedirs('./figures', exist_ok=True)
    plt.savefig('./figures/total_probe_resolution.png', dpi=300)
    plt.close()


def generate_new_unroll_plot(df):
    df = df.copy()

    # Filter out rows without a probe resolution, and those with a prior circuit
    df = df[df['Probe Resolution'].isnull()]
    df = df[df['Unroll Factor'].notnull()]

    df['Unroll Factor'] = pd.to_numeric(df['Unroll Factor'], errors='coerce')

    # Aggregate total leakage for each probe resolution
    total_leakage = df.groupby('Unroll Factor')['Found Keys'].sum()
    total_partial_leakage = df.groupby('Unroll Factor')['Partial Key Leakage'].sum()

    # Find the max in both total and partial, find the earliest unroll cycles where this is 
    max_unroll = total_leakage.idxmax()

    # Plotting
    plt.figure(figsize=(12, 8))
    plt.plot(total_leakage.index, total_leakage, marker='o', linestyle='-', color='blue', label='Total Full Leakage')
    plt.plot(total_partial_leakage.index, total_partial_leakage, marker='o', linestyle=':', color='orange', label='Total Partial Leakage')

    plt.title('Total Key Leakage vs Simulated Cycles')
    plt.xlabel('Simulated Cycles')
    plt.xlim(0, max_unroll+2)

    plt.ylabel('Total Number of Key Bits Leaked')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Ensure the figures directory exists
    os.makedirs('./figures', exist_ok=True)
    plt.savefig('./figures/total_cycles.png', dpi=300)
    plt.close()


def generate_unroll_and_probe_line_plot(df):
    df = df.copy()

    # Filter out rows without a probe resolution, and those with a prior circuit
    df = df[df['Probe Resolution'].notnull()]
    df = df[df['Unroll Factor'].notnull()]

    df['Unroll Factor'] = pd.to_numeric(df['Unroll Factor'], errors='coerce')
    df['Probe Resolution'] = pd.to_numeric(df['Probe Resolution'], errors='coerce')

    # Aggregate total leakage for each probe resolution
    total_leakage = df.groupby('Probe Resolution')['Found Keys'].sum() / 12
    total_partial_leakage = df.groupby('Probe Resolution')['Partial Key Leakage'].sum() / 12

    # Plotting
    plt.figure(figsize=(12, 8))
    plt.plot(total_leakage.index, total_leakage, marker='o', linestyle='-', color='blue', label='Total Full Leakage')
    plt.plot(total_partial_leakage.index, total_partial_leakage, marker='o', linestyle=':', color='orange', label='Total Partial Leakage')

    plt.title('Total Key Leakage vs Probe Resolution')
    plt.xlabel('Probe Resolution')
    plt.xticks(np.arange(0, max(df['Probe Resolution']) + 1, 2))
    plt.ylabel('Total Number of Key Bits Leaked')
    plt.ylim(0, None)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Ensure the figures directory exists
    os.makedirs('./figures', exist_ok=True)
    plt.savefig('./figures/total_probe_and_unroll.png', dpi=300)
    plt.close()


def generate_combined_soloprobe_and_unrolled_line_plot(df):
    df = df.copy()
    
    # Convert 'Probe Resolution' to numeric and filter out rows without a probe resolution
    df['Probe Resolution'] = pd.to_numeric(df['Probe Resolution'], errors='coerce')
    df = df[df['Probe Resolution'].notnull()]

    # Aggregate total leakage for each probe resolution for scenarios with and without Unroll Factor
    df_with_unroll = df[df['Unroll Factor'].notnull()]
    df_without_unroll = df[df['Unroll Factor'].isnull()]

    # Divide by 12 because there are 12 different prior circuits, take the average
    total_leakage_with = df_with_unroll.groupby('Probe Resolution')['Found Keys'].sum() / 12
    total_partial_leakage_with = df_with_unroll.groupby('Probe Resolution')['Partial Key Leakage'].sum() / 12

    total_leakage_without = df_without_unroll.groupby('Probe Resolution')['Found Keys'].sum()
    total_partial_leakage_without = df_without_unroll.groupby('Probe Resolution')['Partial Key Leakage'].sum()

    # Plotting
    plt.figure(figsize=(12, 8))

    # Plot for total leakages with Unroll Factor
    plt.plot(total_leakage_with.index, total_leakage_with, marker='o', linestyle='-', color='blue', label='Full Leakage with Prior Stage')
    plt.plot(total_partial_leakage_with.index, total_partial_leakage_with, marker='o', linestyle=':', color='blue', label='Partial Leakage with Prior Stage')

    # Plot for total leakages without Unroll Factor
    plt.plot(total_leakage_without.index, total_leakage_without, marker='s', linestyle='-', color='orange', label='Full Leakage without Prior Stage')
    plt.plot(total_partial_leakage_without.index, total_partial_leakage_without, marker='s', linestyle=':', color='orange', label='Partial Leakage without Prior Stage')

    plt.title('Key Leakage vs Probe Size')
    plt.xlabel('Probe Size')
    plt.xticks(np.arange(0, max(df['Probe Resolution']) + 1, 2))
    plt.ylabel('Total Number of Key Bits Leaked')
    plt.ylim(0, None)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Ensure the figures directory exists
    os.makedirs('./figures', exist_ok=True)
    plt.savefig('./figures/total_probe_resolution_combined.png', dpi=300)
    plt.close()


def generate_execution_time_plots(df):
    df = df.copy()
    
    # Convert columns to numeric and handle any conversion errors
    df['Execution Time'] = pd.to_numeric(df['Execution Time'], errors='coerce')
    df['Unroll Factor'] = pd.to_numeric(df['Unroll Factor'], errors='coerce')
    df['Probe Resolution'] = pd.to_numeric(df['Probe Resolution'], errors='coerce')
    
    # Drop entries without execution time to ensure we only plot valid data
    df = df.dropna(subset=['Execution Time'])

    # Isolate data sets where either unroll factor or probe resolution is known
    unroll_only_df = df[df['Probe Resolution'].isnull() & df['Unroll Factor'].notnull()]
    probe_only_df = df[df['Unroll Factor'].isnull() & df['Probe Resolution'].notnull()]

    # Aggregate execution times for each unique unroll factor and probe resolution and normalize by the number of runs
    unroll_aggregated = unroll_only_df.groupby('Unroll Factor')['Execution Time'].sum() / 192
    probe_aggregated = probe_only_df.groupby('Probe Resolution')['Execution Time'].sum() / 16

    # Plotting
    plt.figure(figsize=(12, 6))

    # Execution Time vs. Unroll Factor
    plt.subplot(1, 2, 1)
    plt.plot(unroll_aggregated.index, unroll_aggregated, marker='o', linestyle='-', color='blue')
    plt.title('Average Execution Time vs. Simulated Cycles')
    plt.xlabel('Simulated Cycles')
    plt.ylabel('Average Execution Time (seconds)')
    plt.grid(True)

    # Execution Time vs. Probe Resolution
    plt.subplot(1, 2, 2)
    plt.plot(probe_aggregated.index, probe_aggregated, marker='o', linestyle='-', color='orange')
    plt.title('Average Execution Time vs. Probe Size')
    plt.xticks(np.arange(0, max(probe_aggregated.index), 2))
    plt.xlabel('Probe Size')
    plt.ylabel('Average Execution Time (seconds)')
    plt.grid(True)

    plt.tight_layout()

    # Ensure the figures directory exists and save the plot
    os.makedirs('./figures', exist_ok=True)
    plt.savefig('./figures/aggregated_execution_time_plots.png', dpi=300)
    plt.close()


def data_collection_main():
    df = pd.read_csv('./results.csv')
    df = preprocess_data(df)

    ########################################################
    # Execution Time Analysis
    ########################################################
    generate_execution_time_plots(df)

    ########################################################
    # Probe Resolution Analysis
    ########################################################
    generate_probe_res_plot(df)

    ########################################################
    # Unroll Factor Analysis
    ########################################################
    generate_new_unroll_plot(df)

    ########################################################
    # Combined Analysis
    ########################################################
    generate_unroll_and_probe_line_plot(df)

    generate_combined_soloprobe_and_unrolled_line_plot(df)

    ########################################################
    # Leakage Stabilization Analysis
    ########################################################
    # Analyze leakage stabilization
    stabilization_full, stabilization_partial = analyze_leakage_stabilization(df)
    plot_stabilization_histogram(stabilization_full, stabilization_partial)

    # # Analyze trends in leakage 
    # change_in_leakage = analyze_leakage_changes(df)
    # print("Changes in Leakage:")
    # for change in change_in_leakage:
    #     print(change)


    ########################################################
    # Individual Leakage Analysis
    ########################################################
    # Analyze trends in leakage by group
    grouped = df.groupby('Locked Circuit')
    for name, group in grouped:
        print(f"\nAnalysis for Locked Circuit: {name}")
        print(f"Total Entries: {len(group)}")

        # Analyze trends within this specific group
        # trend_results = analyze_trends(group)
        # print("Trend Analysis Results:")
        # for key, value in trend_results.items():
        #     print(f"{key}: {value}")

        # Assuming we have separate control and test dataframes processed before
        control_data = group[group['Prior Circuit'] == '']
        test_data = group[group['Prior Circuit'] != '']

        if not control_data.empty and not test_data.empty:
            plot_leakage_histograms(control_data, test_data)




    synthesize_group_figures()


if __name__ == "__main__":
    data_collection_main()
    # find_universal_priors_main()


