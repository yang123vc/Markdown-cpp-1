#include <iostream>


using namespace std;

#include "test.h"
#include "file.h"
#include "lib/markdown.h"
#include "lib/htmlParser.h"


int main()
{

    string content = get_file_contents(TEST_AUTO_LINKS);

    string parsed = parse( content, new HTMLParser(false));

    cout << parsed << endl;

    set_file_contents("output/out.html",parsed);

    return 0;
}
