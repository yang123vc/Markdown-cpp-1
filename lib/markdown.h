#ifndef MARKDOWN_H_
#define MARKDOWN_H_

#include <string>

#include "parser.h"

using namespace std;

string parse( string& content);
string parse( string& content, Parser* parser);

#endif // MARKDOWN_H_
