#include "links.h"

#include <regex>
#include <iostream>

#include "functions.h"

/** \brief extract references from a markdown file
 *
 * \param a vector containing lines of markdonw code
 * \return a map containing all information about one link reference
 *
 */
map<string, Ref> extract_references(vector<string>& lines){

    map<string, Ref> links;
    smatch match;
    regex e("^\\[(.*)\\]:"); // regex for finding a reference source

    for( vector<string>::iterator it = lines.begin(); it != lines.end();){
        if( regex_search((*it), match,e)) // if the regex matces
        {

            // get the line, which matched and remove leading and trailing spaces
            string line = (*it).substr(match[0].length());
            line = trim(line);

            vector<string> parameters = parameter(line); // parse the parameters

            Ref dummy;

            dummy.link = parameters[0];

            // if there is more then one parameter given
            if( parameters.size() > 1){
                dummy.titel = parameters[1];
                for( vector<string>::iterator jt = parameters.begin()+2; jt != parameters.end(); jt++)
                    dummy.paras.push_back(*jt);
            }

            dummy.name = match[1];
            string name = dummy.name;
            toLower(name);
            links[name] = dummy;
            lines.erase(it);
        }
        else
            it++;
    }

    return links;
}
