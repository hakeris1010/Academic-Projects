#include <stdlib.h>
#include "xmlstructs.h"

char XML_initAttrib(struct XMLAttrib* att)
{
    if(!att) return 1;
    att->name = NULL;
    att->value = NULL;
    return 0;
}

XMLAttrib XML_getEmptyAttrib()
{
    XMLAttrib attr;
    XML_initAttrib(&attr);
    return attr;
}

char XML_initTag(struct XMLTag* tag)
{
    if(!tag) return 1;
    tag->attribCount = 0;
    tag->attribs = NULL;
    tag->tagName = NULL;
    tag->namespaceName = NULL;
    tag->tagType = XML_TAGTYPE_UNDEFINED;
    return 0;
}

XMLTag XML_getEmptyTag()
{
    XMLTag tg;
    XML_initTag(&tg);
    return tg;
}

char XML_initElement(struct XMLElement* elem)
{
    if(!elem) return 1;
    elem->data = NULL;
    elem->innerElemCount = 0;
    elem->innerElements = NULL;
    XML_initTag( &(elem->mainTag) );
    return 0;
}

XMLElement XML_getEmptyElement()
{
    XMLElement el;
    XML_initElement(&el);
    return el;
}

void XML_clearAttrib(struct XMLAttrib* att)
{
    if(!att) return;

    if(att->name)
        free(att->name);
    att->name = NULL;

    if(att->value)
        free(att->value);
    att->value = NULL;
}

void XML_clearTag(struct XMLTag* tag)
{
    if(!tag) return;

    if(tag->attribs)
    {
        for(size_t i = 0; i < tag->attribCount; i++)
        {
            XML_clearAttrib(tag->attribs + i);
        }
        free(tag->attribs);
        tag->attribs=NULL;
    }
    tag->attribCount = 0;

    if(tag->tagName)
        free(tag->tagName);
    tag->tagName = NULL;

    if(tag->namespaceName)
        free(tag->namespaceName);
    tag->namespaceName = NULL;
}

void XML_clearElement(struct XMLElement* elem)
{
    if(!elem) return;
    if(elem->innerElements)
    {
        for(size_t i = 0; i < elem->innerElemCount; i++)
        {
            XML_clearElement(elem->innerElements + i);
        }
        free( elem->innerElements );
        elem->innerElements = NULL;
    }
    elem->innerElemCount = 0;

    XML_clearTag( &(elem->mainTag) );

    if(elem->data)
    {
        free(elem->data);
        elem->data = NULL;
    }
}

//end;
