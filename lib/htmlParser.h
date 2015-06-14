#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "markdownParser.h"

/** \brief markdown to html parser
 *
 * \author Jonas Zinn
 * \date Feb. 2015
 */
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

    /** \brief event called if a header <hX> element should be insert
     *
     * \param the text to be insert as header
     * \param the header number 1-8
     * \param additional parameters like class and id
     * \return the generated string to be insert into the buffer
     */
    string header_event(string line, int level, string paras="");

    /** \brief event call if an horizontal line should be insert into the buffer
     *
     * \param parameters like id and class
     * \return the generated output to put in the buffer
     */
    string horizontal_rule_event(string paras = "");

    void table_begin_event(list<Align> table_header);
    void table_end_event();

    void table_begin_row();
    void table_end_row();

    void table_begin_col();
    void table_end_col();

    void table_begin_header();
    void table_end_header();

    void table_begin_multi_col(int cols);
    void table_end_multi_col();

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

    void code_begin_event(string lang = "");

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

    int  check_escaped_char( const char& c);

    string hide_chars(string line);

    void generate_link( Ref ref, string& name);

    void generate_link_no_replace( Ref ref, string& name);

    void generate_img( Ref src, string& alt);

    void generate_link_img( Ref src, string& alt);

    void footnote_event(string& footnote);

    void footnote_end_event();

  private:

    list<string> table_align;
    list<string>::iterator ta_it;
    list<string> m_footer_content;

};

#endif // HTMLPARSER_H
