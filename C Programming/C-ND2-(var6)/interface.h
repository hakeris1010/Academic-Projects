#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

//UI Commands

#define CMD_NONE            0
#define CMD_HELP            1
#define CMD_ENTER_ARR1      2
#define CMD_ENTER_ARR2      3
#define CMD_STATUS          4
#define CMD_ARR1_STATUS     5
#define CMD_ARR2_STATUS     6
#define CMD_CALCULATE       7
#define CMD_CHANGE_ELEMS_1  8
#define CMD_CHANGE_ELEMS_2  9
#define CMD_ERASE_ELEMS_1   10
#define CMD_ERASE_ELEMS_2   11
#define CMD_INSERT_ELEMS_1  12
#define CMD_INSERT_ELEMS_2  13
#define CMD_EXIT            14

//Functions

int getCommand(int mode);
void printArrayElements(int arr[], int len);
int getNumWithValidation(const char *stringToWrite, int *num, int min, int max);
int clearBuffer();


#endif // INTERFACE_H_INCLUDED
