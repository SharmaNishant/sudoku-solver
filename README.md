# Sudoku_Solver
Hill Climbing, Simulated Annealing based Sudoku solver. 

It Displays the input sudoku, random initialized sudoku with it's current fitness, and the end result. Output is colored to easily test if the output is correct or not as colors make it easier for us to look at sudoku with so many number (each number has it's own color). 

# Compiling
Just run 'make' inside root folder of the package and you should a executable named Sudoku_Solver in the same folder.

# Running the Solver

##just ./Sudoku_Solver
if you use this option this option, it will first search for a launch file (default filename is "sudoku.launch", check the file for launch file format), and if the file exists, solver will read parameters from there. Note: It will only search for the launch file in the execution folder. 
If the file doesn't exist, it will initial a blank sudoku fill it and try to solve it.

##./Sudoku_Solver file <launch_file> or ./Sudoku_Solver 1 <launch_file>
This will check the launch file (look at "sudoku.launch" for sample launch file, arguments should be in even lines) and read parameters from there. This way you don't have to recompile code but just change the launch file and rerun the same executable after changing the launch parameters. 


##./Sudoku_Solver <'args' or '2'> input_file simulation_type seed_value initial_probability drop_constant drop_after_iteration_constant
Self explanatory

## runSolver script
If you want to run multiple simulation, you can use this file as reference to run experiments as batch process and then it saves the output in txt files, which can be then read using 'cat filename'. Note: Those files are not human readable because I am using ANSI escape characters to color values on terminal. 
