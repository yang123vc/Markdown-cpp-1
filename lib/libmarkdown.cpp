#include "libmarkdown.h"

#include <sstream>

/** \brief splits content in a list line by line
 *
 * splits the given content in lines and stores this in a
 * list which will be returned.
 *
 * \param content the file content as a single sting
 * \return a list containing each line as an entry
 *
 */
list<string> split_lines(string& content){

    list<string> lines;

    istringstream split(content);
    for (string each; getline(split, each); )
    {
        // remove line endings if there are some
        size_t line_break = each.rfind('\r');
        if( line_break != string::npos)
        {
            each = each.substr(0,each.length()-1);
        }
        line_break = each.rfind('\n');
        if( line_break != string::npos)
        {
            each = each.substr(0,each.length()-1);
        }
        lines.push_back(each);
    }

    return lines;
}

string parse(string& content, Parser* parser)
{
    list<string> lines = split_lines(content);

    parser->add_header();
    parser->set_lines(lines);

    parser->parse();

    parser->add_footer();

    return parser->get_content();
}
