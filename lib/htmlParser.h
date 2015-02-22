#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "MarkdownParser.h"


class HTMLParser : public MarkdownParser
{
    public:
        /** Default constructor */
        HTMLParser();
        /** Default destructor */
        virtual ~HTMLParser();

    protected:

        void footer_event();
        void header_event();

        string header_event(string line, int level);
        string horizontal_rule_event();

        void paragraph_begin_event();
        void paragraph_end_event();

        void blockquote_begin_event();
        void blockquote_end_event();

        void list_item_begin_event();
        void list_item_end_event();

        void list_begin_event();
        void list_end_event();

        void ordered_list_begin_event();
        void ordered_list_end_event();

        void preformat_begin_event();
        void preformat_end_event();

        void code_begin_event(string lang);
        void code_end_event();

        void replace_code_char(const char& c);
        void replace_char(const char& c);

        void generate_link( Ref ref, string& name);
        void generate_img( Ref src, string& alt);

    private:
};

#endif // HTMLPARSER_H
