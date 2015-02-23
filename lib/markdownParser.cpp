#include "markdownParser.h"

#include "functions.h"

MarkdownParser::MarkdownParser()
{
    m_ordered = false;
    print_headers(true);
    bold = regex("^((?:\\*|_){2})((?!\\s|\\1).*?[^\\s\\\\](?:\\*|_)?)\\1");
    italic = regex("^(\\*|_)((?!\\s|\\1).*?[^\\s\\\\])\\1");
    unicodeChar = regex("^&([a-zA-Z]+|#[0-9]+);");
    comment = regex("^<!--(.*?)-->");
    htmlTags = regex("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>([^\n\r]*?)</\\1>");
    inline_code = regex("^([`]{1,2})(.+?)\\1");
    headline = regex("^([ ]{0,3})<h([1-6])>([^<]*)</h[1-6]>");
    header_rule1 = regex("^(=+|-+)[ \\t\\f]*$");
    header_rule2 = regex("^(#{1,})([^#]*?)(#*)$");
    horizontal_rule1 = regex("^[ ]{0,3}\\*([ ]{0,2}\\*){2,}[ \\t\\f]*$");
    horizontal_rule2 = regex("^[ ]{0,3}(-|_)([ ]{0,2}\\1){2,}[ \\t\\f]*$");
    hrule = regex("^[ ]{0,3}<[ ]?hr[ ]?/?>");
    anylist = regex("^(\\*|\\+|-|[0-9]+\\.)[ ]+");
    ulist = regex("^(\\*|\\+|-) ");
    olist = regex("^[0-9]+\\. ");
    preformated = regex("^([ ]{4}|\\t)");
    code = regex("^```(.*)");
    reference_db = regex("^(!?)\\[([^\\]]+)\\] ?(\\[([a-zA-Z0-9]+)\\])?");
    reference_inline = regex("^(!?)\\[([^\\]]+)\\] ?\\(<?((?:http:/|ftp:/|file:/)?/[^\\)]+?)>?\\)");
    links = regex("^<((?:(?:http|ftp|file)://)[^\\s]+)>");
    force_line = regex("^[ ]{2,}($|\n)");
    refs = regex("^\\s*\\[([^\\]]+)\\]:\\s*(http://|ftp://|/|file://)");// finding references in source
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
                insert_level(level);
                insert(horizontal_rule_event());
                insert_line();
            }
            else if( isheadline(*line)) // header block
            {
                insert_level(level);
                insert(header_event(string(m_match[3]),string(m_match[2])[0]-'0'));
                insert_line();
            }
            else if( iscodeblock(*line)) // code block
            {
                insert_level(level);
                preformat_begin_event();
                code_begin_event(m_match[1]);

                do
                {
                    ++line;
                    if( line == lines.end() || iscodeblock(*line))
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
            else if( ispreformated(*line)) // code block
            {
                insert_level(level);
                preformat_begin_event();
                code_begin_event();
                empty_line = false;
                do
                {
                    if( empty_line)
                        insert_line();
                    if( ispreformated(*line))
                    {
                        string lien = (*line).substr(m_match.length(1));
                        for( size_t j = 0; j < lien.length();++j)
                        {
                            replace_code_char(lien[j]);
                        }
                    }
                    insert_line();
                    empty_line = false;
                    ++line;
                    if( line == lines.end())
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

                }while(true);
                code_end_event();
                preformat_end_event();
                insert_line();
            }
            else if( isblockquote((*line))) // blockquote
            {
                //cout << blockquote_begin_event() << endl;
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
                blockquote_end_event();
                insert_line();
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                bool ordered = m_ordered;
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
                        //cout << list_item_begin_event() << endl;
                        insert_level(level+1);
                        list_item_begin_event();
                        insert_line();
                        list<string> content;
                        content.push_back((*line).substr(m_match.length(0)));
                        do
                        {
                            empty_line = false;
                            ++line;
                            if( line == lines.end())
                                break;
                            else if( (*line).empty())
                            {
                                empty_line = true;
                                ++line;
                            }
                            if( line == lines.end() || isheadline(*line)
                             || isblockquote(*line))
                            {
                                if( empty_line)
                                    --line;
                                break;
                            }
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
                            else if( iscodeblock(*line))
                            {
                                if( empty_line)
                                {
                                    --line;
                                    break;
                                }
                                content.push_back(*line);
                            }
                            else
                                content.push_back(*line);
                        }while(true);

                        list_blocks(content, level+2);

                        insert_level(level+1);
                        list_item_end_event();
                        insert_line();
                    }
                    if( line == lines.end())
                        break;
                    else if((*line).empty())
                    {
                        ++line;
                        if( line == lines.end() || isblockquote(*line) || isblockquote(*line))
                        {
                            --line;
                            --line;
                            break;
                        }
                    }
                    else if( isheadline(*line)  || isblockquote(*line))
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
            else
                found = false;
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
            string content;
            insert_level(level);
            paragraph_begin_event();
            string cont = "";
            do
            {
                cont += (*line);
                ++line;
                if( (line == lines.end()) || isheadline(*line) ||(*line).empty())
                {
                    --line;
                    break;
                }
                else
                {
                    content += cont;
                    cont = "\n";
                    for( int i = 0; i < level; i++)
                        cont += "\t";
                }
            }while(true);

            content += cont;

            parse_line(content);
            paragraph_end_event();
            insert_line();
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
                insert_level(level);
                insert(horizontal_rule_event());
                insert_line();
            }
            else if( isunorderedlist(*line) || isorderedlist(*line)) // list block
            {
                multiline = false;
                bool ordered = m_ordered;
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
                            if( line == lines.end())
                                break;
                            else if( (*line).empty())
                            {
                                empty_line = true;
                                ++line;
                            }
                            if( line == lines.end() || isheadline(*line)
                             || isblockquote(*line))
                            {
                                if( empty_line)
                                    --line;
                                break;
                            }
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
                            else if( iscodeblock(*line))
                            {
                                if( empty_line)
                                {
                                    --line;
                                    break;
                                }
                                content.push_back(*line);
                            }
                            else
                                content.push_back(*line);
                        }while(true);

                        list_blocks(content, level+2);

                        insert_level(level+1);
                        list_item_end_event();
                        insert_line();
                    }

                    if( line == lines.end())
                        break;
                    else if((*line).empty())
                    {
                        ++line;
                        if( line == lines.end() || isblockquote(*line) || iscodeblock(*line))
                        {
                            --line;
                            --line;
                            break;
                        }
                    }
                    else if( isheadline(*line) || isblockquote(*line))
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
            else
            {
                found = false;
            }
            empty_line = false;
        }
        else if((*line).empty())
        {
            if( !empty_line )
            {

                empty_line = true;
                //cout << endl;
                if( next(line) != lines.end())
                    insert_line();
            }
        }

        if( !found) // Normal paragraph
        {
            found = true;
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

        if( line != lines.end())
        {
            ++line;
        }
    }
}

