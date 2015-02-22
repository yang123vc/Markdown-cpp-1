#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <list>
#include <map>

using namespace std;

class Parser
{
    public:

        virtual void add_header() = 0;
        virtual void add_footer() = 0;

        virtual void parse() = 0;

        void set_lines( list<string>& lines);

        string get_content();

        list<string> extract_parameter( string& line);

        void insert( string content);
        void insert( const char& c);

    protected:

        list<string> m_lines;
        string m_content;

    private:
};

#endif // PARSER_H_
