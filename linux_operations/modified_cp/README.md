# Modifed copy command
The purpose of this project is to be able to copy a file multiple times<br>
You call the modified_cp command by passing a file, then passing a -n flag with a number<br>
I also added a -v flag for verbose progress, so you are informed when files are copied

# Running
Usage: mod-cp [OPTIONS]<br>

Copy a file one or more times.<br>

Required:<br>
  -s, --src FILE          Source file to copy<br>

Options:<br>
  -n, --number NUM        Number of copies to create (default: 1)<br>
  -d, --dest DIR          Destination directory (default: .)<br>
  -v, --verbose           Show copy progress<br>
  -h, --help              Show this help message<br>

Examples:<br>
  mod-cp -s file.txt<br>
  mod-cp -s file.txt -n 5<br>
  mod-cp -s file.txt -n 5 -d ./copies<br>
  mod-cp --src file.txt --number 10 --dest ./backup --verbose<br>
