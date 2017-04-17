#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "parser.h"
#include "dummy_functions.h"
#include "token.h"

int if_handling(ParserStatus *status) {}
int end_if_handling(ParserStatus *status) {}

void getCharfromBuffer(SaveObject *sav, wchar_t *chr, int type)
{
#if 0
    printf("From Buffer: lvl: [%d] cursor [%d]\n", sav->level, sav->cursor[sav->level-1]);
    printf("Read from buffer [%S]\n", sav->sav_buff[sav->real_level-1]);
#endif

    switch(type)
    {
        case READ_FROM_BUFFER:
            (sav->cursor[sav->level-1])++;
            *chr = sav->sav_buff[sav->level-1][sav->cursor[sav->level-1]];
            break;
        case READ_FROM_MACRO_BUFFER:
            sav->macro.cursor++;
            *chr = sav->macro.body[sav->macro.cursor];
            break;
    }
}
