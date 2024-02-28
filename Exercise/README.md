# Explanation of Key Sections:
## File Operations: 
The program starts by attempting to open the input and output files. It uses `<fstream>` for file I/O operations.
## Data Structures:
An `unordered_map` is used to efficiently map each symbol to its trade summary. The `TradeSummary` struct holds all necessary calculations per symbol.
## Line Parsing: 
Each line from the input file is read into a `string`, then split into tokens using a `stringstream`. This allows processing of individual trade entries.
## Calculations: 
For each trade, the program updates the trade summary for its symbol. This includes calculating the maximum time gap, total volume, weighted average price, and maximum trade price.
## Sorting and Output: 
The summaries are transferred to a vector to be sorted alphabetically by symbol. The sorted data is then written to the output file, with calculations such as the weighted average price being finalized based on total volume and price sums.