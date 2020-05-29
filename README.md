# particle_dynamic
This is a dynamic simulation of particles under certain interaction, for example gravity or vdW force. It is created completely out of hobby. This program is designed to be called by other program like python script, but is can also work alone. This program utilize openMP parallel acceleration.

## particle state input and output
The initial states of the particle assemble is read via a txt file, which includes 6 columns.
First two columns are initial X and Y coordinates, in range of -100 to 100.
Next two columns are initial Vx and Vy, then mass, finally a flag variable.
The column number can change in the future, adding more properties to the assemble, say charge, radius, etc.
The program will run a given number of rounds of given time interval, using dynamic simulation method ("leap-frog").
In the process, snapshots of system state can be saved for animation or diagnostic.
Finally the system state can be output to a txt file.
