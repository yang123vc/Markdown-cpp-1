#include "MarkdownParser.h"
#include <iostream>
#include <list>
#include <stdio.h>

#include "functions.h"

MarkdownParser::MarkdownParser()
{
    m_headers = true;
}

MarkdownParser::~MarkdownParser()
{

}

void MarkdownParser::add_footer()
{
    if( m_headers)
        footer_event();
}

void MarkdownParser::add_header()
{
    if( m_headers)
        header_event();
}

void MarkdownParser::print_headers(bool headers)
{
    m_headers = headers;
}


void MarkdownParser::parse()
{
    if( m_lines.empty())
        return;

    initial_manipulation();

    define_blocks(m_lines);

    /*for( string line : m_lines)
    {
        cout << line << endl;
    }*/

}

void MarkdownParser::define_blocks(list<string>& lines, int level)
{
    bool found = true;
    bool empty_line = false;
    for( list<string>::iterator line = lines.begin(); line != lines.end();)
    {
        if( (*line).length() > 1)
        {
            if( ishrule(*line))
            {
                (*line) = horizontal_rule_event();
                insert_level(level);
                insert(*line);
                insert_line();
                //cout << (*line) << endl;
            }
            else if( isheadline(*line)) // header block
            {
                (*line) = header_event(m_match[3],string(m_match[2])[0]-'0');
                insert_level(level);
                insert(*line);
                insert_line();
                //cout << (*line) << endl;
            }
            else if( isblockquote((*line))) // blockquote
            {
                //cout << blockquote_begin_event() << endl;
                insert_level(level);
                insert(blockquote_begin_event());
                insert_line();
                list<string> content;
                do
                {
                    if( !(*line).empty() && (*line)[0] == '>' )
                    {
                        string temp = (*line).substr(1);
                        size_t pos = temp.find_first_not_of(' ');
                        if( pos == string::npos)
                            content.push_back("");
                        else
                            content.push_back(temp.substr(pos));
                    }
                    else if( ispreformated(*line))
                    {
                        content.push_back((*line).substr(m_match.length(0)));
                    }
                    else
                        content.push_back(*line);
                    ++line;
                    if( line == lines.end())
                        break;
                    else if((*line).empty())
                    {
                        ++line;
                        if( line == lines.end() || !(isblockquote(*line) || ispreformated(*line)))
                        {
                            --line;
                            --line;
                            break;
                        }
                        else
                            content.push_back("");
                    }
                    else if( isheadline(*line))
                    {
                        --line;
                        break;
                    }
                }while(true);
                blcokquote_manipulation(content);
                define_blocks(content, level+1);

                /*for( string content_line : content)
                    cout << "{" << content_line << "}" << endl;*/
                //cout << blockquote_begin_event() << endl;
                insert_level(level);
                insert(blockquote_end_event());
                insert_line();
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                bool ordered = m_ordered;
                insert_level(level);
                if(ordered)
                    insert(ordered_list_begin_event());//cout << ordered_list_begin_event() << endl;
                else
                    insert( list_begin_event());//cout << list_begin_event() << endl;
                insert_line();
                do
                {
                    if( islist(*line) )
                    {
                        //cout << list_item_begin_event() << endl;
                        insert_level(level);
                        insert(list_item_begin_event());
                        insert_line();
                        list<string> content;
                        content.push_back((*line).substr(m_match.length(0)));
                        do
                        {
                            ++line;
                            if( line == lines.end() || isheadline(*line)
                             || islist(*line) || isblockquote(*line))
                            {
                                break;
                            }
                            else if( ispreformated(*line))
                            {
                                content.push_back((*line).substr(m_match.length(0)));
                            }
                            else
                                content.push_back(*line);
                        }while(true);

                        list_blocks(content, level+1);

                        /*for( string content_line : content)
                            cout << "{" << content_line << "}" << endl;*/

                        //cout << list_item_end_event() << endl;
                        insert_level(level);
                        insert(list_item_end_event());
                        insert_line();
                    }
                    else
                    {
                        cout << "don't know" << endl;
                        ++line;
                    }
                    if( line == lines.end())
                        break;
                    else if((*line).empty())
                    {
                        ++line;
                        if( line == lines.end() || !isblockquote(*line))
                        {
                            --line;
                            --line;
                            break;
                        }
                    }
                    else if( isheadline(*line))
                    {
                        --line;
                        break;
                    }
                }while(true);

                insert_level(level);
                if(ordered)
                    insert(ordered_list_end_event());//cout << ordered_list_end_event() << endl;
                else
                    insert(list_end_event());//cout << list_end_event() << endl;
                insert_line();
            }
            else
            {
                found = false;
            }
            empty_line = false;
        }
        else if((*line).empty())
        {
            if( !empty_line)
            {
                empty_line = true;
                //cout << endl;
                insert_line();
            }
        }

        if( !found) // Normal paragraph
        {
            found = true;
            list<string> content;
            string cont = paragraph_begin_event();
            do
            {
                cont += (*line);
                ++line;
                if( (line == lines.end()) || isheadline(*line) || isblock(*line)||(*line).empty())
                {
                    --line;
                    break;
                }
                else
                {
                    content.push_back(cont);
                    cont = "";
                }
            }while(true);
            cont += paragraph_end_event();
            content.push_back(cont);
            for( string content_line : content)
            {
                insert_level(level);
                insert(content_line);
                insert_line();
                //cout << content_line << endl;
            }
        }

        if( line != lines.end())
            ++line;
    }
}

