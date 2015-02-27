#include "markdownParser.h"

#include "functions.h"

MarkdownParser::MarkdownParser()
{
    m_ordered = false;
    print_headers(true);
    m_level = 0;
    m_bold = regex("^((?:\\*|_){2})((?!\\s|\\1).*?[^\\s\\\\](?:\\*|_)?)\\1");
    m_italic = regex("^(\\*|_)((?!\\s|\\1).*?[^\\s\\\\])\\1");
    m_unicodeChar = regex("^&([a-zA-Z]+|#[0-9]+);");
    m_comment = regex("^<!--(.*?)-->");
    m_htmlTags = regex("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>([^\n\r]*?)</\\1>");
    m_htmlOpenTag = regex("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>");
    m_htmlblock = regex("^[ ]{0,3}<(p|div|table|ul|li|ol|blockquote|form|pre|section)\\b([^>]*)>");
    m_htmlcomment = regex("^[ ]{0,3}<!--");
    m_htmlCloseTag = regex("^</([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>");
    m_htmlTag = regex("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^/]*)[ ]*/[ ]*>");
    m_inline_code = regex("^([`]{1,2})(.+?)\\1");
    m_headline = regex("^(?:[ ]{0,3})<h([1-8])\\b([^>]*)>([^<]*)</h[1-8]>");
    m_header_rule1 = regex("^(=+|-+)[ \\t\\f]*$");
    m_header_rule2 = regex("^(#{1,})([^#]*?)(#*)$");
    m_horizontal_rule1 = regex("^[ ]{0,3}\\*([ ]{0,2}\\*){2,}[ \\t\\f]*$");
    m_horizontal_rule2 = regex("^[ ]{0,3}(-|_)([ ]{0,2}\\1){2,}[ \\t\\f]*$");
    m_hrule = regex("^[ ]{0,3}<[ ]?hr([^>]*?)/?>");
    m_anylist = regex("^(\\*|\\+|-|[0-9]+\\.)(?: {0,3}|\\t)");
    m_ulist = regex("^(\\*|\\+|-)(?: {1,3}|\\t)");
    m_olist = regex("^[0-9]+\\.(?: {1,3}|\\t)");
    m_preformated = regex("^([ ]{4}|\\t)");
    m_code = regex("^```(.*)");
    m_reference_db = regex("^(!?)\\[([^\\]\\[]+?(?:\\[[^\\]]*\\])*)\\](?: |\n)?(\\[([^\\]\\[]*?(?:\\[[^\\]]*\\])*)\\])?");
    m_reference_inline = regex("^(!?)\\[([^\\]]+)\\] ?\\(<?((?:#|http://|/|ftp://|file://)?[^\\)]*?)>?\\)");
    m_links = regex("^<((?:(?:http|ftp|file)://)[^\\s]+)>");
    m_email = regex("^<([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6})>");
    m_force_line = regex("^[ ]{2,}($|\n)");
    m_ref = regex("^\\s{0,3}\\[([^\\]]+)\\]:\\s*(#|http://|ftp://|/|file://)");// finding references in source
    m_word = regex("^((?:[a-zA-Z]+[a-zA-Z0-9]*|[0-9]+) ?)");
}

MarkdownParser::~MarkdownParser()
{

}

void MarkdownParser::set_ground_level( int level)
{
    m_level = level;
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
    define_blocks(m_lines, m_level);

}

void MarkdownParser::define_blocks(list<string>& lines, int level)
{
    bool found = true;
    for( list<string>::iterator line = lines.begin(); line != lines.end();)
    {
        string temp = *line;
        ltrim(temp);
        if( temp.empty())
        {
            line++;
            insert_line();
            continue;
        }

        if( (*line).length() > 1)
        {
            if( ishrule(*line))
            {
                hrule_block(line, level, lines.end());
            }
            else if( isheadline(*line)) // header block
            {
                headline_block(line, level, lines.end());
            }
            else if( ishtmlblock(*line))
            {
                html_block(line, level, lines.end());
            }
            else if( ishtmlcomment(*line))
            {
                comment_block(line, level, lines.end());
            }
            else if( isblockquote(*line)) // blockquote
            {
                blockquote_block(line, level, lines.end());
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                list_block(line, level, lines.end());
            }
            else if( iscodeblock(*line)) // code block
            {
                code_block(line, level, lines.end());
            }
            else if( ispreformated(*line)) // code block
            {
                preformated_block(line, level, lines.end());
            }
            else
                found = false;
        }

        if( !found) // Normal paragraph
        {
            found = true;
            paragraph_block(line, level, lines.end());
        }

        if( line != lines.end())
        {
            ++line;
            insert_line();
        }
    }
}

