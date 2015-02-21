#include <iostream>
#include <regex>
#include <stdio.h>

using namespace std;

static inline string& tolower(string& s)
{
    for( string::iterator it = s.begin(); it != s.end(); it++)
        *it = tolower(*it);
    return s;
}

regex fett("^((?:\\*|_){2})((?!\\s|\\1).*?[^\\s\\\\](?:\\*|_)?)\\1");
regex italic("^(\\*|_)((?!\\s|\\1).*?[^\\s\\\\])\\1");

regex comment("^<!--(.*?)-->");
regex htmlTags("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>([^\n\r]*?)</\\1>");
regex htmlOpenTags("^<([a-zA-Z][a-zA-Z0-9]*)\\b([^>]*)>");
regex htmlCloseTags("^</([a-zA-Z][a-zA-Z0-9]*[^>]*)>");
regex unicodeChar("^&([a-zA-Z]+|#[0-9]+);");
regex reference_db("^(!?)\\[([a-zA-Z0-9]+)\\] ?(\\[([a-zA-Z0-9]+)\\])?");
regex reference_inline("^(!?)\\[([a-zA-Z0-9][^\\]]*?)\\] ?\\(<?((?:http:/|ftp:/|file:/)?/[^\\)]+?)>?\\)");
regex links("^<((?:(?:http|ftp|file)://)[^\\s]+)>");
regex force_line("[ ]{2,}($|\n)");

regex inline_code("^(``|`)(.+?)\\1");

smatch match;

string result;
string return_string;

inline int find_fett( string& s);
inline int find_italic( string& s);
inline int find_unicode( string& s);
inline int find_comment( string& s);
inline int find_htmlTags( string& s);
inline int find_inline_code( string& s);
inline int find_reference(string& s);
inline int find_links(string& s);
inline int find_new_line(string& s);

inline void replace_code_char(const char c, char* buffer);
inline void replace_char( const char c, char* buffer);

inline string generate_link( string& href, string& name);
inline string generate_img( string& src, string& alt);

void parse_string(string s);


int main()
{

    //string s = "**inte &amp; *ressant***\n<!-- schade -->\n<font color=#33333>Schrifft</font>"
    //"\n`<code>`";

    string s = "[link](/script?foo=1&bar=2).  \n\n[link](</script?foo=1&bar=2).";

    smatch match;

    size_t pos = 0;
    size_t length = s.length();
    for( size_t i = 0; i < length; ++i)
    {
        cout << "{" << s[i] << "}" << endl;
        string temp = s.substr(i);
        if( (pos = find_fett(temp))>0)
        {
            cout << return_string << endl;
            i += pos;
            result += return_string;
        }
        else if( (pos = find_italic(temp)) > 0)
        {
            cout << return_string << endl;
            i += pos;
            result += return_string;
        }
        else if( (pos = find_comment(temp))>0)
        {
            cout << return_string << endl;
            i+= pos;
            result += return_string;
        }
        else if( (pos = find_links(temp)) > 0)
        {
            cout << return_string << endl;
            i+= pos;
            result += return_string;
        }
        else if( (pos = find_reference(temp)) > 0)
        {
            cout << return_string << endl;
            i+= pos;
            result += return_string;
        }
        else if( (pos = find_htmlTags(temp)) > 0)
        {
            cout << return_string << endl;
            i += pos;
            result += return_string;
        }
        else if( (pos = find_inline_code(temp)) > 0)
        {
            cout << return_string << endl;
            i += pos;
            result += return_string;
        }
        else if( (pos = find_new_line(temp)) > 0)
        {
            cout << return_string << endl;
            i += pos;
            result += return_string;
        }
        else
        {
            char buffer[2];
            replace_char(temp[0], buffer);
            result += buffer;
        }
        /*if( regex_search(s, match, fett))
        {

            cout << match[0] << endl;
            pos = match.position();
            len = match.length();
            if( pos == 0 || isspace(s[pos-1]))
            {
                s.replace(pos+len-2, 2, "</strong>");
                s.replace(pos, 2, "<strong>");
                cout << s << endl;
            }
            else
                cout << "not found" << endl;
        }

        if( regex_search(s, match, italic))
        {

            cout << match[0] << endl;
            size_t pos = match.position();
            size_t len = match.length();
            if( pos == 0 || s[pos-1]!='\\')
            {
                s.replace(pos+len-1, 1, "</em>");
                s.replace(pos, 1, "<em>");
                cout << s << endl;
            }
            else
                cout << "not found" << endl;
        }*/
    }

    cout << s << endl;
    cout << "=>" << endl;
    cout << result << endl;

    return 0;
}

