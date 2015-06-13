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

    /** \brief abstract declaration of the footer_event function will be called after all parsing is done and headers is true
     *
     */
    void footer_event();

    /** \brief abstract declaration of the header_event function will be called before all parsing is done and headers is true
     *
     */
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

    /** \brief event call if a new paragraph block is started
     *
     */
    void paragraph_begin_event();

    /** \brief event call if a paragraph block is ended
     *
     */
    void paragraph_end_event();

    /** \brief event call if a blockquote block is started
     *
     */
    void blockquote_begin_event();

    /** \brief event call if a blockquote block is ended
     *
     */
    void blockquote_end_event();

    /** \brief event call if a new list item will be started
     *
     */
    void list_item_begin_event();

    /** \brief event call after a new list item was inserted
     *
     */
    void list_item_end_event();

    /** \brief event call if a new list should be insert
     *
     */
    void list_begin_event();

    /** \brief event call if a new list was insert
     *
     */
    void list_end_event();

    /** \brief event call if a new ordered list should be insert
     *
     */
    void ordered_list_begin_event();

    /** \brief event call if a ordered list was insert
     *
     */
    void ordered_list_end_event();

    /** \brief event call if a new preformated block will be insert
     *
     */
    void preformat_begin_event();

    /** \brief event call if a new preformated block was insert
     *
     */
    void preformat_end_event();

    /** \brief event call if a new code block should be insert
     *
     * \param the language of the code block
     */
    void code_begin_event(string lang = "");

    /** \brief event call after a code block was insert
     *
     */
    void code_end_event();

    /** \brief event call before a comment block will be insert
     *
     */
    void comment_begin_event();

    /** \brief event call after a comment block was insert
     *
     */
    void comment_end_event();

    /** \brief event call before a html tag will be insert
     *
     * \param html tag style to be insert
     * \param further parameters to insert into tag
     */
    void html_begin_event(string& tag, string& paras);

    /** \brief event call after a html tag was insert
     *
     * \param the tag to be closed
     */
    void html_end_event(string& tag);

    /** \brief event call before bold text will be insert
     *
     */
    void text_bold_begin_event();

    /** \brief event call after bold text was insert
     *
     */
    void text_bold_end_event();

    /** \brief event call before italic text will be insert
     *
     */
    void text_italic_begin_event();

    /** \brief event call after italic text was insert
     *
     */
    void text_italic_end_event();

    /** \brief event call to insert a new line
     *
     */
    void new_line_event();

    /** \brief event to replace and insert the given c to the buffer
     *
     * Some special chars has to be replaced like &lt; and &gt; for the code environment
     * others can be just insert
     *
     * \param the char to replace
     */
    void replace_code_char(const char& c);

    /** \brief event to replace and insert the given c to the buffer
     *
     * Some special chars has to be replaced like &lt; and &gt; for the code environment
     * others can be just insert
     *
     * \param the string to replace
     */
    void replace_code_char(string& line);

    /** \brief event to replace a given char and insert it into the buffer
     *
     * Some chars need to be replaced before inserting into the buffer
     *
     * \param the char to analyze and insert into the buffer
     */
    void replace_char(const char& c);

    /** \brief checks if the given char needed to be escape and will do so
     *
     * \param the char which was escaped
     * \return 0 if it was not escaped or 1 if it was
     */
    int  check_escaped_char( const char& c);

    /** \brief takes a string containing text and will return an hexcode representation of that text
     *
     * \param the text to hide
     * \return return the hidden text
     */
    string hide_chars(string line);


    /** \brief generate an link to an given item
     *
     * \param a reference struct containing link target and title if any
     * \param the name to display
     */
    void generate_link( Ref ref, string& name);

    /** \brief generate an link to an given item, without further char replacement
     *
     * \param a reference struct containing link target and title if any
     * \param the name to display
     */
    void generate_link_no_replace( Ref ref, string& name);

    /** \brief generate an link to an given image
     *
     * \param a reference struct containing link target and title if any
     * \param the alt text for the image
     */
    void generate_img( Ref src, string& alt);

  private:

    list<string> table_align;
    list<string>::iterator ta_it;
};

#endif // HTMLPARSER_H
