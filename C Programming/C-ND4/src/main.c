#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "htools/logger.h"
#include "htools/hfun.h"
#include "htools/hstring.h"
#include "arraystack/arraystack.h"
#include "xparser/xmlparser.h"

char* charToString(const char ch)
{
    char* c = (char*) malloc(sizeof(char)*2);
    c[0] = ch;
    c[1] = '\0';
    return c;
}

void playground1()
{
    printf("Testing on Char:\n");
    const char* bam = hstr_makeNewString(0, "[A-B]|g");
    char g = 'C';
    printf("Is \'%c\' in: \"%s\" ? Ans: %d\n", g, bam, hfun_isCharInSpecifieds(g, bam, '|', '[', ']', '-', '#', 'x') );

    printf("\nTesting on Const String:\n");
    const char* bam1 = hstr_makeNewString(0, "a|[#x61bc-#099ba]|google|uauaua");
    const char* srch = "uauaua";
    printf("Is \'%s\' in: \"%s\" ? Ans: %d\n", srch, bam1, hfun_isStringInSpecifieds(srch, bam1, '|', '[', ']', '-', '#', 'x') );

    /*ArrayStack nuStak = ArrayStack_create(1, ARRAYSTACK_DEFAULT_PADDING, 1);

    ArrayStack_pushArray(&nuStak, "Kawaii|Neko|Desu|Chan~", 0);
    printf("String from stack: \"%s\"\n", nuStak.arr);

    ArrayStack_clear(&nuStak, NULL);
    //printf("String from stack: \"%s\"\n", nuStak.arr);*/

    //ArrayStack_Show(nuStak, 1, 0, 0, charToString ,stdout);
}

int main()
{
    hlog_setLoggerMode(HLOG_TO_SCREEN);
    //hlog_setFileCloseMode(1);
    //hlog_setLogFile("log2.txt");

    hlogf("C Test no. %d\n", 10);
    hlogf("Kawaii miku-chan~~~\n\n");

    playground1();

    //XParser newParser;
    //xps_init(&newParser, 1, 1);

    //hlog_closeLogFile();

    return 0;
}

//moe moe kyun kawaii desuuuuu~~~~~~~~~~~~~
