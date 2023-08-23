# Modifed cp command
- recreate the cp command, except pass in an argument -n where it will create that number of copies
- Make another argument -s where it will show the progress of the copy

# How this will be done
- make a main.c file that implements these features 
    - This has been done, but it is not efficient, and should use a thread for the -s process do it does not conflict with the cp process
- create another repo where I fork off of: https://github.com/coreutils/coreutils
- directly edit the cp command with the added features
- make it a submodule to this project
- contact coreutils@gnu.org to see if they would be interested in adding these features
