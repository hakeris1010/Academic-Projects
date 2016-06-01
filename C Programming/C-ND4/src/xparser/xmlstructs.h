#ifndef XMLSTRUCTS_H_INCLUDED
#define XMLSTRUCTS_H_INCLUDED

#include "xmlstandard.h"

typedef struct XMLAttrib
{
    char* name;
    char* value;
} XMLAttrib;

typedef struct XMLTag
{
    char* tagName;
    char tagType; //can be char*
    struct XMLAttrib* attribs;
    size_t attribCount;
} XMLTag;

typedef struct XMLElement
{
    struct XMLTag mainTag;
    char* data;
    struct XMLElement* innerElements;
    size_t innerElemCount;
} XMLElement;

char XML_initAttrib(struct XMLAttrib* att);
char XML_initTag(struct XMLTag* tag);
char XML_initElement(struct XMLElement* elem);

XMLAttrib XML_getEmptyAttrib();
XMLTag XML_getEmptyTag();
XMLElement XML_getEmptyElement();

void XML_clearAttrib(struct XMLAttrib* att);
void XML_clearTag(struct XMLTag* tag);
void XML_clearElement(struct XMLElement* elem);

#endif // XMLSTRUCTS_H_INCLUDED