int find_fett(string& s)
{
    return_string = "";
    string temp_return_string = "";
    if( regex_search(s, match, fett))
    {
        size_t len = match.length(0)-1;
        temp_return_string = "<strong>";
        string strong_text = match[2];
        for( size_t j = 0; j < strong_text.length(); j++)
        {
            string temp = strong_text.substr(j);
            size_t pos;
            if( (pos = find_italic(temp)) > 0)
            {
                temp_return_string += return_string;
                j+= pos;
            }
            else if ( (pos = find_unicode(temp)) > 0)
            {
                temp_return_string += return_string;
                j+= pos;
            }
            else
                temp_return_string += temp[0];
        }
        temp_return_string += "</strong>";
        return_string = temp_return_string;
        cout << match[0] << ":" << match.length(0) << endl;
        return len;
    }
    return 0;
}

int find_italic(string& s)
{
    return_string = "";
    string temp_return_string = "";
    if( regex_search(s, match, italic))
    {
        temp_return_string = "<em>";
        temp_return_string += match[2];
        temp_return_string += "</em>";
        return_string = temp_return_string;
        return match.length(0)+1;
    }
    return 0;
}

int find_unicode(string& s)
{
    return_string = "";
    if( regex_search(s, match, unicodeChar))
    {
        cout << "found " << match[0] << endl;
        return_string = match[0].str();
        return match.length(0)-1;
    }
    return 0;
}

int find_comment(string& s)
{
    return_string = "";
    if( regex_search(s, match, comment))
    {
        cout << "find comment: " << match[0] << endl;

        return_string = match[0].str();

        return match.length(0)-1;
    }

    return 0;
}

int find_htmlTags(string& s)
{
    return_string = "";
    if( regex_search(s, match, htmlTags))
    {
        cout << "found html: " << match[0] << endl;

        cout << "class " << match[1] << endl;
        cout << "paras " << match[2] << endl;
        cout << "text " << match[3] << endl;

        return_string = "<" + match[1].str() + match[2].str() + ">"+match[3].str() + "</" + match[1].str() + ">";

        return match.length(0)-1;
    }

    return 0;
}

int find_inline_code(string& s)
{
    return_string = "";
    if( regex_search(s, match, inline_code))
    {
        size_t len = match.length(0)-1;
        cout << "code found: " << match[2] << endl;
        string code_string = match[2].str();
        char buffer[20];
        for( size_t j = 0; j < code_string.length(); j++)
        {
            replace_code_char(code_string[j], buffer);
            cout << buffer << endl;
            return_string += buffer;
        }
        return len;
    }
    return 0;
}

