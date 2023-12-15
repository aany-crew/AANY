import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

pdir = "/Users/noahmarkowitz/Documents/GitHub/AANY"

# Read csv output file
fname = pdir + os.sep + "analysis/output.csv"
df = pd.read_csv(fname)

PLOT_GRAPHS = False
SAVE_GRAPHS = True

plt.clf()
plt.cla()

#region Compression and decompression time vs n_characters
df = df.sort_values("n_chracters")
plt.plot(df["n_chracters"], df["compression_time"], 'b*', markersize=12, label="Compression Time")
plt.plot(df["n_chracters"], df["compression_time"], 'b')
plt.plot(df["n_chracters"], df["decompression_time"], 'r*', markersize=12, label="Decompression Time")
plt.plot(df["n_chracters"], df["decompression_time"], 'r')
plt.xscale("log")
plt.yscale("log")
plt.title("Compression and Decompression vs. Number of characters")
plt.xlabel("Number of characters")
plt.ylabel("Time (seconds)")
plt.legend()

if PLOT_GRAPHS:
    plt.show()
if SAVE_GRAPHS:
    plt.savefig('analysis/fig1.png')

plt.cla()
plt.clf()

#endregion

#region Compression and decompression time graphs vs. # unique characters
#df2 = df[df["file_name"].str.contains(".csv")==False]
df2 = df.copy()
df2 = df2.sort_values("n_unique_characters")
plt.plot(df2["n_unique_characters"], df2["compression_time"], 'b*', markersize=12, label="Compression Time")
plt.plot(df2["n_unique_characters"], df2["compression_time"], 'b')
plt.plot(df2["n_unique_characters"], df2["decompression_time"], 'r*', markersize=12, label="Decompression Time")
plt.plot(df2["n_unique_characters"], df2["decompression_time"], 'r')
#plt.xscale("log")
plt.yscale("log")
plt.title("Compression and Decompression vs. Number of unique characters")
plt.xlabel("Number of unique characters")
plt.ylabel("Time (seconds)")
plt.legend()
if PLOT_GRAPHS:
    plt.show()
if SAVE_GRAPHS:
    plt.savefig('analysis/fig2.png')

plt.cla()
plt.clf()

#endregion

#region Scanning text vs # of characters
df = df.sort_values("n_chracters")
plt.plot(df["n_chracters"], df["get_freq_time"], 'c*', markersize=12)
plt.plot(df["n_chracters"], df["get_freq_time"], 'c')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Get character frequency vs. Number of characters")
plt.xlabel("Number of characters")
plt.ylabel("Get Character Frequency (seconds)")
#plt.legend()
if PLOT_GRAPHS:
    plt.show()
if SAVE_GRAPHS:
    plt.savefig('analysis/fig3.png')

plt.cla()
plt.clf()

#endregion

#region Decompression vs Compression
df = df.sort_values("compression_time")
plt.plot(df["compression_time"], df["decompression_time"], 'm*', markersize=12)
plt.plot(df["compression_time"], df["decompression_time"], 'm')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Decompression vs Compression time")
plt.xlabel("Compression Time")
plt.ylabel("Decompression Time")
#plt.legend()
if PLOT_GRAPHS:
    plt.show()
if SAVE_GRAPHS:
    plt.savefig('analysis/fig4.png')

plt.cla()
plt.clf()

#endregion

#region Generate Codes and Build Trees (ODD/BAD)
df = df.sort_values("n_unique_characters")
plt.plot(df["n_unique_characters"], df["generate_code_time"], 'k*', markersize=12, label="Generate Codes")
plt.plot(df["n_unique_characters"], df["generate_code_time"], 'k')
plt.plot(df["n_unique_characters"], df["build_tree_time"], 'y*', markersize=12, label="Build Tree")
plt.plot(df["n_unique_characters"], df["build_tree_time"], 'y')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Generate Huffman Code vs. Time")
plt.xlabel("Number Unique Characters")
plt.ylabel("Tree Traversal Time (seconds)")
ax = plt.gca()
ax.ticklabel_format(axis="y", style="sci", scilimits=(-7,-4))
plt.legend()
if PLOT_GRAPHS:
    plt.show()
if SAVE_GRAPHS:
    plt.savefig('analysis/fig5.png')

plt.cla()
plt.clf()

#endregion

#region Plot avg_bit_size vs tree difference

plt.cla()
plt.clf()

df = df.sort_values("height_diff")
plt.plot(df["height_diff"], df["decompression_time"], 'k*', markersize=12, label="Generate Codes")
plt.plot(df["height_diff"], df["decompression_time"], 'k')
plt.plot(df["n_unique_characters"], df["build_tree_time"], 'y*', markersize=12, label="Build Tree")
plt.plot(df["n_unique_characters"], df["build_tree_time"], 'y')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Generate Huffman Code vs. Time")
plt.xlabel("Number Unique Characters")
plt.ylabel("Tree Traversal Time (seconds)")
plt.legend()
if PLOT_GRAPHS:
    plt.show()
#if SAVE_GRAPHS:
    #plt.savefig('analysis/fig6.png')

#endregion


#region Modeling and Fitting

if False:
    import pandas as pd
    import statsmodels.api as sm
    import matplotlib.pyplot as plt

    # Read the CSV file into a DataFrame
    file_path = 'your_file.csv'  # Replace with the actual file path
    data = pd.read_csv(file_path)

    # Select relevant columns for the model
    selected_columns = ['decompression_time', 'tree_height', 'height_diff', 'n_chracters']
    df = data[selected_columns]

    # Drop rows with missing values, if any
    df = df.dropna()

    # Define the independent variables (X) and the dependent variable (y)
    X = df[['tree_height', 'height_diff', 'n_chracters']]
    y = df['decompression_time']

    # Add a constant term to the independent variables for the intercept
    X = sm.add_constant(X)

    # Fit the linear regression model
    model = sm.OLS(y, X).fit()

    # Print the model summary
    print(model.summary())

    # Plot the predicted values against the actual values
    fig, ax = plt.subplots()
    ax.scatter(y, model.fittedvalues)
    ax.set_xlabel('Actual Decompression Time')
    ax.set_ylabel('Predicted Decompression Time')
    ax.set_title('Actual vs Predicted Decompression Time')
    plt.xscale("log")

    # Display the plot
    plt.show()

#endregion

#region Basic statistics

# Compression ratio
compression_ratio = df["n_chracters"]*8/df["n_bits"]
compression_ratio_mean = compression_ratio.mean()
compression_ratio_std = np.std(compression_ratio)
compression_ratio_min = compression_ratio.min()
compression_ratio_max = compression_ratio.max()
print("Compression (Size Reduction)\nMean=%.3f\nstd=%.3f\nrange=%.3f-%.3f" %(compression_ratio_mean, compression_ratio_std, compression_ratio_min, compression_ratio_max))

# Average number of bits per character
avg_bits_per_char = df["avg_n_bits"]
avg_bits_per_char_mean = avg_bits_per_char.mean()
avg_bits_per_char_std = avg_bits_per_char.std()
avg_bits_per_char_min = avg_bits_per_char.min()
avg_bits_per_char_max = avg_bits_per_char.max()
print("Average Bits/Char\nMean=%.3f\nstd=%.3f\nrange=%.3f-%.3f" %(avg_bits_per_char_mean, avg_bits_per_char_std, avg_bits_per_char_min, avg_bits_per_char_max))


#endregion