void MarkdownParser::list_blocks(list<string>& lines, int level)
{
    bool found = true;
    bool multiline = false;
    for( list<string>::iterator line = lines.begin(); line != lines.end();)
    {

        if( (*line).empty())
        {
            line++;
            continue;
        }

        if( (*line).length() > 1)
        {
            if( ishrule(*line))
            {
                multiline = false;
                hrule_block(line, level, lines.end());
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                multiline = false;
                list_block(line, level, lines.end());
            }
            else
            {
                found = false;
            }
        }

        if( !found) // Normal paragraph
        {
            found = true;
            normal_block(line, level, multiline, lines.end());
        }

        if( line != lines.end())
        {
            ++line;
        }
    }
}

void MarkdownParser::headline_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    insert_level(level);
    insert(header_event(string(m_match[3]),string(m_match[1])[0]-'0', m_match.str(2)));
    insert_line();
}

void MarkdownParser::hrule_block( list<string>::iterator& line, int level, list<string>::iterator ende)
{
    insert_level(level);
    insert(horizontal_rule_event(m_match[1]));
    insert_line();
}

void MarkdownParser::code_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    insert_level(level);
    preformat_begin_event();
    code_begin_event(m_match[1]);

    do
    {
        ++line;
        if( line == ende || iscodeblock(*line))
            break;
        string temp = "";
        for( size_t j = 0; j < (*line).length();++j)
        {
            replace_code_char((*line)[j]);
        }
        insert_line();
    }while(true);
    code_end_event();
    preformat_end_event();
    insert_line();
}

void MarkdownParser::preformated_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    bool empty_line = false;
    insert_level(level);
    preformat_begin_event();
    code_begin_event();
    string lien;
    do
    {
        if( empty_line)
            insert_line();
        if( ispreformated(*line))
        {
            lien = (*line).substr(m_match.length(1));
            //for( size_t j = 0; j < lien.length();++j)
            {
                replace_code_char(lien);//[j]);
            }
            ltrim(lien);
        }
        empty_line = false;
        ++line;
        if( line == ende)
            break;
        else if( (*line).empty())
        {
            empty_line = true;
            ++line;
        }

        if( isblockquote(*line) || !ispreformated(*line))
        {
            --line;
            if( empty_line)
                --line;
            break;
        }
        insert_line();

    }while(true);
    if( !lien.empty())
        insert_line();
    code_end_event();
    preformat_end_event();
    insert_line();
}

void MarkdownParser::blockquote_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    insert_level(level);
    blockquote_begin_event();
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
                content.push_back(temp.substr(pos>0));
        }
        else if( ispreformated(*line))
        {
            content.push_back((*line).substr(m_match.length(0)));
        }
        else
            content.push_back(*line);
        ++line;
        if( line == ende)
            break;
        else if((*line).empty())
        {
            ++line;
            if( line == ende || !(isblockquote(*line) || ispreformated(*line)))
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
    blockquote_manipulation(content);
    define_blocks(content, level+1);

    insert_level(level);
    blockquote_end_event();
    insert_line();
}

