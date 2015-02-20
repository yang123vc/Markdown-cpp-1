#include "HTMLParser.h"

#include <stdio.h>
#include <iostream>

HTMLParser::HTMLParser()
{
    //ctor
}

HTMLParser::~HTMLParser()
{
    //dtor
}

void HTMLParser::footer_event()
{

}

void HTMLParser::header_event()
{

}

string HTMLParser::header_event(string line, int level)
{
    char buffer[line.length() + 12];
    sprintf(buffer, "<h%d>%s</h%d>", level, line.c_str(), level);
    return buffer;
}

string HTMLParser::horizontal_rule_event()
{
    return "<hr />";
}

string HTMLParser::paragraph_begin_event()
{
    return "<p>";
}

string HTMLParser::paragraph_end_event()
{
    return "</p>";
}

string HTMLParser::blockquote_begin_event()
{
     return "<blockquote>";
}

string HTMLParser::blockquote_end_event()
{
    return "</blockquote>";
}

string HTMLParser::list_item_begin_event()
{
    return "<li>";
}

string HTMLParser::list_item_end_event()
{
    return "</li>";
}


string HTMLParser::list_begin_event()
{
    return "<ul>";
}

string HTMLParser::list_end_event()
{
    return "</ul>";
}

string HTMLParser::ordered_list_begin_event()
{
    return "<ol>";
}

string HTMLParser::ordered_list_end_event()
{
    return "</ol>";
}

string HTMLParser::preformat_begin_event()
{
    return "<pre>";
}

string HTMLParser::preformat_end_event()
{
    return "</pre>";
}

string HTMLParser::code_begin_event(string lang)
{
    if( lang.empty())
        return "<code>";
    return "<code class='"+lang+"'>";
}

string HTMLParser::code_end_event()
{
    return "</code>";
}
