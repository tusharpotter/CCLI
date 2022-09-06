# Shell implementation using C #

### Compilation Steps ###
````bash
# Optional (To use makefile)
sudo apt install make

# Compile Files
make

# To remove .out files
make clean 

# Create a file name PATH
touch PATH

# Put bin directory into the Path file
# The shell will use access() sys call
# to check if the required command exists in the directory
echo /bin > PATH

./csh
csh > Type your command
````

### Todo ###
- Redirection
- Error Handling