#include "parser.h"

#include "functions.h"

Parser::Parser() : content(""),block(""),temp("")
{
    empty_line = false;
}

Parser::~Parser()
{

}

void Parser::add_header()
{

}

void Parser::add_footer()
{

}

#include <iostream>
void Parser::new_line(string line)
{

    if( line.empty())
    {
        if( !block.empty())
        {
            edit_block(block);
            block = "";
        }
        if( empty_line)
            mul_empty_line_event();
        else
        {
            empty_line = true;
            empty_line_event();
        }
    }
    else
    {
        empty_line = false;
        bool newline = !block.empty();

        for( size_t i = 0; i < line.length(); i++)
        {
            const char* cont = line.c_str();
            int skip = 0;
            if( isspezial(cont[i]))
            {
                skip = spezial_char_event(&cont[i]);
            }
            else if( isreference(cont[i]))
            {
                skip = reference_event(&cont[i]);
            }
            else
            {
                block += cont[i];
            }

            if( skip != 0)
            {
                line.replace(i,skip,temp.c_str());
                temp = "";
                i--;
            }
        }
        if( newline)
            block += "\n";
    }
}

void Parser::edit_block(string block)
{
    insert(block);
}

void Parser::empty_line_event()
{

}

void Parser::mul_empty_line_event()
{

}

int Parser::spezial_char_event( const char* c)
{
    block += c[0];
    return 0;
}

int Parser::reference_event(const char* c)
{
    block += c[0];
    return 0;
}

bool Parser::isspezial( char c)
{
    return false;
}

bool Parser::isreference(char c)
{
    return false;
}

void Parser::flush()
{
    if( !block.empty())
    {
        edit_block(block);
        block = "";
    }
}

string Parser::get_content()
{
    return this->content;
}

void Parser::insert(string cont)
{
    this->content += cont;
    this->content += "\n";
}

void Parser::insert_block(string cont)
{
    this->block += cont;
    this->block += "\n";
}

void Parser::add(string cont)
{
    this->content += cont;
}

void Parser::add_block(string cont)
{
    this->block += cont;
}

void Parser::newline()
{
    this->content += "\n";
}

void Parser::add_temp(string cont)
{
    temp += cont;
}

void Parser::set_refs(map<string, Ref>* refs)
{
    this->refs = refs;
}

map<string,Ref>::iterator Parser::find_ref(string item)
{
    toLower(item);
    return refs->find(item);
}

bool Parser::found_ref(map<string,Ref>::iterator item)
{
    if( item != refs->end())
        return true;
    return false;
}
