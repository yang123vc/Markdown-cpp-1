#include "htmlParser.h"

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

void HTMLParser::paragraph_begin_event()
{
    insert( "<p>");
}

void HTMLParser::paragraph_end_event()
{
    insert( "</p>");
}

void HTMLParser::blockquote_begin_event()
{
     insert( "<blockquote>");
}

void HTMLParser::blockquote_end_event()
{
    insert( "</blockquote>");
}

void HTMLParser::list_item_begin_event()
{
    insert( "<li>");
}

void HTMLParser::list_item_end_event()
{
    insert( "</li>");
}


void HTMLParser::list_begin_event()
{
    insert( "<ul>");
}

void HTMLParser::list_end_event()
{
    insert( "</ul>");
}

void HTMLParser::ordered_list_begin_event()
{
    insert( "<ol>");
}

void HTMLParser::ordered_list_end_event()
{
    insert( "</ol>");
}

void HTMLParser::preformat_begin_event()
{
    insert( "<pre>");
}

void HTMLParser::preformat_end_event()
{
    insert( "</pre>");
}

void HTMLParser::code_begin_event(string lang)
{
    if( lang.empty())
        insert( "<code>");
    else
        insert( "<code class='"+lang+"'>");
}

void HTMLParser::code_end_event()
{
    insert( "</code>");
}

void HTMLParser::new_line_event()
{
    insert(" <br />");
}

void HTMLParser::comment_begin_event()
{
    insert("<!--");
}

void HTMLParser::comment_end_event()
{
    insert("-->");
}

void HTMLParser::html_begin_event(string& tag, string& paras)
{
    insert("<" + tag + paras + ">");
}

void HTMLParser::html_end_event(string& tag)
{
    insert("</"+tag+">");
}

void HTMLParser::text_bold_begin_event()
{
    insert("<strong>");
}

void HTMLParser::text_bold_end_event()
{
    insert("</strong>");
}

void HTMLParser::text_italic_begin_event()
{
    insert("<em>");
}

void HTMLParser::text_italic_end_event()
{
    insert("</em>");
}


void HTMLParser::replace_code_char(const char& c)
{
    switch( c)
    {
        case '<':
            insert( "&lt;");
        break;
        case '>':
            insert( "&gt;");
        break;
        case '&':
            insert( "&amp;");
        break;
        default:
            insert( c);
        break;
    }
}

void HTMLParser::replace_char(const char& c)
{
    switch( c)
    {
        case '&':
            insert( "&amp;");
        break;

        case '<':
            insert( "&lt;");
        break;

        default:
            insert( c);
        break;
    }
}

int HTMLParser::check_escaped_char(const char& c)
{
    int len = 1;
    switch(c)
    {
        case '*':
        case '_':
        case '-':
        case '+':
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case '>':
        case '\\':
        case '#':
        case '`':
        case '.':
        case '!':
            insert(c);
        break;
        default:
            len = 0;
        break;
    }
    return len;
}

void HTMLParser::generate_link(Ref ref, string& name)
{
    insert("<a href='");
    for( char c : ref.link)
        replace_char(c);
    insert("'>");
    for( char c : name)
        replace_char(c);
    insert("</a>");
}

void HTMLParser::generate_img(Ref src, string& alt)
{
    insert("<img src='");
    for( char c : src.link)
        replace_char(c);
    insert("' alt='");
    for( char c : alt)
        replace_char(c);
    insert("'/>");
}
