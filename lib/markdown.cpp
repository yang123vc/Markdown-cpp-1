#include "markdown.h"

#include <vector>
#include <sstream>

#include <iostream>

#include "links.h"


/** \brief splits content in a vector line by line
 *
 * splits the given content in lines and stores this in a
 * vector which will be returned.
 *
 * \param content the file content as a single sting
 * \return a vector containing each line as an entry
 *
 */
vector<string> split_lines(string& content){

    vector<string> lines;

    istringstream split(content);
    for (string each; getline(split, each); lines.push_back(each));

    return lines;
}

string parse( string& content){
    return parse(content, new Parser());
}

/** \brief parse markdown content to an other format
 *
 * \param content to parse from html
 * \return the now parsed content
 *
 */
string parse( string& content, Parser* parser){

    vector<string> lines = split_lines(content);

    map<string, Ref> refs = extract_references(lines);

    parser->set_refs(&refs);

    parser->add_header();
    for( string line: lines){
        parser->new_line(line);
    }
    parser->flush();
    parser->add_footer();
    return parser->get_content();
}
