#include <stdlib.h>
#include "xmlstructs.h"

void clearXMLAttrib(struct XMLAttrib* att)
{
    if(!att) return;

    free(att->name);
    att->name = NULL;

    free(att->value);
    att->value = NULL;
}

void clearXMLTag(struct XMLTag* tag)
{
    if(!tag) return;
    for(int i=tag->attribCount-1; i>=0; i--)
    {
        clearXMLAttrib(tag->attribs + i);
    }
    tag->attribCount = 0;

    free(tag->attribs);
    tag->attribs=NULL;

    free(tag->tagName);
    tag->tagName=NULL;

    free(tag->tagType);
    tag->tagType=NULL;
}

void clearXMLElement(struct XMLElement* elem)
{
    if(!elem) return;
    for(int i=elem->innerElemCount-1; i>=0; i--)
    {
        clearXMLElement(elem->innerElements + i);
    }
    elem->innerElemCount = 0;

    free(elem->innerElements);
    elem->innerElements = NULL;

    clearXMLTag(&(elem->mainTag));

    free(elem->text);
    elem->text = NULL;
}

//end;
