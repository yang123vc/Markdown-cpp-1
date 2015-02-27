#include "parser.h"

#include "functions.h"

#include <string.h>
#include <iostream>

void Parser::set_lines(list<string>& lines)
{
    m_lines = lines;
}

string Parser::get_content()
{

    while(!m_content.empty() && m_content.back()== '\n')
        m_content.pop_back();
    m_content += '\n';

    return m_content;
}

list<string> Parser::extract_parameter(string& line)
{
    list<string> params;

    for( size_t i = 0; i < line.length(); i++)
    {
        if( isspace(line[i]))
            continue;
        else if( line[i] == '\"' || line[i] == '\'')
        {
            char c = line[i++];
            string temp = "";
            while( i < line.length() && (line[i] != c))
                temp += line[i++];
            params.push_back(temp);
        }
        else
        {
            string temp = "";
            while( i < line.length() && !isspace(line[i]))
                temp += line[i++];
            params.push_back(temp);
        }
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
