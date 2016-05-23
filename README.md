# README #

# DESCRIPTION #
SCENCO is a tool for supporting encoding and synthesis of Conditional Partial 
Order Graphs models.
Most updated version: ScEnco v1.4.3

# CHANGELOG #
Changelog Scenco v1.4.4 - 23/05/2016
-OS X support

Changelog Scenco v1.4.3 - 19/05/2016
-Alias dependency removed from ABC
-Max and min weights computed via limits library

Changelog Scenco v1.4.2 - 04/11/2015
-Output formatted (Total run-time program computed and displayed in verbose)
-Structure for starting graphs allocation allocated and freed at run time.

Changelog Scenco v1.4.1 - 03/11/2015
-Minor changes at the verbose output

Changelog Scenco v1.4 - 03/11/2015
-Memory management of Boolean equations changed into a dynamic memory allocation
-Minor changes inside main. Useful print out added.

Changelog Scenco v1.3.4 - 02/11/2015
-Fixed bug when CPOG_SIZE synthesis mode was selected

Changelog Scenco v1.3.3 - 02/11/2015
-All temp files removed
-Fixed bug name with a couple of temporary file with same names
-Reading of ABC output file modified

Changelog Scenco v1.3.2 - 01/11/2015
-Modified way constraints and output of espresso are read

Changelog Scenco v1.3.1 - 31/10/2015
-All warning removed
-Add parameter for printing out version of ScEnco

Changelog Scenco v1.3 - 31/10/2015
-Fixed bug: temporary name file created to feed ABC tool
-heuristic search: algorithm for associating opcodes to partial orders before
 simulated annealing skipped. Now first assignment is random
-Removed functions for the synthesis of one single encoding at a time
-Removed simulated annealing algorithm without heuristic function
-Some warnings removed. Code refactored. Code commented removed

Changelog Scenco v1.2.1 - 28/10/2015
-Fixed bug: Single Literal Encoding run always at the beginning of the program

Changelog Scenco v1.2
-Fixed bug with temporary files removal
-Scenco version printed out with the help of the tool

Changelog Scenco v1.1.1
-Fixed bug with random encoding constrained
-Fixed bug which did not allow the SCRIPT_PATH file to be removed in Windows

Changelog Scenco v1.1
-Constrained encoding added for Exhaustive and Random Search
-Fixed bug: early encoding creation in Heuristic encoding

Changelog Scenco v1.0
-Supports Windows and Linux.
