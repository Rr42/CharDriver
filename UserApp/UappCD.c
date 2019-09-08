/* To compile: 
gcc -o UappCD UappCD.c

To run:
./UappCD
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DEVICE "/dev/CharDriver"

int main()
{
    int i;
    char ch, io_buf[100];

    int fp = open(DEVICE, O_RDWR); // open for reading and writing

    if(fp == -1)
    {
        printf("file %s either does not exist or has been locked by another user\n", DEVICE);
        exit(-1);
        /* Try using
         sudo chmod 777 /dev/CharDriver
         # chmod <super user><group><others> <device path>
         */
    }

    printf("1. \'r\' for reading data from device\n2. \'w\' for writing data to device\nEnter option: ");
    scanf("%c", &ch);

    switch (ch)
    {
    case 'r':
        read(fp, io_buf, sizeof(io_buf));
        printf("Data: %s\n", io_buf);
        break;
    case 'w':
        printf("Enter data: ");
        scanf(" %[^\n]", io_buf);
        printf("io_buf: %s\n", io_buf);
        write(fp, io_buf, sizeof(io_buf));
        break;
    default:
        printf("Invalid input\n");
        break;
    }

    close(fp);

    return 0;
}

/*    
     FILE *fp1;

     char c;

     fp1= fopen ("C:\\myfiles\\newfile.txt", "r");


     while(1)
     {
        c = fgetc(fp1);
        if(c==EOF)
            break;
        else
            printf("%c", c);
     }
     fclose(fp1);
     return 0;
} */