bool MarkdownParser::isblock(string& block)
{
    return (isblockquote(block) || ishrule(block)
         || islist(block) || ispreformated(block) || iscodeblock(block));
}

bool MarkdownParser::isblockquote(string& block)
{
    if( !block.empty() && block.length() > 2 && block[0] == '>' )
        return true;
    return false;
}

bool MarkdownParser::isheadline(string& block)
{
    return regex_search(block, m_match, headline);
}

bool MarkdownParser::ishrule(string& block)
{
    return regex_search(block, m_match, hrule);
}

bool MarkdownParser::islist(string& block)
{
    return regex_search(block,m_match, anylist);
}

bool MarkdownParser::isunorderedlist(string& block)
{
    return regex_search(block,m_match, ulist);
}

bool MarkdownParser::isorderedlist(string& block)
{
    m_ordered = regex_search(block,m_match, olist);
    return m_ordered;
}

bool MarkdownParser::ispreformated(string& block)
{
    return regex_search(block, m_match, preformated);
}

bool MarkdownParser::iscodeblock(string& block)
{
    return regex_search(block, m_match, code);
}

void MarkdownParser::initial_manipulation()
{
    list<string> temp_lines;

    for( string line : m_lines)
    {
        if( !reference_line(line))
        {
            //cout << line << endl;
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

void MarkdownParser::parse_line(string& s)
{
    for( size_t i = 0; i < s.length(); ++i)
    {
        size_t pos = 0;
        string temp = s.substr(i);
        if( (pos = find_bold(temp)))
        {
            i+= pos;
        }
        else if( (pos = find_italic(temp)))
        {
            i += pos;
        }
        else if( (pos = find_comment(temp)))
        {
            //cout << "found comment" << endl;
            i += pos;
        }
        else if( (pos = find_links(temp)))
        {
            //cout << "found link" << endl;
            i += pos;
        }
        else if( (pos = find_references(temp)))
        {
         //   cout << "found references" << endl;
            i += pos;
        }
        else if( (pos = find_htmlTags(temp)))
        {
         //   cout << "found htmltags" << endl;
            i += pos;
        }
        else if( (pos = find_inline_code(temp)))
        {
            //cout << "found inline code" << endl;
            i += pos;
        }
        else if( (pos = find_new_line(temp)))
        {
            i += pos;
        }
        else if( (pos = find_unicode(temp)))
        {
         //   cout << "found spezial char" << endl;
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

int MarkdownParser::find_bold(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, bold))
    {
        len = m_match.length(0)-1;
        text_bold_begin_event();
        string strong_text = m_match[2];
        for( size_t j = 0; j < strong_text.length(); ++j)
        {
            string temp = strong_text.substr(j);
            size_t pos;
            if( (pos = find_italic(temp)))
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

    if( regex_search(s, m_match, italic))
    {
        len = m_match.length(0)-1;
        text_italic_begin_event();
        string italic_text = m_match[2];
        for( size_t j = 0; j < italic_text.length(); ++j)
        {
            string temp = italic_text.substr(j);
            size_t pos;
            if( (pos = find_unicode(temp)))
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

    if( regex_search(s, m_match, unicodeChar))
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

    if( regex_search(s, m_match, comment))
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

   if( regex_search(s, m_match, htmlTags))
    {
        len = m_match.length(0)-1;
        //cout << m_match[1] << endl;
        string tag = m_match[1];
        string paras = m_match[2];
        string text = m_match[3];
        html_begin_event(tag, paras);
        for( size_t j = 0; j < text.length(); ++j)
        {
            size_t pos;
            string temp = text.substr(j);
            if( (pos = find_htmlTags(temp)))
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
        html_end_event(tag);
    }

    return len;
}

int MarkdownParser::find_inline_code(string& s)
{
    size_t len = 0;

    if( regex_search(s, m_match, inline_code))
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

    if( regex_search(s, m_match, reference_inline))
    {
        len = m_match.length(0)-1;
        bool image = m_match.length(1) > 0;
        Ref dummy;
        dummy.name = m_match.str(2);
        dummy.link = m_match.str(3);

        if( image)
            generate_img(dummy, dummy.name);
        else
            generate_link(dummy, dummy.name);
    }
    else if( regex_search(s, m_match, reference_db))
    {
        len = m_match.length(0)-1;
        bool image = m_match.length(1) > 0;

        if( m_match.length(3) != 0)
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

    if( regex_search(s, m_match, links))
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

    if( regex_search(s, m_match, force_line))
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
