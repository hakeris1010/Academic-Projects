#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char isGood(char c, const char* excl)
{
    for(size_t i=0; i<strlen(excl); i++)
    {
        if(c == excl[i])
            return 0;
    }
    return 1;
}

int main(int argc, char** argv)
{
    const char* inp = NULL;
    const char* outp = NULL;
    if(argc>1)
    {
        inp = argv[1];
        if(argc>2)
            outp = argv[2];
    }
    else
    {
        printf("No input!\n");
        return 1;
    }

    FILE* f = fopen(inp, "r");
    if(!f)
    {
        printf("Bad input file!\n");
        return 2;
    }

    const char* excl = "\n; _,./"; //exclusions (word separators)

    fseek(f, 0, SEEK_END);
    long siz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* inData = (char*) malloc(sizeof(char) * siz);
    if(!inData)
    {
        printf("Malloc() err!\n");
        return 3;
    }

    size_t cur=0;
    char lastGood = 1;
    while(!feof(f))
    {
        char c;
        if(fscanf(f, "%c", &c) != 1)
            break;

        if(cur < siz)
        {
            if(isGood(c, excl))
            {
                inData[cur] = c;
                lastGood = 1;
                cur++;
            }
            else
            {
                if(lastGood)
                {
                    inData[cur] = '\n';
                    cur++;
                }
                lastGood = 0;
            }
        }
    }
    inData[cur] = '\0';

    if(!outp) //no output, print to screen;
    {
        printf("%s\n", inData);
    }
    else
    {
        if(f)
            fclose(f);

        f = fopen(outp, "w");
        if(!f)
        {
            printf("Can't open output file!\n");
        }
        fprintf(f, "%s", inData);
    }

    if(f)
        fclose(f);

    free(inData);

    return 0;
}
