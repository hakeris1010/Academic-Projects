// 11 darb.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Data
{
    char str[30];
    int d;
    double l;
} data[100];

void printfStuff(struct Data* d, size_t dataCount, FILE* f, char showFormat, size_t strWidth, size_t intWidth, size_t dblWidth, size_t dblPrecision)
{
    if(!f)
        f = stdout;
    if(showFormat)
    {
        for(size_t i=0; i<strWidth+intWidth+dblWidth; i++)
        {
            if(i==strWidth || i==strWidth+intWidth || i==strWidth+intWidth+dblWidth)
                fprintf(f,"|");
            fprintf(f,".");
        }
        fprintf(f,"\n");
    }
    for(size_t i = 0; i<dataCount; ++i)
    {
        char intStr[intWidth+1];
        snprintf(intStr, intWidth+1, "%d", d[i].d);

        char dblStr1[dblWidth > 20 ? dblWidth : 20];
        snprintf(dblStr1, sizeof(dblStr1), "%d", (long)(trunc(d[i].l)) );

        if(strlen(dblStr1) > (dblWidth - dblPrecision - 1))
        {
            memmove(dblStr1, dblStr1 + strlen(dblStr1) - (dblWidth - dblPrecision - 1), (dblWidth - dblPrecision - 1));
            dblStr1[dblWidth - dblPrecision - 1] = '\0';
        }

        long frac = ((long)round(d[i].l*pow(10, dblPrecision))) % ((long)round(pow(10, dblPrecision)));
        if(frac<0) frac*=-1;

        char spaces[dblPrecision+2];
        spaces[0] = '\0';
        while(!(frac%10) && strlen(spaces) < sizeof(spaces))
        {
            frac /= 10;
            strcat(spaces, " ");
        }

        char dblStr2[dblPrecision+2];
        snprintf(dblStr2, dblPrecision+2, ".%d%s", frac, spaces );

        char dblStrFinal[24];
        strcpy(dblStrFinal, dblStr1);
        strcat(dblStrFinal, dblStr2);

        //fprintf(f, "%s\n", dblStrFinal);

        fprintf(f,"%-*.*s %*.*s %*.*s\n", strWidth, strWidth, d[i].str, intWidth, intWidth, intStr, dblWidth, dblWidth, dblStrFinal);
    }
}

int main()
{
    strcpy(data[0].str, "a");
    data[0].d = 10;
    data[0].l = -13.14;

    strcpy(data[1].str, "abcdef");
    data[1].d = -47;
    data[1].l = 61.1;

    strcpy(data[2].str, "ab");
    data[2].d = -4770;
    data[2].l = 0.0;

    printfStuff(data, 3, stdout, 0, 4, 3, 6, 2);

    return 0;
}
