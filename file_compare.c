#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * This function check if the files are similar.
 * @param file1 - the first file.
 * @param file2 - the second file.
 * @return if the files are similar.
 */
int isSimilar(int file1, int file2) {
    char ch1, ch2;
    while (1) {
        int x1, x2;
        while (1) {
            x1 = read(file1, &ch1, 1);
            if (x1 < 0) {
                perror("Failed: read first file\n");
                exit(-1);
            }
            //check if we got to the end of the file
            if (x1 == 0) {
                break;
            }
            // check if the char is not a special char
            if (ch1 != '\a' && ch1 != '\b' && ch1 != '\f' && ch1 != '\n' && ch1 != '\r' && ch1 != '\t'&&
                ch1 != '\v' && ch1 != ' ') {
                break;
            }
        }
        while (1) {
            x2 = read(file2, &ch2, 1);
            if (x2 < 0) {
                perror("Failed: read second file\n");
                exit(-1);
            }
            //check if we got to the end of the file
            if (x2 == 0) {
                break;
            }
            // check if the char is not a special char
            if (ch2 != '\a' && ch2 != '\b' && ch2 != '\f' && ch2 != '\n' && ch2 != '\r' && ch2 != '\t' &&
                ch2 != '\v' && ch2 != ' ') {
                break;
            }
        }
//        printf("value of x1 - %d , value of x2 - %d \n", x1, x2);
//        printf("ch1 -  %s , ch2 - %s \n", &ch1, &ch2);
//        printf("--------\n");
        //If we got to the end of the file together then the file are similar
        if (x1 == 0 && x2 == 0) {
            return 3;
        }
        //convert the chars to lower case
        if (ch1 >= 'A' && ch1 <= 'Z') {
            ch1 = 'a' + (ch1 - 'A');
        }
        if (ch2 >= 'A' && ch2 <= 'Z') {
            ch2 = 'a' + (ch2 - 'A');
        }
        //compare between the chars
        if (ch1 != ch2) {
            return 2;
        }
        if (x1 == 0 || x2 == 0) {
            return 2;
        }
    }
}

/**
 * This function check if the files are the same.
 * @param file1 - the first file.
 * @param file2 - the second file.
 * @return if the files are identical.
 */

int isIdentical(int file1, int file2) {
    char ch1, ch2;
    while (1) {
        int x1, x2;
        //try to read from first file
        x1 = read(file1, &ch1, 1);
        if (x1 < 0) {
            perror("Failed: read first file\n");
            exit(-1);
        }
        //try to read from second file
        x2 = read(file2, &ch2, 1);
        if (x2  < 0) {
            perror("Failed: read second file\n");
            exit(-1);
        }
//        printf("value of x1 - %d , value of x2 - %d \n", x1, x2);
//        printf("ch1 -  %s , ch2 - %s \n", &ch1, &ch2);
//        printf("--------\n");
        if (x1 == 0 && x2 == 0) {
            return 1;
        }
        if (ch1 != ch2) {
            return 0;
        }
        if (x1 == 0 || x2 == 0) {
            return 0;
        }
    }
}

/**
 * This function is the main of the program
 * @param count - how much args the user send.
 * @param argv - a paths where the files are contained.
 * @return if the file are identical, similar or different.
 */
int main(int count, char **argv) {
    //check if I got enough args
    if (count != 3) {
        write(1, "2 path needs to send as input. \n", 21);
        return -1;
    }
    int file1, file2;
    if ((file1 = open(argv[1], O_RDONLY)) == -1) // Open the first file
    {
        perror("Failed: open first file\n");
        exit(-1);
    }
    if ((file2 = open(argv[2], O_RDONLY)) == -1) // Open the second file
    {
        perror("Failed: open second file\n");
        exit(-1);
    }
    struct stat fileStat1;
    struct stat fileStat2;
    if (stat(argv[1], &fileStat1) < 0) {
        perror("Failed: get stat of first file\n");
        exit(-1);
    }
    if (stat(argv[2], &fileStat2) < 0) {
        perror("Failed: get stat of second file\n");
        exit(-1);
    }
    //Check if the file have the same size
    if (fileStat1.st_size == fileStat2.st_size) {
        //Check if the files have the same text
        if (isIdentical(file1, file2)) {
            return 1;
        } else {
            // need to open the file from the begging because we need to initial the pointer
            int file1, file2;
            if ((file1 = open(argv[1], O_RDONLY)) == -1) // Open the first file
            {
                perror("Failed: open first file\n");
                exit(-1);
            }
            if ((file2 = open(argv[2], O_RDONLY)) == -1) // Open the second file
            {
                perror("Failed: open second file\n");
                exit(-1);
            }
            return isSimilar(file1, file2);
        }
    } else {
        return isSimilar(file1, file2);
    }
}
