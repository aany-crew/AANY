import pandas as pd
import matplotlib.pyplot as plt
import os

pdir = "/Users/noahmarkowitz/Documents/GitHub/AANY"

# Read csv output file
fname = pdir + os.sep + "analysis/output.csv"
df = pd.read_csv(fname)

plt.clf()
plt.cla()

# Compression and decompression time vs n_characters
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
#plt.show()
plt.savefig('analysis/fig1.png')

plt.cla()
plt.clf()

# Compression and decompression time graphs vs. # unique characters
df2 = df[df["file_name"].str.contains(".csv")==False]
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
plt.savefig('analysis/fig2.png')
#plt.show()

plt.cla()
plt.clf()

# Scanning text vs # of characters
df = df.sort_values("n_chracters")
plt.plot(df["n_chracters"], df["get_freq_time"], 'c*', markersize=12)
plt.plot(df["n_chracters"], df["get_freq_time"], 'c')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Get character frequency vs. Number of characters")
plt.xlabel("Number of characters")
plt.ylabel("Get Character Frequency (seconds)")
#plt.legend()
#plt.show()
plt.savefig('analysis/fig3.png')

plt.cla()
plt.clf()

# Decompression vs Compression
df = df.sort_values("compression_time")
plt.plot(df["compression_time"], df["decompression_time"], 'm*', markersize=12)
plt.plot(df["compression_time"], df["decompression_time"], 'm')
#plt.xscale("log")
#plt.yscale("log")
plt.title("Decompression vs Compression time")
plt.xlabel("Compression Time")
plt.ylabel("Decompression Time")
#plt.legend()
plt.show()
plt.savefig('analysis/fig4.png')

plt.cla()
plt.clf()


# Generate Codes and Build Trees (ODD/BAD)
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
plt.legend()
#plt.show()
plt.savefig('analysis/fig5.png')