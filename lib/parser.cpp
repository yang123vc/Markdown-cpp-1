#include "parser.h"

#include <string.h>

void Parser::set_lines(list<string>& lines)
{
    m_lines = lines;
}

string Parser::get_content()
{
    return m_content;
}

list<string> Parser::extract_parameter(string& line)
{
    list<string> params;
    char para[512];
    int j = 0;
    bool wasSpace = false;

    for( size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];
        if( isspace(c))
        {
            if( !wasSpace)
            {
                para[j] = '\0';
                params.push_back(para);
                j = 0;
                wasSpace = true;
            }
        }
        else if( c == '\"' || c == '\'')
        {
            wasSpace = false;
            while( (i < line.length()) && (line[i] != c))
                para[j++] = line[++i];
        }
        else
        {
            para[j++] = c;
            wasSpace = false;
        }
    }
    if( j!= 0)
    {
        para[j] = '\0';
        params.push_back(para);
    }

    return params;
}

void Parser::insert(string content)
{
    m_content += content;
}

void Parser::insert(const char& c)
{
    m_content += c;
}