void MarkdownParser::list_blocks(list<string>& lines, int level)
{
    bool empty_line = false;
    bool found = true;
    bool multiline = false;
    for( list<string>::iterator line = lines.begin(); line != lines.end();)
    {
        if( (*line).length() > 1)
        {
            if( ishrule(*line))
            {
                multiline = false;
                (*line) = horizontal_rule_event();
                //cout << (*line) << endl;
                insert_level(level);
                insert(*line);
                insert_line();
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                multiline = false;
                bool ordered = m_ordered;
                insert_level(level);
                if(ordered)
                    insert(ordered_list_begin_event());//cout << ordered_list_begin_event() << endl;
                else
                    insert(list_begin_event());//cout << list_begin_event() << endl;
                insert_line();
                do
                {
                    if( islist(*line) )
                    {
                        insert_level(level);
                        insert(list_item_begin_event());
                        insert_line();
                        //cout << list_item_begin_event() << endl;
                        list<string> content;
                        content.push_back((*line).substr(m_match.length(0)));
                        do
                        {
                            ++line;
                            if( line == lines.end() || isheadline(*line)
                             || islist(*line) || isblockquote(*line))
                            {
                                break;
                            }
                            else if( ispreformated(*line))
                            {
                                content.push_back((*line).substr(m_match.length(0)));
                            }
                            else
                                content.push_back(*line);
                        }while(true);

                        list_blocks(content, level+1);
                        //cout << list_item_end_event() << endl;
                        insert_level(level);
                        insert(list_item_end_event());
                        insert_line();
                    }
                    else
                    {
                        cout << "don't know" << endl;
                        ++line;
                    }
                    if( line == lines.end())
                        break;
                    else if((*line).empty())
                    {
                        ++line;
                        if( line == lines.end() || !isblockquote(*line))
                        {
                            --line;
                            --line;
                            break;
                        }
                    }
                    else if( isheadline(*line))
                    {
                        --line;
                        break;
                    }
                }while(true);

                insert_level(level);
                if(ordered)
                    insert(ordered_list_end_event());//cout << ordered_list_end_event() << endl;
                else
                    insert(list_end_event());//cout << list_end_event() << endl;
                insert_line();
            }
            else
            {
                found = false;
            }
            empty_line = false;
        }
        else if((*line).empty())
        {
            if( !empty_line)
            {
                empty_line = true;
                //cout << endl;
                insert_line();
            }
        }

        if( !found) // Normal paragraph
        {
            found = true;
            list<string> content;
            string cont = "";
            if( multiline)
            {
                cont = paragraph_begin_event();
            }
            do
            {
                cont += (*line);
                ++line;
                if( (line == lines.end()) || isblock(*line))
                {
                    --line;
                    break;
                }
                else if( (*line).empty())
                {
                    if( !multiline)
                    {
                        multiline = true;
                        if( content.empty())
                            cont = paragraph_begin_event() + cont;
                        else
                            content.front() = paragraph_begin_event() + content.front();
                        --line;
                    }
                    break;
                }
                else
                {
                    content.push_back(cont);
                    cont = "";
                }
            }while(true);
            if( multiline)
                cont += paragraph_end_event();
            content.push_back(cont);
            for( string content_line : content)
            {
                //cout << content_line << endl;
                insert_level(level);
                insert(content_line);
                insert_line();
            }
        }

        if( line != lines.end())
            ++line;
    }
}

bool MarkdownParser::isblock(string& block)
{
    return (isblockquote(block) || ishrule(block) || islist(block));
}

bool MarkdownParser::isblockquote(string& block)
{
    if( !block.empty() && block.length() > 2 && block[0] == '>' )
        return true;
    return false;
}

bool MarkdownParser::isheadline(string& block)
{
    regex headline("^([ ]{0,3})<h([1-6])>([^<]*)</h[1-6]>");
    return regex_search(block, m_match, headline);
}

bool MarkdownParser::ishrule(string& block)
{
    regex hrule("^[ ]{0,3}<[ ]?hr[ ]?/?>");
    return regex_search(block, m_match, hrule);
}

bool MarkdownParser::islist(string& block)
{
    regex anylist("^(\\*|\\+|-|[0-9]+\\.)[ ]+");
    return regex_search(block,m_match, anylist);
}