void MarkdownParser::list_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    bool ordered = m_ordered;
    bool empty_line = false;
    insert_level(level);
    if(ordered)
        ordered_list_begin_event();
    else
        list_begin_event();
    insert_line();
    do
    {
        if( islist(*line) )
        {
            insert_level(level+1);
            list_item_begin_event();
            insert_line();
            list<string> content;
            content.push_back((*line).substr(m_match.length(0)));
            do
            {
                empty_line = false;
                ++line;

                while( line != ende && (*line).empty())
                {
                    empty_line = true;
                    ++line;
                }
                if( line == ende || isheadline(*line) || isblockquote(*line))
                    break;
                else if(islist(*line))
                {
                    if( empty_line)
                        content.push_back("");
                    break;
                }
                else if( ispreformated(*line))
                {
                    if( empty_line)
                        content.push_back("");
                    content.push_back((*line).substr(m_match.length(0)));
                }
                else
                {
                    if( empty_line)
                    {
                        --line;
                        break;
                    }
                    content.push_back(*line);
                }
            }while(true);

            list_blocks(content, level+2);

            insert_level(level+1);
            list_item_end_event();
            insert_line();
        }

        if( line == ende)
            break;
        else if((*line).empty())
        {
            ++line;
            if( line == ende || isblockquote(*line) || iscodeblock(*line))
            {
                --line;
                --line;
                break;
            }
        }
        else if( !islist(*line) && !ispreformated(*line))
        {
            --line;
            break;
        }
    }while(true);

    insert_level(level);
    if(ordered)
        ordered_list_end_event();
    else
        list_end_event();
    insert_line();
}

void MarkdownParser::html_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    list<string> content;
    string tag = m_match[1];
    string paras = m_match[2];
    string rest = m_match[3];

    insert_level(level);
    html_begin_event(tag, paras);
    *line = (*line).substr(m_match.length(0));

    int balance = 1;
    size_t max_length = 0;

    do
    {
        max_length = get_tag_balance(tag, *line, balance);
        if( balance == 0)
            break;
        if( tag.compare("code"))
        {
            ltrim(*line);
        }
        content.push_back(*line);
        ++line;
        if( line == ende)
            break;
        if( tag.compare("p") == 0)
        {
            if( (*line).empty())
                break;
        }

    }while(true);

    if( max_length != string::npos)
    {
        content.push_back((*line).substr(0,max_length));
        if( (*line).length() > (max_length + tag.length() + 3))
        {
            *line = (*line).substr(max_length + tag.length() + 3);
            --line;
        }
    }
    else
        content.push_back(*line);

    while(content.back().empty()) content.pop_back();

    for( list<string>::iterator it = content.begin(); it != content.end(); ++it)
    {
        if( ishtmlblock(*it))
        {
            html_block(it, level+1, content.end());
        }
        else if( (*it).empty())
        {
            insert_line();
        }
        else
        {
            if( content.size() > 1)
                insert_level(level+1);
            parse_line(*it);
            if( content.size() > 1)
                insert_line();
        }
    }

    if( content.size() > 1)
    {
        insert_level(level);
    }
    html_end_event(tag);
    insert_line();
}

void MarkdownParser::comment_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    insert_level(level);
    comment_begin_event();
    *line = (*line).substr(m_match.length(0));
    list<string> content;
    size_t max_length = 0;

    do
    {
        max_length = (*line).find("-->");
        if( max_length != string::npos)
            break;
        content.push_back(*line);
        ++line;
        if( line == ende)
            break;
    }while(true);

    if( max_length != string::npos)
    {
        content.push_back((*line).substr(0,max_length));
        if( (*line).length() > (max_length + 3))
        {
            *line = (*line).substr(max_length + 3);
            --line;
        }
    }
    else
        content.push_back(*line);

    while(content.back().empty()) content.pop_back();

    for( string content_line : content)
    {
        if( content.size() > 1)
            insert_level(level);
        insert(content_line);
        if( content.size() > 1)
        insert_line();
    }

    if( content.size() > 1)
    {
        insert_level(level);
    }
    comment_end_event();
    insert_line();
}

void MarkdownParser::paragraph_block(list<string>::iterator& line, int level, list<string>::iterator ende)
{
    string content;
    insert_level(level);
    paragraph_begin_event();
    string cont = "";
    do
    {
        cont += (*line);
        ++line;
        if( (line == ende) || isheadline(*line) ||(*line).empty())
        {
            --line;
            break;
        }
        else
        {
            content += cont;
            cont = "\n";
            for( int i = 0; i < level; i++)
                cont += "    ";
        }
    }while(true);

    content += cont;
    parse_line(content);
    paragraph_end_event();
    insert_line();
}

