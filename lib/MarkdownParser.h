#ifndef MARKDOWN_PARSER_H_
#define MARKDOWN_PARSER_H_

#include "Parser.h"

#include <iostream>
#include <list>
#include <stdio.h>
#include <regex>

struct sRef
{
    string link;
    string title;
    list<string> paras;
    string name;
};

typedef struct sRef Ref;

using namespace std;

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

        void parse_line(string& s);
        void parse_code_line(string& s);

        int find_bold(string& s);
        int find_italic(string& s);
        int find_unicode(string& s);
        int find_comment(string& s);
        int find_htmlTags(string& s);
      //int find_open_htmlTags(string& s);
      //int find_close_htmlTags(string& s);
        int find_inline_code(string& s);
        int find_references(string& s);
        int find_links(string& s);
        int find_new_line(string& s);

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

        virtual void paragraph_begin_event()=0;
        virtual void paragraph_end_event()=0;

        virtual void blockquote_begin_event()=0;
        virtual void blockquote_end_event()=0;

        virtual void list_item_begin_event()=0;
        virtual void list_item_end_event()=0;

        virtual void list_begin_event()=0;
        virtual void list_end_event()=0;

        virtual void ordered_list_begin_event()=0;
        virtual void ordered_list_end_event()=0;

        virtual void preformat_begin_event()=0;
        virtual void preformat_end_event()=0;

        virtual void code_begin_event(string lang = "")=0;
        virtual void code_end_event()=0;

        virtual void comment_begin_event()=0;
        virtual void comment_end_event()=0;

        virtual void html_begin_event(string& tag, string& paras)=0;
        virtual void html_end_event(string& tag)=0;

        virtual void text_bold_begin_event()=0;
        virtual void text_bold_end_event()=0;

        virtual void text_italic_begin_event()=0;
        virtual void text_italic_end_event()=0;

        virtual void new_line_event()=0;

        virtual void replace_code_char(const char& c)=0;
        virtual void replace_char(const char& c)=0;
        virtual int  check_escaped_char( const char& c)=0;

        virtual void generate_link( Ref ref, string& name)=0;
        virtual void generate_img( Ref src, string& alt)=0;

    private:

        map<string,Ref> m_refs;

        regex bold;
        regex italic;
        regex unicodeChar;
        regex comment;
        regex htmlTags;
        regex inline_code;
        regex headline;
        regex header_rule1;
        regex header_rule2;
        regex horizontal_rule1;
        regex horizontal_rule2;
        regex hrule;
        regex anylist;
        regex ulist;
        regex olist;
        regex preformated;
        regex code;
        regex reference_db;
        regex reference_inline;
        regex links;
        regex force_line;
        regex refs;


        smatch m_match;

        bool m_headers;
        bool m_ordered;
};

#endif // MARKDOWN_PARSER_H_