inline int find_reference(string& s)
{
    return_string = "";
    char buffer[20];
    size_t pos = 0;
    if( regex_search(s, match, reference_inline))
    {
        cout << "inline ref found: " << match[1] << endl;
        bool image = match.length(1) > 0;
        pos = match.length(0)-1;
        string name = match.str(2);
        string link = match.str(3);

        string temp_name = "";
        string temp_link = "";
        for( size_t j = 0; j < name.length(); ++j)
        {
            replace_char(name[j], buffer);
            temp_name += buffer;
        }
        for( size_t j = 0; j < link.length(); ++j)
        {
            replace_char(link[j], buffer);
            temp_link += buffer;
        }
        if( image)
            return_string = generate_img(temp_link, temp_name);
        else
            return_string = generate_link(temp_link, temp_name);
    }
    else if( regex_search(s, match, reference_db))
    {
        pos = match.length(0)-1;
        bool image = match.length(1) > 0;

        cout << "ref DB found: " << match[2] << endl;
        if( match.length(3) != 0)
        {
            string name = match.str(2);
            string temp_ref = match.str(3).substr(1,match.length(3)-2);
            string link = "http://das_ist_ein_test.de";
            if( link.empty())
            {
                string dummy = match.str(0);
                string temp_link = "";
                for( size_t j = 0; j < dummy.length(); ++j)
                {
                    replace_char(dummy[j], buffer);
                    temp_link += buffer;
                }
                return_string = temp_link;
            }
            else
            {
                string temp_link = "";
                string temp_name = "";
                for( size_t j = 0; j < name.length(); ++j)
                {
                    replace_char(name[j], buffer);
                    temp_name += buffer;
                }
                for( size_t j = 0; j < link.length(); ++j)
                {
                    replace_char(link[j], buffer);
                    temp_link += buffer;
                }
                if( image)
                    return_string = generate_img(temp_link, temp_name);
                else
                    return_string = generate_link(temp_link, temp_name);
            }
        }
        else
        {
            string name = match.str(2);
            string link = "";//get_reference;
            if( link.empty())
            {
                for( size_t j = 0; j < name.length(); ++j)
                {
                    replace_char(name[j], buffer);
                    link += buffer;
                }
                return_string = link;
            }
            else
            {
                string temp_name = "";
                string temp_link = "";
                for( size_t j = 0; j < name.length(); ++j)
                {
                    replace_char(name[j], buffer);
                    temp_name += buffer;
                }
                for( size_t j = 0; j < link.length(); ++j)
                {
                    replace_char(link[j], buffer);
                    temp_link += buffer;
                }
                if( image)
                    return_string = generate_img(temp_link, temp_name);
                else
                    return_string = generate_link(temp_link, temp_name);
            }
        }
    }
    return pos;
}

inline int find_links(string& s)
{
    return_string = "";
    if( regex_search(s, match, links))
    {
        size_t pos = match.length(0)-1;

        cout << "links found: " << match[1] << endl;

        string link_string = match[1].str();
        char buffer[20];
        string temp_link = "";
        for( size_t j = 0; j < link_string.length(); j++)
        {
            replace_char(link_string[j], buffer);
            temp_link += buffer;
        }
        return_string = generate_link(temp_link, temp_link);
        return pos;
    }
    return 0;
}

inline int find_new_line(string& s)
{
    return_string = "";
    if( regex_search(s, match, force_line))
    {
        size_t pos = match.length(0)-1;

        return_string = "<br />";
        return pos;
    }
    return 0;
}

int find_escape_char( string& s)
{

    return 0;
}


void replace_code_char(const char c, char* buffer)
{
    switch( c)
    {
        case '<':
            sprintf(buffer, "&lt;");
        break;
        case '>':
            sprintf(buffer, "&gt;");
        break;
        case '&':
            sprintf(buffer, "&amp;");
        break;
        default:
            sprintf(buffer, "%c", c);
        break;
    }
}

void replace_char( const char c, char* buffer)
{
    switch( c)
    {
        case '&':
            sprintf(buffer, "&amp;");
        break;

        case '<':
            sprintf(buffer, "&lt;");
        break;

        default:
            sprintf(buffer, "%c", c);
        break;
    }
}

string generate_link( string& href, string& name)
{
    return "<a href='" + href + "'>" + name +"</a>";
}

string generate_img( string& src, string& alt)
{
    return "<img src='" + src + "' alt='"+alt+"' />";
}
