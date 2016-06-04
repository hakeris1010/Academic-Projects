#include "xmlstandard.h"
#include "string.h"

//Predefined self-close tags: ( separated by | ).
const char* HTML_SelfCloseTags = "area|base|br|col|command|embed|hr|img|input|keygen|link|meta|param|source|track|wbr";

//Interval specified by [...-...], and then by separated by |
//Startin' chars
const char* XML_NameStartChars = ":|[A-Z]|_|[a-z]|[#xC0-#xD6]|[#xD8-#xF6]";

//Valid chars
const char* XML_ValidNameChars = ":|[A-Z]|_|[a-z]|[#xC0-#xD6]|[#xD8-#xF6]|-|.|[0-9]|#xB7";