void MarkdownParser::normal_block(list<string>::iterator& line, int level, bool& multiline, list<string>::iterator ende)
{
    string content;
    string cont = "";
    insert_level(level);
    if( multiline)
    {
        paragraph_begin_event();
    }
    do
    {
        cont += (*line);
        ++line;
        if( (line == ende) || isblock(*line))
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
                {
                    paragraph_begin_event();
                    content += cont;
                    cont = "\n";
                }
                else
                {
                    paragraph_begin_event();
                }
                --line;
            }
            break;
        }
        else
        {
            content += cont;
            cont = "\n";
        }
    }while(true);

    //content.push_back(cont);
    if( cont.back() == '\n')
        cont.pop_back();
    content += cont;

    parse_line(content);
    if( multiline)
        paragraph_end_event();
    insert_line();
}

bool MarkdownParser::isblock(string& line)
{
    return (isblockquote(line) || ishrule(line)
         || islist(line) || ispreformated(line) || iscodeblock(line));
}

bool MarkdownParser::isblockquote(string& line)
{
    if( !line.empty() && line.length() > 2 && line[0] == '>')
        return true;
    return false;
}

bool MarkdownParser::isheadline(string& line)
{
    return regex_search(line, m_match, m_headline);
}

bool MarkdownParser::ishrule(string& line)
{
    return regex_search(line, m_match, m_hrule);
}

bool MarkdownParser::ishtmlblock(string& line)
{
    return regex_search(line, m_match, m_htmlblock);
}

bool MarkdownParser::ishtmlcomment(string& line)
{
    return regex_search(line, m_match, m_htmlcomment);
}

bool MarkdownParser::islist(string& line)
{
    return regex_search(line,m_match, m_anylist);
}

bool MarkdownParser::isunorderedlist(string& line)
{
    return regex_search(line,m_match, m_ulist);
}

bool MarkdownParser::isorderedlist(string& line)
{
    m_ordered = regex_search(line,m_match, m_olist);
    return m_ordered;
}

bool MarkdownParser::ispreformated(string& line)
{
    return regex_search(line, m_match, m_preformated);
}

