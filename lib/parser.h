#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <list>
#include <map>

using namespace std;

/** \brief Abstract class for all parser. Containing the content to be parsed and the parsed content
 * \author Jonas Zinn
 * \date Feb. 2015
 */
class Parser
{
    public:

        /** \brief invoke an event to enable to add an specific header to the output
         *
         */

        virtual void add_header() = 0;

        /** \brief invoke an event to enable to add an specific footer to the output
         */
        virtual void add_footer() = 0;

        /** \brief call the parse procedure
         *
         */
        virtual void parse() = 0;

        /** \brief set the content which will be parsed
         *
         * \param lines the lines which are parsed
         */
        void set_lines( list<string>& lines);

        /** \brief returns the parser output
         *
         * \return the parsed output in a single string
         */
        string get_content();

        /** \brief extracts parameters from a given text line
         *
         * \param a line which will be split into parameter
         * \return a list of strings each is one parameter
         */
        list<string> extract_parameter( string& line);


        /** \brief insert a complete string into the buffer of the parsed chars
         *
         * \param a string to insert into the buffer
         */
        void insert( string content);

        /** \brief insert a single char into the buffer of the parsed chars
         *
         * \param a single char to insert into the buffer
         */
        void insert( const char& c);

    protected:

        list<string> m_lines; //< the lines to be parsed
        string m_content; //< the parsed content

};

#endif // PARSER_H_
