#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>


//#include <time.h>
//#include <sys/types.h>
//#include <limits.h>

/**
 * This function compare between the output and the correct output
 * @param output_path - the path to the correct output
 * @return the status of the compering
 */
int compText(char *output_path) {
    int exe;
    int status;
    pid_t pid;
    char *args[] = {"./comp.out", "./output.txt", output_path, NULL};
    if ((pid = fork()) == -1) {
        if (write(2, "Error in: fork\n", strlen("Error in: fork\n")) < 0);
        return -1;
    } else if (pid == 0) {
        if ((exe = execvp("./comp.out", args)) == -1) {
            if (write(2, "Error in: execvp\n", strlen("Error in: execvp\n")) < 0);
            return -1;
        }
    } else {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

/**
 * This function compile ex21.c file.
 * @return if the compile succeed
 */
int compileEx21File() {
    int stat;
    pid_t pid;
    if ((pid = fork()) == -1) {
        if (write(2, "Error in: fork\n", strlen("Error in: fork\n")) < 0);
        return -1;
    }
    if (pid == 0) {
        char *compiled_file = "./ex21.c";
        char *argument_list[] = {"gcc", compiled_file, "-o", "comp.out", NULL};
        if (execvp("gcc", argument_list) == -1) {
            if (write(2, "Error in: execvp\n", strlen("Error in: execvp\n")) < 0);
            return -1;
        }
    }
    wait(&stat);
    if (!WIFEXITED(stat) || WEXITSTATUS(stat) < 0) {
        if (write(2, "Error in: compile ex21 file\n", strlen("Error in: compile ex21 file\n")) < 0);
        return -1;
    }
    return 0;
}

/**
 * This function run the compiled file.
 * @param input_path - the input to the program.
 * @param studentName - the student name.
 * @return if the running success.
 */
int runProgram(char *input_path, char *studentName) {
    pid_t stat;
    pid_t pid;
//    time_t start, end;
//    start = time(NULL);
    if ((pid = fork()) == -1) {
        if (write(2, "Error in: fork\n", strlen("Error in: fork\n")) < 0);
        return -1;
    } else if (pid == 0) {
        //child
        int outputStream; // where the output of the running we save
        int inputStream;
        inputStream = open(input_path, O_RDWR, S_IRWXU | S_IROTH | S_IRGRP);
        if (inputStream < 0) {
            if (write(2, "Error in: open\n", strlen("Error in: open\n")) < 0);
            return -1;
        }
        //redirect the output stream to the output path
        outputStream = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IROTH | S_IRGRP);
        if (outputStream < 0) {
            if (write(2, "Error in: open\n", strlen("Error in: open\n")) < 0);
            //close input file
            close(inputStream);
            return -1;
        }
        if (dup2(inputStream, 0) == -1) {
            if (write(2, "Error in: dup2\n",strlen("Error in: dup2\n")) < 0);
            //close input file and the new output file
            close(inputStream);
            close(outputStream);
            return -1;
        }
        if (dup2(outputStream, 1) == -1) {
            if (write(2, "Error in: dup2\n",strlen("Error in: dup2\n")) < 0);
            //close input file and the new output file
            close(inputStream);
            close(outputStream);
            return -1;
        }
        char compiled_file[] = "./";
        strcat(compiled_file, studentName);
        char *argument_list[] = {compiled_file, NULL};
        if (execvp(compiled_file, argument_list) == -1) {
            if (write(2, "Error in: execvp\n", strlen("Error in: execvp\n")) < 0);
            //close input file and the new output file
            close(inputStream);
            close(outputStream);
            return -1;
        }
        //close input file and the new output file
        close(inputStream);
        close(outputStream);
    } else {
        wait(&stat);
//    end = time(NULL);
        if (!WIFEXITED(stat) || WEXITSTATUS(stat) < 0) {
            if (write(2, "Error in: run c file\n", strlen("Error in: run c file\n")) < 0);
            return -1;
        }

        //how much time the program run
//    if (end - start > 5) {
//        return 1;
//    }

        remove(studentName);
        return 0;
    }
}

/**
 * This function compile the c file.
 * @param c_path - the path to the c file.
 * @param input_path - the input to the program.
 * @param output_path - what the output should be.
 * @param studentName - the student his name.
 * @return if the compile succeed.
 */
int compile_c_file(char *c_path, char *input_path, char *studentName) {
    int exe;
    int status;
    strcat(studentName, ".out");
    char *gcc_args[] = {"gcc", c_path, "-o", studentName, NULL};
    pid_t pid;
    if ((pid = fork()) < 0) {
        if (write(2, "Error in: fork\n", strlen("Error in: fork\n")) < 0);
        return -1;
    } else if (pid == 0) {
        //child
        exe = execvp("gcc", gcc_args);
        if (exe == -1) {
            write(2, "Error in: execvp\n", strlen("Error in: execvp\n"));
        }
    }
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

/**
 * This function write to the result file.
 * @param resultFile - the result file
 * @param studentName - the student`s name
 * @param feedback - type of feedback
 */
void writeToResultFile(int resultFile, char *studentName, char *feedback) {
    \
    char line[strlen(studentName) + 1];

    // Check each case and print the correct output
    if (!strcmp(feedback, "NO_C_FILE")) {
        strcpy(line, studentName);
        strcat(line, ",0,NO_C_FILE\n");
        if (write(resultFile, line, strlen(line)) < 0);
    } else if (!strcmp(feedback, "COMPILATION_ERROR")) {
        strcpy(line, studentName);
        strcat(line, ",10,COMPILATION_ERROR\n");
        if (write(resultFile, line, strlen(line)) < 0);
    } else if (!strcmp(feedback, "TIMEOUT")) {
        strcpy(line, studentName);
        strcat(line, ",20,TIMEOUT\n");
        if (write(resultFile, line, strlen(line)) < 0);
    } else if (!strcmp(feedback, "WRONG")) {
        strcpy(line, studentName);
        strcat(line, ",50,WRONG\n");
        if (write(resultFile, line, strlen(line)) < 0);
    } else if (!strcmp(feedback, "SIMILAR")) {
        strcpy(line, studentName);
        strcat(line, ",75,SIMILAR\n");
        if (write(resultFile, line, strlen(line)) < 0);
    } else if (!strcmp(feedback, "EXCELLENT")) {
        strcpy(line, studentName);
        strcat(line, ",100,EXCELLENT\n");
        if (write(resultFile, line, strlen(line)) < 0);
    }

}

/**
 * This function search a c file and send it to compile
 * @param new_path - the path with the directory of a specific student
 * @param input_path - the path to the input file
 * @param output_path - the path to the output file
 * @param studentName - the name of the student
 */
void search_C_File(char *new_path, char *input_path, char *output_path, char *studentName, int result_file) {
    DIR *infor;
    struct dirent *pDirent;
    //try to open directory
//    printf("new path: %s\n", new_path);
    if ((infor = opendir(new_path)) == NULL) {
        if (write(2, "Error in: opendir\n", strlen("Error in: opendir\n")) < 0);
        return;
    }

    //get the stat of the directory
    struct stat dirStat;
    if (stat(new_path, &dirStat) < 0) {
        if (write(2, "Error in: stat\n", strlen("Error in: stat\n")) < 0);
        return;
    }

    //compile the ex21 file
    int ifComp; // check if the compiled file succeed to run
    ifComp = compileEx21File();
    if (ifComp == -1) {
        // Failed in fork or exec or compile ex21 file, already mark in the error file
    }

    // indicate if we have a c file in a directory
    int flag = 0;
    // looping through the directory, search for c file
    while ((pDirent = readdir(infor)) != NULL) {
        if (pDirent->d_name[strlen(pDirent->d_name) - 2] == '.' &&
            pDirent->d_name[strlen(pDirent->d_name) - 1] == 'c') {
            flag = 1;

            // create the new path - the current path with the name of the c file
            char c_path[strlen(new_path) + 1 + strlen(pDirent->d_name) + 1];
            //copy the path
            strcpy(c_path, new_path);
            // add to the path the new directory
            strcat(c_path, "/");
            strcat(c_path, pDirent->d_name);

            //save the student name in new var
            char fileName[strlen(studentName)+1];
            strcpy(fileName,studentName);

            int ifCompiled; // check if the c file compiled
            ifCompiled = compile_c_file(c_path, input_path, studentName);
            if (ifCompiled != 0) {
                writeToResultFile(result_file, fileName, "COMPILATION_ERROR");
                continue;
            }

            int ifRunning; // check if the compiled file succeed to run
            ifRunning = runProgram(input_path, studentName);
            if (ifRunning == -1) {
                // Failed in fork or dup2 or exec or running c file
                // or open input\output file, already mark in the error file
                continue;
            }

            //what is the result of the compare
            int feedback;
            feedback = compText(output_path);
            if (feedback == -1) {
                // Failed in fork or exec or running ex21 file, already mark in the error file
                continue;
            } else {
                if (feedback == 1) {
                    writeToResultFile(result_file, fileName, "EXCELLENT");
                } else if (feedback == 2) {
                    writeToResultFile(result_file, fileName, "WRONG");
                } else if (feedback == 3) {
                    writeToResultFile(result_file, fileName, "SIMILAR");
                }
            }
        }
    }
    if (closedir(infor) == -1) {
        if (write(2, "Error in: closedir\n", strlen("Error in: closedir\n")) < 0);
    }
    if (flag == 0) { // we don`t have c file
        writeToResultFile(result_file, studentName, "NO_C_FILE");
    }
}

/**
 * This function get into the directory of student.
 * @param dir_path - the path to the student directory
 * @param input_path - the path to the input file
 * @param output_path - the path to the output file
 */
void readStudentDir(char *dir_path, char *input_path, char *output_path, int result_file, int error_file) {
    DIR *info;
    struct dirent *pDirent;
    //try to open directory
    if ((info = opendir(dir_path)) == NULL) {
        if (write(2, "Not a valid directory\n", strlen("Not a valid directory\n")) < 0);
        exit(-1);
    }

    //get the stat of the directory
    struct stat dirStat;
    if (stat(dir_path, &dirStat) < 0) {
        if (write(2, "Error in: get stat of directory\n",
                  strlen("Error in: get stat of directory\n")) < 0);
        exit(-1);
    }
    // looping through the directory, searching instance from type directory
    while ((pDirent = readdir(info)) != NULL) {
        if (S_ISDIR(dirStat.st_mode)) {
            if (!strcmp(pDirent->d_name, ".") || !strcmp(pDirent->d_name, "..")) {
                continue;
            }

            //create the new path
            char new_path[strlen(dir_path) + 1 + strlen(pDirent->d_name) + 1];

            //copy the path
            strcpy(new_path, dir_path);

            // add to the path the new directory
            strcat(new_path, "/");
            strcat(new_path, pDirent->d_name);
            char studentName[strlen(pDirent->d_name) + 1];
            strcpy(studentName, pDirent->d_name);

            //get into each directory and look for a c file and compile it
//            printf("\ndir name %s", pDirent->d_name);
            search_C_File(new_path, input_path, output_path, studentName, result_file);
        }
    }
    if (closedir(info) == -1) {
        if (write(2, "Error in: closedir\n",
                  strlen("Error in: closedir\n")) < 0);
        exit(-1);
    }
}

/**
 * The main function.
 * @param count - how much parameter the user send
 * @param argv - the input from the user
 * @return
 */
int main(int count, char **argv) {
    //check if I got enough args
    if (count != 2) {
        if (write(2, "Need to send as input a path to configuration file. \n", 54) < 0);
        exit(-1);
    }

    //try to open file
    int file;
    if ((file = open(argv[1], O_RDONLY)) == -1) // Open the file
    {
        if (write(2, "Not a valid directory.\n", strlen("Not a valid directory.\n")) < 0);
        exit(-1);
    }

    //memory allocation
    char buffer[453] = {0};
    int id;
    //try to read from file to a buffer
    id = read(file, &buffer, 453);
    if (id < 0) {
        if ((2, "Error in: read\n", strlen("Error in: read\n")) < 0);
        exit(-1);
    }

    //split the input to tokens and copy the path to its var.
    char dir_path[151];
    char input_path[151];
    char output_path[151];
    char *token;
    token = strtok(buffer, "\n");
    strcpy(dir_path, token);
    token = strtok(NULL, "\n");;
    strcpy(input_path, token);
    token = strtok(NULL, "\n");;
    strcpy(output_path, token);
    if (close(file) == -1) {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
    }

    // open a file that contain the result
    int result_file;
    result_file = open("results.csv", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
    if (result_file < 0) {
        if (write(2, "Error in: open\n", strlen("Error in: open\n")) < 0);
        exit(-1);
    }

    //check if the input file is exist
    int ip;
    ip = open(input_path, O_RDONLY);
    if (ip == -1) {
        write(2, "Input file not exist\n", strlen("Input file not exist\n"));
    }

    //check if the output file is exist
    int op;
    op = open(output_path, O_RDONLY);
    if (op == -1) {
        write(2, "Output file not exist\n", strlen("Output file not exist\n"));
    }

    // open a file that contain the error
    int error_file;
    error_file = open("error.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);
    if (error_file < 0) {
        if (write(2, "Error in: open\n", strlen("Error in: open\n")) < 0);
        exit(-1);
    }
    if (dup2(error_file, 2) == -1) {
        if (write(2, "Error in: dup2\n",
                  strlen("Error in: dup2\n")) < 0);
        exit(-1);
    }


    readStudentDir(dir_path, input_path, output_path, result_file, error_file);

    if (remove("output.txt") < 0) {
        if (write(2, "Error in: remove\n", strlen("Error in: remove\n")) < 0);
    }

    //try to close file
    if (close(file) < 0) // close the file
    {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
        exit(-1);
    }

    //try to close result file
    if (close(result_file) < 0) // close the file
    {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
        exit(-1);
    }

    //try to close input file
    if (close(ip) < 0) // close the file
    {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
        exit(-1);
    }

    //try to close output file
    if (close(op) < 0) // close the file
    {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
        exit(-1);
    }

    //try to close error file
    if (close(error_file) < 0) // close the file
    {
        if (write(2, "Error in: close\n", strlen("Error in: close\n")) < 0);
        exit(-1);
    }


}

