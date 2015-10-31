# README #

# DESCRIPTION #
SCENCO is a tool for supporting encoding and synthesis of Conditional Partial 
Order Graphs models.
Most updated version: Scenco v1.3

# CHANGELOG #
Changelog Scenco v1.3 - 31/10/2015
-Fixed bug: temporary name file created to feed ABC tool
-heuristic search: algorithm for associating opcodes to partial orders before
 simulated annealing skipped. Now first assignment is random
-Removed functions for the synthesis of one single encoding at a time
-Removed simulated annealing algorithm without heuristic function
-All warnings removed. Code refactored. Code commented removed

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