bool MarkdownParser::iscodeblock(string& line)
{
    return regex_search(line, m_match, m_code);
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
                    string last_line = temp_lines.back();
                    trim(last_line);
                    if( !temp_lines.empty() && !last_line.empty())
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

void MarkdownParser::blockquote_manipulation(list<string>& lines)
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

void MarkdownParser::parse_line(string& s)
{

    for( size_t i = 0; i < s.length(); ++i)
    {
        size_t pos = 0;
        string temp = s.substr(i);
        if( isspace(temp[0]))
        {
            replace_char(temp[0]);
        }
        else if( (pos = find_word(temp)))
        {
             i+= pos-1;
        }
        else if( (pos = find_bold(temp)))
        {
            i+= pos;
        }
        else if( (pos = find_italic(temp)))
        {
            i += pos;
        }
        else if( (pos = find_comment(temp)))
        {
            i += pos;
        }
        else if( (pos = find_links(temp)))
        {
            i += pos;
        }
        else if( (pos = find_references(temp)))
        {
            i += pos;
        }
        else if( (pos = find_htmlTags(temp)))
        {
            i += pos;
        }
        else if( (pos = find_inline_code(temp)))
        {
            i += pos;
        }
        else if( (pos = find_new_line(temp)))
        {
            i += pos;
        }
        else if( (pos = find_unicode(temp)))
        {
            i += pos;
        }
        else if( temp[0] == '\\')
        {
            if( temp.length() > 1)
            {
                i += check_escaped_char(temp[1]);
            }
            else
                replace_char(temp[0]);
        }
        else
        {
            replace_char(temp[0]);
        }
    }
}

void MarkdownParser::parse_code_line(string& s)
{
    for( size_t i = 0; i < s.length(); ++i)
    {
        replace_code_char(s[i]);
    }
}

int MarkdownParser::find_word(string& s)
{
    size_t len = 0;

    if( regex_search( s, m_match, m_word))
    {
        len = m_match.length(0);
        string temp = m_match.str(1);
        for(char c : temp)
            replace_char(c);
    }

    return len;
}

int MarkdownParser::find_bold(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_bold))
    {
        len = m_match.length(0)-1;
        text_bold_begin_event();
        string strong_text = m_match[2];
        for( size_t j = 0; j < strong_text.length(); ++j)
        {
            string temp = strong_text.substr(j);
            size_t pos;
            if( isspace(temp[0]))
            {
                replace_char(temp[0]);
            }
            else if( (pos = find_word(temp)))
            {
                j += pos-1;
            }
            else if( (pos = find_italic(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_unicode(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_htmlTags(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_references(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_links(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_comment(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_inline_code(temp)))
            {
                j+=pos;
            }
            else if( temp[0] == '\\')
            {
                if( temp.length() > 1)
                {
                    j += check_escaped_char(temp[1]);
                }
                else
                    replace_char(temp[0]);
            }
            else
            {
                replace_char(temp[0]);
            }
        }
        text_bold_end_event();
    }

    return len;
}

int MarkdownParser::find_italic(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_italic))
    {
        len = m_match.length(0)-1;
        text_italic_begin_event();
        string italic_text = m_match[2];
        for( size_t j = 0; j < italic_text.length(); ++j)
        {
            string temp = italic_text.substr(j);
            size_t pos;
            if( isspace(temp[0]))
            {
                replace_char(temp[0]);
            }
            else if( (pos = find_word(temp)))
            {
                j += pos-1;
            }
            else if( (pos = find_unicode(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_htmlTags(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_references(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_links(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_comment(temp)))
            {
                j+=pos;
            }
            else if( (pos = find_inline_code(temp)))
            {
                j+=pos;
            }
            else if( temp[0] == '\\')
            {
                if( temp.length() > 1)
                {
                    j += check_escaped_char(temp[1]);
                }
                else
                    replace_char(temp[0]);
            }
            else
            {
                replace_char(temp[0]);
            }
        }
        text_italic_end_event();
    }

    return len;
}

int MarkdownParser::find_unicode(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_unicodeChar))
    {
        len = m_match.length(0)-1;
        string unicode = m_match[0];
        insert(unicode);
    }

    return len;
}

int MarkdownParser::find_comment(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_comment))
    {
        len = m_match.length(0)-1;
        comment_begin_event();
        insert(m_match[1]);
        comment_end_event();
    }

    return len;
}

int MarkdownParser::find_htmlTags(string& s)
{
    size_t len = 0;

   if( regex_search(s, m_match, m_htmlTags))
    {
        len = m_match.length(0)-1;
        string tag = m_match[1];
        string paras = m_match[2];
        string text = m_match[3];
        html_begin_event(tag, paras);
        if( tag.compare("code"))
        {

            for( size_t j = 0; j < text.length(); ++j)
            {
                size_t pos;
                string temp = text.substr(j);
                if( isspace(temp[0]))
                {
                    replace_char(temp[0]);
                }
                else if( (pos = find_word(temp)))
                {
                    j += pos-1;
                }
                else if( (pos = find_htmlTags(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_inline_code(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_bold(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_italic(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_unicode(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_comment(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_references(temp)))
                {
                    j+=pos;
                }
                else if( (pos = find_links(temp)))
                {
                    j+=pos;
                }
                else if( temp[0] == '\\')
                {
                    if( temp.length() > 1)
                    {
                        j += check_escaped_char(temp[1]);
                    }
                    else
                        replace_char(temp[0]);
                }
                else
                {
                    replace_char(temp[0]);
                }
            }
        }
        else
        {
            replace_code_char(text);
        }
        html_end_event(tag);
    }

    return len;
}

int MarkdownParser::find_open_htmlTags(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_htmlOpenTag))
    {

    }
    return len;
}

int MarkdownParser::find_inline_code(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_inline_code))
    {
        len = m_match.length(0)-1;
        string code_string = m_match.str(2);
        code_begin_event();
        for( size_t j = 0; j < code_string.length(); j++)
        {
            replace_code_char(code_string[j]);
        }
        code_end_event();
    }

    return len;
}

int MarkdownParser::find_references(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_reference_inline))
    {
        len = m_match.length(0)-1;
        bool image = m_match.length(1) > 0;
        Ref dummy;
        dummy.name = m_match.str(2);
        string temp = m_match.str(3);
        list<string> paras = extract_parameter(temp);
        if( paras.size() > 0)
        {
            dummy.link = paras.front();
            paras.pop_front();
            if( paras.size() > 0)
            {
                dummy.title = paras.front();
                paras.pop_front();
                if( paras.size() > 0)
                    dummy.paras = paras;
            }
        }
        else
            dummy.link = "";

        if( image)
            generate_img(dummy, dummy.name);
        else
            generate_link(dummy, dummy.name);
    }
    else if( regex_search(s, m_match, m_reference_db))
    {
        len = m_match.length(0)-1;
        bool image = m_match.length(1) > 0;

        if( (m_match.length(3) != 0) && m_match.str(3).compare("[]"))
        {
            string name = m_match.str(2);
            string temp_ref = m_match.str(3).substr(1,m_match.length(3)-2);
            toLower(temp_ref);
            Ref refs = m_refs[temp_ref];
            if( refs.link.empty())
            {
                string dummy = m_match.str(0);
                for( size_t j = 0; j < dummy.length(); ++j)
                {
                    replace_char(dummy[j]);
                }
            }
            else
            {
                if( image)
                    generate_img(refs, name);
                else
                    generate_link(refs, name);
            }
        }
        else
        {
            string name = m_match.str(2);
            toLower(name);
            Ref refs = m_refs[name];
            name = m_match.str(2);
            if( refs.link.empty())
            {
                name = "["+name+"]" + m_match.str(3);
                for( size_t j = 0; j < name.length(); ++j)
                {
                    replace_char(name[j]);
                }
            }
            else
            {
                if( image)
                    generate_img(refs, name);
                else
                    generate_link(refs, name);
            }
        }
    }

    return len;
}

int MarkdownParser::find_links(string& s)
{
    size_t len = 0;
    if( regex_search(s, m_match, m_email))
    {
        len = m_match.length(0)-1;
        string email = hide_chars(m_match.str(1));
        Ref refs;
        refs.link = hide_chars("mailto:") + email;
        generate_link_no_replace(refs, email);
    }
    else if( regex_search(s, m_match, m_links))
    {
        len = m_match.length(0)-1;
        Ref refs;
        refs.link = m_match.str(1);
        generate_link(refs, refs.link);
    }

    return len;
}

int MarkdownParser::find_new_line(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, m_force_line))
    {
        len = m_match.length(0)-1;
        new_line_event();
        if( m_match.length(1))
            insert_line();
    }

    return len;
}

bool MarkdownParser::reference_line(string& line)
{
    smatch match;

    if( regex_search(line, match, m_ref))
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
        toLower(name);
        m_refs[name] = referenz;

        return true;
    }

    return false;
}

