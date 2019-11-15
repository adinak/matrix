#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROTATE_LEFT 1
#define ROTATE_RIGHT 0
#define OP_ROTATE_LEFT 'L'
#define OP_ROTATE_RIGHT 'R'
#define OP_AVERAGE 'V'
#define OP_NEGATIVE 'N'

#define ERROR 0
#define PIXEL 255
#define AVERAGE_SQUARE 1
#define INNER_DIVIDER 9
#define CORNER_DIVIDER 4
#define FRAME_DIVIDER 6
#define MULTIPLY 2

/****************************************************
* Print Functions:
****************************************************/

void PrintRowMessage()		{	printf("Enter the number of rows in your image matrix:\n");	}
void PrintColumnMessage()	{	printf("Enter the number of columns in your image matrix:\n");	}
void PrintEnterMatrix()		{	printf("Enter your matrix:\n");	}
void PrintImageMessage()	{	printf("Your original image is:\n");	}
void PrintPrompt()			{	printf("What you would like to do with your photo (L,R,V,N)?\n"); }
void PrintLeftRotation()	{	printf("The image after left rotation:\n");	}
void PrintRightRotation()	{	printf("The image after right rotation:\n");	}
void PrintAveraging()		{	printf("The image after averaging:\n");	}
void PrintNegating()		{	printf("The image after negating:\n");	}

/****************************************************
* decalred Functions:
****************************************************/

int read_size(void);
void read_matrix(int *image, int row, int column);
char read_action(void);
void send_to_function_according_to_action(char action, int *image, int row, int column, int *target);
void average(int* image, int n, int m, int* target);
void calculate_right_corners(int *image, int *target, int n, int m);
void calculate_left_corners(int *image, int *target, int n, int m);
void calculate_frame(int *image, int *target, int n, int m);
void calculate_inner_image(int *image, int *target, int n, int m);
void rotate(int* image, int n, int m, int* target, int d);
void negative (int* image, int n, int m, int* target);
void print_image(int* image, int n, int m);

int main(){

    PrintRowMessage();
    int row_size = read_size();
    PrintColumnMessage();
    int column_size = read_size();

    PrintEnterMatrix();
    int *image = (int*)malloc(sizeof(int)*(row_size*column_size));
    if(image == NULL){
        return ERROR;
    }
    read_matrix(image, row_size, column_size);
    PrintImageMessage();
    print_image(image, row_size, column_size);
    PrintPrompt();

    char action = read_action();
    int *target = (int*)malloc(sizeof(int)*(row_size*column_size));
    if(target == NULL){
        return ERROR;
    }
    send_to_function_according_to_action(action, image, row_size, column_size, target);
    if(action == OP_ROTATE_LEFT || action == OP_ROTATE_RIGHT){
        print_image(target, column_size, row_size);
    } else {
        print_image(target, row_size, column_size);
    }

    free(image);
    free(target);

    return 0;
}

int read_size(){
    int size = 0;
    scanf(" %d", &size);

    return size;
}

void read_matrix(int *image, int row, int column){

    for (int i = 0; i < row*column; ++i) {
            scanf(" %d", (image + i));
    }

}

char read_action(){
    char action = 0;
    scanf(" %c", &action);

    return action;
}

void send_to_function_according_to_action(char action, int *image, int row, int column, int *target){

    if(action == OP_ROTATE_LEFT){
        rotate(image, row, column, target, ROTATE_LEFT);
    } else if(action == OP_ROTATE_RIGHT){
        rotate(image, row, column, target, ROTATE_RIGHT);
    }
    else if(action == OP_AVERAGE){
        average(image, row, column, target);
    }
    else if (action == OP_NEGATIVE){
        negative(image, row, column, target);
    }
}

//changes each pixel to average value of the pixels surrounding it
void average(int* image, int n, int m, int* target){

    calculate_right_corners(image, target,n, m);

    calculate_left_corners(image, target, n, m);

    calculate_inner_image(image, target, n, m);

    calculate_frame(image, target, n, m);

    PrintAveraging();

}

void calculate_left_corners(int *image, int *target, int n, int m){
    double upper_corner = 0, bottom_corner = 0;

    int k = 0;
    //upper right corner
    while(k <= AVERAGE_SQUARE){
        upper_corner += (*(image + k) + *(image + m + k));
        k++;
    }
    *target = upper_corner / CORNER_DIVIDER;
    //bottom right corner
    k = 0;
    while(k <= AVERAGE_SQUARE){
        bottom_corner += (*(image + m*n - m + k) + *(image + m*n - m*MULTIPLY + k));
        k++;
    }
    *(target + m*n - m) = bottom_corner / CORNER_DIVIDER;
}

