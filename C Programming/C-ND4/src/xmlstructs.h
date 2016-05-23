#ifndef XMLSTRUCTS_H_INCLUDED
#define XMLSTRUCTS_H_INCLUDED

//XML-Type

#define XML_TAGTYPE_UNDEFINED  0
#define XML_TAGTYPE_INITIALIZE 1  // the <?...?> tag, or <!...(!)> tag
#define XML_TAGTYPE_BEGIN      2   // <...> tag
#define XML_TAGTYPE_END        3   // </...> tag
#define XML_TAGTYPE_SELFCLOSE  4 // <... /> tag, or specific, predefined (like <br>)

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

void XML_clearAttrib(struct XMLAttrib* att);
void XML_clearTag(struct XMLTag* tag);
void XML_clearElement(struct XMLElement* elem);

#endif // XMLSTRUCTS_H_INCLUDED
