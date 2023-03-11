<h1 align=center>
  <strong> PIPEX </strong>
</h1>

<p align="center">
  <sub> C program to redirect process output to input via pipes and execute specified commands in a file.
  <sub>
</p>


[![-----------------------------------------------------](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png)](#table-of-contents)

<p align="center">
  <a href="#About"> 💡 About the project </a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#HowWork"> 📝 How Pipex works</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#HowUse"> ⚙️ How to use</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;
  <a href="#Credits"> 🏆 Credits</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</p>

<br/>

<a id="About"></a>
## 💡 About the project
> This project involves creating a C program that redirects the output of one process to the input of another process using pipes. The goal of the project is to create a program that is capable of executing two commands (e.g., cat and grep) and redirecting the output of the first command as input to the second command using pipes. The program should be able to read a file and execute the commands specified in it.

<br/>

<a id="HowWork"></a>
## 📝 How Pipex works

1. Reads the command line arguments and check if the correct number of arguments was provided.
2. Create a pipe using the `pipe()` system call.
3. Create a child process using the `fork()` system call. The child process executes the `child_process()`, while the parent process executes the `parent_process()`.
4. The `child_process()` opens the input file and redirects its standard input to this file. Then, the child process redirects its standard output to the pipe created by the parent process and execute the first command using the `execute()`.
5. The `parent_process()` opens or creates the output file and redirects its standard output to this file. Then, the parent process redirects its standard input to the pipe created by the parent process and executes the second command using the `execute()`.
6. The parent process closes the file descriptors of the pipes that are not needed.

<br/>

<a id="HowUse"></a>
## ⚙️ How to use

1. Clone this repository
```sh
git clone https://github.com/jpedr0c/42_pipex.git
```
2. In the project directory, compile the program using the following command
```sh
make
```
3. To run the program, you will need to specify the commands you want to execute and the file that should be used as input for the first command and output for the second command. For example:
```sh
./pipex Makefile "cat" "wc -l" out
```
4. To see the result just use the following command
```sh
cat out
```
5. If you want to remove the object files generated during compilation
```sh
make clean
```
6. If you want to remove the object files and executable generated during compilation
```sh
make fclean
```

<br/>

<a id="Credits"></a>
## 🏆 Credits
***Thank you for taking the time to review my project. If you have any questions or would like to connect with me, please feel free to reach out to me on [LinkedIn](https://www.linkedin.com/in/jpedroc)***
