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

        inline bool isblock( string& block);
        inline bool isblockquote( string& block);
        inline bool isheadline( string& block);
        inline bool ishrule(string& block);
        inline bool islist(string& block);
        inline bool isunorderedlist(string& block);
        inline bool isorderedlist(string& block);
        inline bool ispreformated( string& block);
        inline bool iscodeblock(string& block);

        void initial_manipulation();
        void blcokquote_manipulation(list<string>& lines);

        inline bool reference_line(string& line);
        inline int header_line( string& line);
        inline bool horizontal_rule_line(string &line);

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

        virtual string preformat_begin_event()=0;
        virtual string preformat_end_event()=0;

        virtual string code_begin_event(string lang = "")=0;
        virtual string code_end_event()=0;

    private:

        map<string,Ref> m_refs;
        smatch m_match;

        bool m_headers;
        bool m_ordered;
};

#endif // MARKDOWN_PARSER_H_