int MarkdownParser::header_line(string& line)
{
    int mode = 0;
    smatch match;

    if( regex_search(line, match, m_header_rule1))
    {
        if( string(match[1])[0] == '=')
            mode = 1;
        else
            mode = 2;
    }
    else if( regex_search(line, match, m_header_rule2))
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
    return (regex_search(line, match, m_horizontal_rule1)
         || regex_search(line, match, m_horizontal_rule2));

}

void MarkdownParser::insert_level(int level)
{
    for( int i = 0; i < level; i++)
        insert("    ");
}

void MarkdownParser::insert_line()
{
    if( m_content.back() != '\n')
        insert("\n");
    else
    {
        m_content.pop_back();
        if( m_content.back() != '\n')
            insert("\n");
        m_content.push_back('\n');
    }
}

size_t MarkdownParser::get_tag_balance(string& tag, string& line, int& balance)
{
    size_t len = tag.length();
    size_t pos = 0;
    do{
        pos = line.find("<"+tag+">", pos);
        if( pos != string::npos)
        {
            ++balance;
            pos+=len+2;
        }
    }while( pos != string::npos);

    pos = 0;
    do{
        pos = line.find("</"+tag+">", pos);
        if( pos != string::npos)
        {
            --balance;
            if( balance == 0)
                return pos;
            pos+=len+3;
        }
    }while( pos != string::npos);

    return 0;
}

