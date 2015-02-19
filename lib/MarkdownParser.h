#ifndef MARKDOWN_PARSER_H_
#define MARKDOWN_PARSER_H_

#include "Parser.h"

#include <regex>

struct sRef
{
    string link;
    string title;
    list<string> paras;
    string name;
};

typedef struct sRef Ref;

class MarkdownParser : public Parser
{
    public:
        MarkdownParser();
        ~MarkdownParser();

        /** @brief (one liner)
          *
          * (documentation goes here)
          */
        void add_footer();

        /** @brief (one liner)
          *
          * (documentation goes here)
          */
        void add_header();

        /** @brief (one liner)
          *
          * (documentation goes here)
          */
        void print_headers(bool headers);

        /** @brief (one liner)
          *
          * (documentation goes here)
          */
        void parse();

        void define_blocks(list<string>& lines, int level = 0);
        void list_blocks(list<string>& lines, int level);

        bool isblock( string& block);
        bool isblockquote( string& block);
        bool isheadline( string& block);
        bool ishrule(string& block);
        bool islist(string& block);
        bool isunorderedlist(string& block);
        bool isorderedlist(string& block);
        bool ispreformated( string& block);

        void initial_manipulation();
        void blcokquote_manipulation(list<string>& lines);

        bool reference_line(string& line);
        int header_line( string& line);
        bool horizontal_rule_line(string &line);

        void insert_level(int level);
        void insert_line();

    protected:

        virtual void footer_event()=0;
        virtual void header_event()=0;

        virtual string header_event(string line, int level)=0;
        virtual string horizontal_rule_event()=0;

        virtual string paragraph_begin_event()=0;
        virtual string paragraph_end_event()=0;

        virtual string blockquote_begin_event()=0;
        virtual string blockquote_end_event()=0;

        virtual string list_item_begin_event()=0;
        virtual string list_item_end_event()=0;

        virtual string list_begin_event()=0;
        virtual string list_end_event()=0;

        virtual string ordered_list_begin_event()=0;
        virtual string ordered_list_end_event()=0;

    private:

        map<string,Ref> m_refs;
        smatch m_match;

        bool m_headers;
        bool m_ordered;
};

#endif // MARKDOWN_PARSER_H_

