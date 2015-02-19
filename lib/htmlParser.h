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

        string paragraph_begin_event();
        string paragraph_end_event();

        string blockquote_begin_event();
        string blockquote_end_event();

        string list_item_begin_event();
        string list_item_end_event();

        string list_begin_event();
        string list_end_event();

        string ordered_list_begin_event();
        string ordered_list_end_event();


    private:
};

#endif // HTMLPARSER_H
