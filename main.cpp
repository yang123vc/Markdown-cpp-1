#include <iostream>
#include <stdlib.h>

using namespace std;

#include "test.h"
#include "file.h"
#include "lib/libmarkdown.h"
#include "lib/HTMLParser.h"

const char* files[50] = {TEST_AMPS,TEST_AUTO_LINKS,TEST_BACKSLASH,TEST_BLOCKQUOTES,TEST_HARD_WRAPP,
                      TEST_HORIZONTAL_RULES,TEST_INLINE_HTML_ADV,TEST_INLINE_HTML_SIMPLE,TEST_INLINE_HTML_COM,
                      TEST_LINKS_INLINE,TEST_LINKS_REF,TEST_LITERAL,TEST_DOC_BASIC,TEST_DOC_SYNTAX,TEST_NESTED,
                      TEST_ORDERED,TEST_STRONG,TEST_TABS,TEST_TIDY,TEST_CUSTOM};


int main(int argc, char** argv)
{


    string content;
    if( argc == 1)
        content = get_file_contents(TEST_CUSTOM);
    else
        content = get_file_contents(TEST_CUSTOM);
    HTMLParser* parser = new HTMLParser();
    parser->print_headers(false);
    string parsed = parse( content, parser);

    cout << parsed << endl;

    set_file_contents("output/out.html",parsed);

    return 0;
}
