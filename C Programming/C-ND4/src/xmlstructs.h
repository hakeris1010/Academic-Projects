#ifndef XMLSTRUCTS_H_INCLUDED
#define XMLSTRUCTS_H_INCLUDED

//XML-Type

#define XML_TAGTYPE_INITIALIZE "init"  // the <?...?> tag
#define XML_TAGTYPE_BEGIN      "beg"   // <...> tag
#define XML_TAGTYPE_END        "end"   // </...> tag
#define XML_TAGTYPE_SELFCLOSE  "close" // <... /> tag, or specific, predefined (like <br>)

struct XMLAttrib
{
    char* name;
    char* value;
};

struct XMLTag
{
    char* tagName;
    char* tagType; //can be int
    struct XMLAttrib* attribs;
    int attribCount;
};

struct XMLElement
{
    struct XMLTag mainTag;
    //char* tagString;         //in tagObject
    //std::vector<XMLAttrib> attribs;
    char* text;
    struct XMLElement* innerElements;
    int innerElemCount;
};

void clearXMLAttrib(struct XMLAttrib* att);
void clearXMLTag(struct XMLTag* tag);
void clearXMLElement(struct XMLElement* elem);

#endif // XMLSTRUCTS_H_INCLUDED
