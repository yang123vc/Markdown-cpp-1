#ifndef HTML_PARSER_H_
#define HTML_PARSER_H_

#include "parser.h"

class HTMLParser : public Parser{

    public:
        HTMLParser(bool header = false);
        virtual ~HTMLParser();

        void add_header();
        void add_footer();

        //void new_line(string line);
        void edit_block(string block);

        void empty_line_event();
        int spezial_char_event(const char* c);
        int reference_event(const char* c);

        bool isspezial(char c);
        bool isreference(char c);

        void insert_link(Ref item, string name = "");

    private:
        string block;
        bool header;
};

#endif // HTML_PARSER_H_
