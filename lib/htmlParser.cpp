#include "htmlParser.h"

#include <iostream>
#include <string.h>
#include <regex>

#include "functions.h"

HTMLParser::HTMLParser(bool header)
{
    this->header = header;
}

HTMLParser::~HTMLParser()
{

}

void HTMLParser::add_header()
{
    if( header)
        insert("<html>");
}

void HTMLParser::add_footer()
{
    if( header)
        insert("</html>");
}

void HTMLParser::edit_block(string block)
{
    add("<p>");
    add(block);
    add("</p>");
    newline();
}

void HTMLParser::empty_line_event()
{
    newline();
}

int HTMLParser::spezial_char_event(const char* c)
{
    if( c[0] == '&')
    {
        if( strncmp(c, "&amp;", 5) != 0)
        {
            add_block("&amp;");
        }
        else
        {
            add_block("&");
        }
    }
    else if( c[0] == '<')
    {
        if( isspace(c[1]) || isdigit(c[1]))
        {
            add_block("&lt;");
        }
        else
        {
            regex expression("^<(http|ftp|file)(://[^?]*)([^>]*)>");
            smatch match;
            if( regex_search(string(c), match, expression))
            {
                Ref dummy;
                dummy.name = match[1];
                dummy.name += match[2];
                dummy.name += match[3];
                dummy.link = dummy.name;
                insert_link(dummy);
                return match.length();
            }
            else
                add_block("<");
        }
    }
    return 0;
}

void HTMLParser::insert_link(Ref item, string name)
{
    add_temp("<a href='");
    add_temp(item.link);
    add_temp("'");
    if( !item.titel.empty())
    {
        add_temp(" title='");
        add_temp(item.titel);
        add_temp("'");
    }
    add_temp(">");
    if(name.empty())
        add_temp(item.name);
    else
        add_temp(name);
    add_temp("</a>");
}

int HTMLParser::reference_event(const char* c)
{
    // Match 1 = Image (Optional)
    // Match 2 = referenz name
    // Match 3 = id (Optional) or link and optional parameter
    regex reg("^(!)?\\[([^\\]]*)\\][ \t]?(\\([^\\)]*\\)|\\[[^\\]]*\\])?");
    smatch match;
    Ref ref;
    map<string,Ref>::iterator item;
    string image;
    string name;
    string id;

    if( regex_search(string(c), match, reg))
    {
        image = match[1];
        name = match[2];
        id = match[3];
        if( image.length() == 0) // Link no image
        {
            if(id.length() == 0)
            {

                item = find_ref(name);
                if( found_ref(item) )
                {
                    insert_link((*item).second,name);
                }
            }
            else
            {

                if( id[0] == '[')
                {
                    string subid = id.substr(1, id.length()-2);
                    item = find_ref(subid);
                    if( !found_ref(item) )
                    {
                        item = find_ref(name);

                    }

                    if( found_ref(item) )
                    {
                        insert_link((*item).second,name);
                    }
                }
                else
                {
                    string subid = id.substr(1, id.length()-2);
                    if(subid[0] == '<')
                    {
                        subid = subid.substr(1);
                    }
                    if(subid[subid.length()-1] == '>')
                    {
                        subid = subid.substr(0,subid.length()-1);
                    }
                    vector<string> parameters = parameter(subid); // parse the parameters

                    Ref dummy;
                    dummy.link = parameters[0];
                    // if there is more then one parameter given
                    if( parameters.size() > 1){
                        dummy.titel = parameters[1];
                        for( vector<string>::iterator jt = parameters.begin()+2; jt != parameters.end(); jt++)
                            dummy.paras.push_back(*jt);
                    }
                    dummy.name = name;
                    insert_link(dummy,name);
                }
            }
        }
        else // image no link
        {

        }
    }
    else
    {
        add_block(c);
    }

    return match.length();

}

bool HTMLParser::isspezial(char c)
{

    if( c == '&')
    {
        return true;
    }
    else if( c == '<')
    {
        return true;
    }

    return false;
}

bool HTMLParser::isreference(char c)
{
    if( c == '[' || c == '!')
        return true;
    return false;
}

