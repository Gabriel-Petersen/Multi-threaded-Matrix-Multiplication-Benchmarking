import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('benchmarks/results.csv')

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

for method in df['Method'].unique():
    subset = df[df['Method'] == method]
    ax1.plot(subset['Size'], subset['Time_ms'], marker='o', linewidth=2, label=method)

ax1.set_title('Visão Geral: Todos os Métodos (Log Scale)')
ax1.set_xlabel('Matrix Size (N x N)')
ax1.set_ylabel('Time (ms) - Log Scale')
ax1.set_yscale('log')
ax1.grid(True, which="both", ls="-", alpha=0.3)
ax1.legend()

parallel_methods = [m for m in df['Method'].unique() if 'Parallel' in m]
df_fast = df[df['Method'].isin(parallel_methods)]

for method in parallel_methods:
    subset = df_fast[df_fast['Method'] == method]
    ax2.plot(subset['Size'], subset['Time_ms'], marker='s', markersize=8, linewidth=3, label=method)

ax2.set_title('ZOOM: Comparação entre Métodos Paralelos')
ax2.set_xlabel('Matrix Size (N x N)')
ax2.set_ylabel('Time (ms)')
ax2.grid(True, ls="--", alpha=0.5)
ax2.legend()

plt.tight_layout()
plt.savefig('benchmarks/performance_analysis.png', dpi=300)
plt.show()