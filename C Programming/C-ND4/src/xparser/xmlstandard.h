#ifndef XMLSTANDARD_H_INCLUDED
#define XMLSTANDARD_H_INCLUDED

//XML-Type
//Tag Types
#define XML_TAGTYPE_UNDEFINED   0
#define XML_TAGTYPE_INITIALIZE  1  // the <?...?> tag, or <!...(!)> tag
#define XML_TAGTYPE_BEGIN       2  // <...> tag
#define XML_TAGTYPE_END         3  // </...> tag
#define XML_TAGTYPE_SELFCLOSE   4  // <... /> tag, or specific, predefined (like <br>)

//---------------------      XML Standard stuff      ---------------------//
//Predefined self-close tags (in xmlstructs.c).
extern const char* HTML_SelfCloseTags;
//Valid chars
extern const char* XML_ValidNameChars;
//Startin' chars
extern const char* XML_NameStartChars;
//Future (near or far).

//--------------------- XParser extended definitions ---------------------//
//CharTypes
#define XPS_CHARTYPE_DEFAULT     1
#define XPS_CHARTYPE_ALPHANUM    2
#define XPS_CHARTYPE_ALPHA       4
#define XPS_CHARTYPE_NUMBER      8
#define XPS_CHARTYPE_WHITESPACE  16
#define XPS_CHARTYPE_ASCIIOTHER  32
#define XPS_CHARTYPE_EXTENDED    64

//Char XML class types
#define XPS_CHARCLASS_STANDARD        0  // all chars, except the following
#define XPS_CHARCLASS_XMLINIT         1  // ?
#define XPS_CHARCLASS_END             2  // /
#define XPS_CHARCLASS_TAG             3  // < and >
#define XPS_CHARCLASS_AMPERSAND       4  // &
#define XPS_CHARCLASS_QUOTES          5  // " "
#define XPS_CHARCLASS_NAMESPACE       6  // :
#define XPS_CHARCLASS_HTMLDECLARATION 7  // !
#define XPS_CHARCLASS_EXTENDED        8  // chars in the interval [127-255] and [0-32]

//Start types
#define XPS_STARTTYPE_DEFAULT  1
#define XPS_STARTTYPE_INIT     2
#define XPS_STARTTYPE_NAME     4
#define XPS_STARTTYPE_TAG      8
#define XPS_STARTTYPE_VALUE    16

//structure o'kawaiidesu

typedef struct Xps_CharProps
{
    char goodnessInContext;
    char charType;
    char charClass;
    char startType;
} Xps_CharProps;

#endif // XMLSTANDARD_H_INCLUDED
