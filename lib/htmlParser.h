#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "markdownParser.h"


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

        string header_event(string line, int level, string paras);
        string horizontal_rule_event(string paras);

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

        void comment_begin_event();
        void comment_end_event();

        void html_begin_event(string& tag, string& paras);
        void html_end_event(string& tag);

        void text_bold_begin_event();
        void text_bold_end_event();

        void text_italic_begin_event();
        void text_italic_end_event();

        void new_line_event();

        void replace_code_char(const char& c);
        void replace_code_char(string& line);
        void replace_char(const char& c);
        int  check_escaped_char(const char& c);
        string hide_chars(string line);

        void generate_link( Ref ref, string& name);
        void generate_link_no_replace(Ref ref, string& name);
        void generate_img( Ref src, string& alt);

    private:
};

#endif // HTMLPARSER_H