void calculate_right_corners(int *image, int *target, int n, int m){

    double upper_corner = 0, bottom_corner = 0;
    int k = 0;

    //upper left corner
    while(k <= AVERAGE_SQUARE){
        upper_corner += (*(image + m - k -1) + *((image + m*MULTIPLY - 1) - k));
        k++;
    }
    *(target + m - 1) = upper_corner / CORNER_DIVIDER;

    //bottom left corner
    k = 0;
    while(k <= AVERAGE_SQUARE){
        bottom_corner += (*(image + m*n - 1 - k) + *(image + m*n - m - 1 - k));
        k++;
    }
    *(target + m*n - 1) = bottom_corner / CORNER_DIVIDER;

}

void calculate_frame(int *image, int *target, int n, int m){

    double upper = 0, bottom = 0, left = 0, right = 0;
    int k = -1;
    int i = 0, j = 0;

    //upper frame
    for (j = 1; j < m-1; ++j) {
        while (k <= AVERAGE_SQUARE) {
            upper += (*(image + j + k) + *(image + m + j + k));
            k++;
        }
        *(target + j) = upper / FRAME_DIVIDER;
        upper = 0;
        k = -1;
    }

    //left frame
    k = -1;
    for (i = 1; i < n-1; ++i) {
        while(k <= AVERAGE_SQUARE){
            left += (*((image + i*m) + k*m + 1) + *(image + i*m + k*m));
            k++;
        }
        *(target + i*m) = left / FRAME_DIVIDER;
        left = 0;
        k = -1;
    }

    //right frame
    k = -1;
    j = m-1;
    for (i = 1; i < n-1; ++i) {
        while (k <= AVERAGE_SQUARE) {
            right += (*((image + i*m) + j + k*m -1) + *((image + i*m) + j + k*m));
            k++;
        }
        *(target + i * m + j) = right / FRAME_DIVIDER;
        right = 0;
        k = -1;
    }

    //bottom frame
    k = -1;
    i = n-1;
    for (j = 1; j < m-1; ++j) {
        while(k <= AVERAGE_SQUARE){
            bottom += (*((image + i*m) + j + k) + *((image + i*m) - m + j + k));
            k++;
        }
        *(target + i*m + j) = bottom / FRAME_DIVIDER;
        bottom = 0;
        k = -1;
    }

}

void calculate_inner_image(int *image, int *target, int n, int m){

    double average = 0;

    for (int i = 1; i < n-1; ++i) {
        for (int j = 1; j < m-1; ++j) {

            if((i > 0 && i < n-1) || (j > 0 && j < m-1)) {
                int k = -1;
                while (k <= AVERAGE_SQUARE) {
                    average += (*((image + i*m) - m + j + k) + *((image + i*m) + j + k) + *((image + i*m) + m + j + k));
                    k++;
                }
                *((target + i*m) + j) = average / INNER_DIVIDER;
                average = 0;
            }
        }
    }
}

//rotates the image according to the direction of the users input
void rotate(int* image, int n, int m, int* target, int d){

    //rotate R
    if(d == ROTATE_RIGHT){
        for(int i = 0; i < m ; ++i) {
            for(int j = 0; j < n; ++j) {
                *((target + i*n) + n - 1 - j) = *((image + j*m) + i);
            }
        }
        PrintRightRotation();
    }
    else {
        //rotate L
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                *((target + i*n) + n - 1 - j) = *((image + m*n - 1 - i) - j*m);
            }
        }
        PrintLeftRotation();
    }
}

//changes each pixel to the delta between it and the num 255
void negative (int* image, int n, int m, int* target){

    for (int i = 0; i < m*n; ++i) {
        *(target + i) = PIXEL - *(image + i);
    }
    PrintNegating();
}

//prints the image matrix
void print_image(int* image, int n, int m)
{
    int i,j;
    for(j=0;j < n; j++)
    {
        for (i =0; i < m; i++)
        {
            if (i != m-1)
                printf("%d\t", *( (image+j*m) + i));//image[j][i]
            else
                printf("%d", *( (image+j*m) + i));

        }
        printf("\n");
    }
}