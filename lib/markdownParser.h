#ifndef MARKDOWN_PARSER_H_
#define MARKDOWN_PARSER_H_

#include "parser.h"

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

        void hrule_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void headline_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void code_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void preformated_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void blockquote_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void list_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void html_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void comment_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void paragraph_block(list<string>::iterator& line, int level, list<string>::iterator ende);
        void normal_block(list<string>::iterator& line, int level, bool& multiline, list<string>::iterator ende);

        bool isblock( string& line);
        bool isblockquote( string& line);
        bool isheadline( string& line);
        bool ishrule(string& line);
        bool ishtmlblock(string& line);
        bool ishtmlcomment(string& line);
        bool islist(string& line);
        bool isunorderedlist(string& line);
        bool isorderedlist(string& line);
        bool ispreformated( string& line);
        bool iscodeblock(string& line);

        void initial_manipulation();
        void blockquote_manipulation(list<string>& lines);

        void parse_line(string& s);
        void parse_code_line(string& s);

        int find_word(string& s);
        int find_bold(string& s);
        int find_italic(string& s);
        int find_unicode(string& s);
        int find_comment(string& s);
        int find_htmlTags(string& s);
        int find_open_htmlTags(string& s);
        int find_close_htmlTags(string& s);
        int find_inline_code(string& s);
        int find_references(string& s);
        int find_links(string& s);
        int find_new_line(string& s);

        bool reference_line(string& line);
        int header_line( string& line);
        bool horizontal_rule_line(string &line);

        void insert_level(int level);
        void insert_line();

        size_t get_tag_balance(string& tag, string& line, int& balance);

    protected:

        virtual void footer_event()=0;
        virtual void header_event()=0;

        virtual string header_event(string line, int level, string paras="")=0;
        virtual string horizontal_rule_event(string paras = "")=0;

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
        virtual void replace_code_char(string& line)=0;
        virtual void replace_char(const char& c)=0;
        virtual int  check_escaped_char( const char& c)=0;
        virtual string hide_chars(string line)=0;

        virtual void generate_link( Ref ref, string& name)=0;
        virtual void generate_link_no_replace( Ref ref, string& name)=0;
        virtual void generate_img( Ref src, string& alt)=0;

    private:

        map<string,Ref> m_refs;

        regex m_bold;
        regex m_italic;
        regex m_unicodeChar;
        regex m_comment;
        regex m_htmlTags;
        regex m_htmlOpenTag;
        regex m_htmlblock;
        regex m_htmlcomment;
        regex m_htmlCloseTag;
        regex m_htmlTag;
        regex m_inline_code;
        regex m_headline;
        regex m_header_rule1;
        regex m_header_rule2;
        regex m_horizontal_rule1;
        regex m_horizontal_rule2;
        regex m_hrule;
        regex m_anylist;
        regex m_ulist;
        regex m_olist;
        regex m_preformated;
        regex m_code;
        regex m_reference_db;
        regex m_reference_inline;
        regex m_links;
        regex m_force_line;
        regex m_ref;
        regex m_word;
        regex m_email;

        smatch m_match;

        bool m_headers;
        bool m_ordered;
};

#endif // MARKDOWN_PARSER_H_