bool MarkdownParser::isunorderedlist(string& block)
{
    regex ulist("^(\\*|\\+|-) ");
    return regex_search(block,m_match, ulist);
}

bool MarkdownParser::isorderedlist(string& block)
{
    regex olist("^[0-9]+\\. ");
    m_ordered = regex_search(block,m_match, olist);
    return m_ordered;
}

bool MarkdownParser::ispreformated(string& block)
{
    regex preformated("^[ ]{4}|\\t");
    return regex_search(block, m_match, preformated);
}

void MarkdownParser::initial_manipulation()
{
    list<string> temp_lines;

    for( string line : m_lines)
    {

        if( !reference_line(line))
        {
            int mode;
            if( ( mode = header_line(line)) != 0)
            {
                if( mode == -1)
                {
                    if( !temp_lines.empty() && !temp_lines.back().empty())
                        temp_lines.push_back("");
                    temp_lines.push_back(line);
                    continue;
                }
                else if( mode > 0)
                {
                    if( !temp_lines.empty() && !temp_lines.back().empty())
                    {
                        string last = temp_lines.back();
                        temp_lines.pop_back();
                        if( !temp_lines.empty() && !temp_lines.back().empty())
                            temp_lines.push_back("");
                        temp_lines.push_back(header_event(last,mode));
                        continue;
                    }
                }
            }
            if( horizontal_rule_line(line))
            {
                if( !temp_lines.empty() && !temp_lines.back().empty())
                    temp_lines.push_back("");
                temp_lines.push_back(horizontal_rule_event());
            }
            else
            {
                temp_lines.push_back(line);
            }
        }
    }

    m_lines = temp_lines;
}

void MarkdownParser::blcokquote_manipulation(list<string>& lines)
{
    list<string> temp_lines;

    for( string line : lines)
    {

        int mode;
        if( ( mode = header_line(line)) != 0)
        {
            if( mode == -1)
            {
                if( !temp_lines.empty() && !temp_lines.back().empty())
                    temp_lines.push_back("");
                temp_lines.push_back(line);
                continue;
            }
            else if( mode > 0)
            {
                if( !temp_lines.empty() && !temp_lines.back().empty())
                {
                    string last = temp_lines.back();
                    temp_lines.pop_back();
                    if( !temp_lines.empty() && !temp_lines.back().empty())
                        temp_lines.push_back("");
                    temp_lines.push_back(header_event(last,mode));
                    continue;
                }
            }
        }
        if( horizontal_rule_line(line))
        {
            if( !temp_lines.empty() && !temp_lines.back().empty())
                temp_lines.push_back("");
            temp_lines.push_back(horizontal_rule_event());
        }
        else
        {
            temp_lines.push_back(line);
        }
    }

    lines = temp_lines;
}

bool MarkdownParser::reference_line(string& line)
{
    regex refs("^\\s*\\[([^\\]]+)\\]:\\s*(http://|ftp://|/|file://)"); // finding references in source
    smatch match;

    if( regex_search(line, match, refs))
    {
        string ref_line = match[2];
        ref_line += line.substr(match[0].length());
        list<string> paras = extract_parameter(ref_line);

        Ref referenz;
        list<string>::iterator jt = paras.begin();
        referenz.link = (*jt);

        // if there is more then one parameter given
        if( paras.size() > 1){
            ++jt;
            referenz.title = (*jt);
            for( ++jt; jt != paras.end(); ++jt)
                referenz.paras.push_back(*jt);
        }

        referenz.name = match[1];
        string name = referenz.name;
        tolower(name);
        m_refs[name] = referenz;

        return true;
    }

    return false;
}

int MarkdownParser::header_line(string& line)
{
    int mode = 0;
    smatch match;
    regex header_rule1("^(=+|-+)[ \\t\\f]*$");
    regex header_rule2("^(#{1,})([^#]*?)(#*)$");

    if( regex_search(line, match, header_rule1))
    {
        if( string(match[1])[0] == '=')
            mode = 1;
        else
            mode = 2;
    }
    else if( regex_search(line, match, header_rule2))
    {
        int len = string(match[1]).length();
        string hold = "";
        if( len > 6)
        {
            for( ; len > 6; len--)
                hold += "#";
        }
        string temp = hold + string(match[2]);
        trim(temp);
        line = header_event(temp,len);
        mode = -1;
    }

    return mode;
}

bool MarkdownParser::horizontal_rule_line(string& line)
{
    smatch match;
    regex horizontal_rule1("^[ ]{0,3}\\*([ ]{0,2}\\*){2,}[ \\t\\f]*$");
    regex horizontal_rule2("^[ ]{0,3}-([ ]{0,2}-){2,}[ \\t\\f]*$");
    return (regex_search(line, match, horizontal_rule1)
         || regex_search(line, match, horizontal_rule2));

}

void MarkdownParser::insert_level(int level)
{
    for( int i = 0; i < level; i++)
        insert("\t");
}

void MarkdownParser::insert_line()
{
    insert("\n");
}
