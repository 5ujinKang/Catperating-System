import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import csv
import numpy as np

pages = []
times = []

with open('tlb_results.csv') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        pages.append(int(row['num_pages']))
        times.append(float(row['time_per_access_ns']))

plt.plot(pages, times, marker='o')

plt.xscale('log', base=2)  # matplotlib >= 3.3 supports base param
plt.xlabel('Number of Pages')
plt.ylabel('Time per Access (ns)')
plt.title('TLB Size and Miss Cost Measurement')
plt.grid(True, which="both", ls="--")

# Set major ticks at powers of 2
ax = plt.gca()
ax.xaxis.set_major_locator(ticker.LogLocator(base=2.0, numticks=20))
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x)}"))

plt.savefig('tlb_plot.png')