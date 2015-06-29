#ifndef MARKDOWN_PARSER_H_
#define MARKDOWN_PARSER_H_

#include "parser.h"

#include <iostream>
#include <list>
#include <stdio.h>
#include <regex>

/** \brief a struct holding a reference
 *
 * \author Jonas Zinn
 * \date Feb. 2015
 */
struct sRef
{
  string link; /// containing the link to the content
  string title; /// the title if any
  list<string> paras; /// further given parameters
  string name; /// the name of the reference
};
typedef struct sRef Ref;

enum Align
{
  DEFAULT,
  LEFT,
  RIGHT,
  CENTER
};

inline Align operator|(Align a, Align b)
{
  return static_cast<Align>(static_cast<int>(a) | static_cast<int>(b));
}

using namespace std;

/** \brief A parser for the markdown language
 * \author Jonas Zinn
 * \date Feb. 2015
 */
class MarkdownParser : public Parser
{
  public:
    MarkdownParser();
    ~MarkdownParser();

    /** \brief set the default level of the ground blocks
     *
     * \param the level to be the default level
     */
    void set_ground_level( int level);

    /** @brief implementation of the abstract function to add an header
      *
      * invokes the abstract \c header_event function
      */
    void add_footer();

    /** @brief implementation of the abstract function to add an footer
      *
      * invokes the abstract \c footer_event function
      *
      */
    void add_header();

    /** @brief enable or disable the header/footer event call
      *
      * sets if the \c footer_event and \c header_event function should be called
      * they will be called by default.
      */
    void print_headers(bool headers);

    /** @brief starts the parsing of the content
      *
      * main function of each parser. Parses the content and writes the parsed stuff to m_content
      */
    void parse();

    /** \brief takes lines and defines the corresponding blocks
     *
     * takes the lines and groups them to blocks
     * for example: list block, paragraph, blockquote etc.
     *
     * \param the lines which will be parsed
     * \param the indentation level, default value is 0
     */
    void define_blocks(list<string>& lines, int pLevel = -1);

    /** \brief takes lines of a list block
     *
     * takes the content of a list block and parses them for deeper levels
     *
     * \param the lines which will be parsed
     * \param the indentation level
     */
    void list_blocks(list<string>& lines, int level);

    /** \brief block containing an hr element
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void hrule_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block containing an <hX> text </hX> element
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void headline_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block in \code{```} fences
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void code_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block indented by 4 spaces or one tabulator
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void preformated_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block in > fences
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void blockquote_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block containing a list
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void list_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block containing an html element
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void html_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block containing a comment block
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void comment_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief block containing a paragraph block
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void paragraph_block(list<string>::iterator& line, int level, list<string>::iterator ende);


    /** @brief block containing a table
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void table_block(list<string>::iterator& line, int level, list<string>::iterator ende);

    /** \brief containing a single line or a paragraph block
     *
     * \param an iterator to the current line
     * \param the level of indentation
     * \param an iterator to the end of the lines, just for break the line graping
     */
    void normal_block(list<string>::iterator& line, int level, bool& multiline, list<string>::iterator ende);

    /** \brief checks if the given line belongs to one of the specified blocks is
     *
     * checks if the given line belongs to a
     *  blockquote block or hrule block or preformated block or code block
     *
     * \param the line to check which block this line belong to
     * \return true if the next line is a blockquote or hrule or list or preformated or code block line
     */
    bool isblock( string& line);

    /** \brief checks if a given line belongs to a blockquote block
     *
     * \param the line to check if this line is a blockquote block line
     * \return true if the given line is a blockquote block line
     */
    bool isblockquote( string& line);

    /** \brief checks if a given line belongs to a headline block
     *
     * \param the line to check if this line is a headline block line
     * \return true if the given line is a headline block line
     */
    bool isheadline( string& line);

    /** \brief checks if a given line belongs to a hrule block
     *
     * \param the line to check if this line is a hrule block line
     * \return true if the given line is a hrule block line
     */
    bool ishrule(string& line);

    /** \brief checks if a given line belongs to a htmlblock block
     *
     * \param the line to check if this line is a htmlblock block line
     * \return true if the given line is a htmlblock block line
     */
    bool ishtmlblock(string& line);

    /** \brief checks if a given line belongs to a comment block
     *
     * \param the line to check if this line is a comment block line
     * \return true if the given line is a comment block line
     */
    bool ishtmlcomment(string& line);

    /** \brief checks if a given line belongs to a list block
     *
     * \param the line to check if this line is a list block line
     * \return true if the given line is a list block line
     */
    bool islist(string& line);

    /** \brief checks if a given line belongs to a unorderedlist block
     *
     * \param the line to check if this line is a unorderedlist block line
     * \return true if the given line is a unorderedlist block line
     */
    bool isunorderedlist(string& line);

    /** \brief checks if a given line belongs to a orderedlist block
     *
     * \param the line to check if this line is a orderedlist block line
     * \return true if the given line is a orderedlist block line
     */
    bool isorderedlist(string& line);

    /** \brief checks if a given line belongs to a preformated block
     *
     * \param the line to check if this line is a preformated block line
     * \return true if the given line is a preformated block line
     */
    bool ispreformated( string& line);

    /** \brief checks if a given line belongs to a code block
     *
     * \param the line to check if this line is a code block line
     * \return true if the given line is a code block line
     */
    bool iscodeblock(string& line);

    /** \brief checks if a given line defines a code block end
     *
     * \param the line to check if this line is a code block line
     * \return true if the given line is a code block line
     */
    bool iscodeblock_end(string& line);

    /** \brief test if the given line may contain a table
     *
     * \param the line to check if this ls is a table line
     * \return bool true if the given line can be a table line
     */
    bool istable( string& line);

    /** \brief test if the given line is a table align line
     * a table align line is the most important line it is the
     * second line in the table
     *
     * \param the line to text if this is a real table line
     * \return true if the given line is a table align line
     */
    bool istable_align( string& line);

    /** \brief does some parsing preparations
     * replaces - = lines with headlines and insert hr lines and stuff
     */
    void initial_manipulation();

    /** \brief does some parsing preparations
     *  replace headlines and hrules in blockquotes
     */
    void blockquote_manipulation(list<string>& lines);

    /** \brief insert a line into the buffer after given manipulations
     *
     * \param line to parse and insert into the buffer
     */
    void parse_line(string& s);

    /** \brief insert a code line into the buffer after given manipulations
     *
     * \param line to parse and insert into the buffer
     */
    void parse_code_line(string& s);

    /** \brief if the first in the string is a word or a number insert it
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_word(string& s);

    /** \brief if the first in the string is a bolded word do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_bold(string& s);

    /** \brief if the first in the string is a italic word do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_italic(string& s);

    /** \brief if the fist in the string is a unicode char do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_unicode(string& s);

    /** \brief if the first in the string is a comment do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_comment(string& s);

    /** \brief if the first in the string is a html tag do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_htmlTags(string& s);

    /** \brief if the first in the string is an open html tag do the stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_open_htmlTags(string& s);

    /** \brief if the first in the string is a closing html tag do the stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_close_htmlTags(string& s);

    /** \brief if the first in the string is a code do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_inline_code(string& s);

    /** \brief if the first in the string is a code block do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_inline_code_block(string& s);

    /** \brief if the first in the string is a reference do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_references(string& s);

    /** @brief if the first in the string is a footnote do the required stuff
     *
     * @param the line which will be detected
     * @return int 0 if nothing was found else the number of chars consumed -1
     */
    int find_footnote(string& s);

    /** \brief if the first in the string is a link reference do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_links(string& s);

    /** \brief if the first in the string is a new line break force do the required stuff
     *
     * \param the line which will be detected
     * \return 0 if nothing was found else the number of chars consumed -1
     */
    int find_new_line(string& s);

    /** \brief
     *
     * \param
     * \return
     */
    bool reference_line(string& line);

    /** \brief checks if the given line contains a reference
     *
     * \param a line to be checked
     * \return ture if the given line contain a reference
     */
    int header_line( string& line);

    /** \brief checks if the given line is a hr line
     *
     * \param the line to be checked
     * \return true if the given line is a horizontal rule line
     */
    bool horizontal_rule_line(string &line);

    /** \brief insert into the buffer the given level of indentation
     *
     * \param the indentation level
     */
    void insert_level(int level);

    /** \brief insert a new empty line into the buffer
     *
     */
    virtual void insert_line();

    /** \brief find for each opening html tag the closing one
     *
     * searches the given string for opening and closing tags, from the search given tag.
     * added and subtract them from the balance account.
     * In case that the balance will reach 0, the position of the last closing tag will be returned
     *
     * \param the html tag searching for
     * \param line in which we will be searching
     * \param the current balance account
     * \return 0 if balance is != 0 and in case the position of the last closing html tag
     */
    size_t get_tag_balance(string& tag, string& line, int& balance);

  protected:

    /** \brief abstract declaration of the footer_event function will be called after all parsing is done and headers is true
     *
     */
    virtual void footer_event()=0;

    /** \brief abstract declaration of the header_event function will be called before all parsing is done and headers is true
     *
     */
    virtual void header_event()=0;

    /** \brief event called if a header <hX> element should be insert
     *
     * \param the text to be insert as header
     * \param the header number 1-8
     * \param additional parameters like class and id
     * \return the generated string to be insert into the buffer
     */
    virtual string header_event(string line, int level, string paras="")=0;

    /** \brief event call if an horizontal line should be insert into the buffer
     *
     * \param parameters like id and class
     * \return the generated output to put in the buffer
     */
    virtual string horizontal_rule_event(string paras = "")=0;

    /** @brief event call if a new table block is started
     *
     * @param table_header the header of a table entries and alignment
     */
    virtual void table_begin_event(list<Align> table_header)=0;

    /** @brief event call if a table block will be ended
     *
     */
    virtual void table_end_event()=0;

    virtual void table_begin_header()=0;
    virtual void table_end_header()=0;

    virtual void table_begin_row()=0;
    virtual void table_end_row()=0;

    virtual void table_begin_col()=0;
    virtual void table_end_col()=0;

    virtual void table_begin_multi_col(int cols)=0;
    virtual void table_end_multi_col()=0;

    /** \brief event call if a new paragraph block is started
     *
     */
    virtual void paragraph_begin_event()=0;

    /** \brief event call if a paragraph block is ended
     *
     */
    virtual void paragraph_end_event()=0;

    /** \brief event call if a blockquote block is started
     *
     */
    virtual void blockquote_begin_event()=0;

    /** \brief event call if a blockquote block is ended
     *
     */
    virtual void blockquote_end_event()=0;

    /** \brief event call if a new list item will be started
     *
     */
    virtual void list_item_begin_event()=0;

    /** \brief event call after a new list item was inserted
     *
     */
    virtual void list_item_end_event()=0;

    /** \brief event call if a new list should be insert
     *
     */
    virtual void list_begin_event()=0;

    /** \brief event call if a new list was insert
     *
     */
    virtual void list_end_event()=0;

    /** \brief event call if a new ordered list should be insert
     *
     */
    virtual void ordered_list_begin_event()=0;

    /** \brief event call if a ordered list was insert
     *
     */
    virtual void ordered_list_end_event()=0;

    /** \brief event call if a new preformated block will be insert
     *
     */
    virtual void preformat_begin_event()=0;

    /** \brief event call if a new preformated block was insert
     *
     */
    virtual void preformat_end_event()=0;

    /** \brief event call if a new code block should be insert
     *
     * \param the language of the code block
     */
    virtual void code_begin_event(string lang = "")=0;

    /** \brief event call after a code block was insert
     *
     */
    virtual void code_end_event()=0;

    /** \brief event call if a new code block should be insert
     *
     * \param the language of the code block
     */
    virtual void code_inline_begin_event(string lang = "")=0;

    /** \brief event call after a code block was insert
     *
     */
    virtual void code_inline_end_event()=0;

    /** \brief event call before a comment block will be insert
     *
     */
    virtual void comment_begin_event()=0;

    /** \brief event call after a comment block was insert
     *
     */
    virtual void comment_end_event()=0;

    /** \brief event call before a html tag will be insert
     *
     * \param html tag style to be insert
     * \param further parameters to insert into tag
     */
    virtual void html_begin_event(string& tag, string& paras)=0;

    /** \brief event call after a html tag was insert
     *
     * \param the tag to be closed
     */
    virtual void html_end_event(string& tag)=0;

    /** \brief event call before bold text will be insert
     *
     */
    virtual void text_bold_begin_event()=0;

    /** \brief event call after bold text was insert
     *
     */
    virtual void text_bold_end_event()=0;

    /** \brief event call before italic text will be insert
     *
     */
    virtual void text_italic_begin_event()=0;

    /** \brief event call after italic text was insert
     *
     */
    virtual void text_italic_end_event()=0;

    /** \brief event call to insert a new line
     *
     */
    virtual void new_line_event()=0;

    /** \brief event to replace and insert the given c to the buffer
     *
     * Some special chars has to be replaced like &lt; and &gt; for the code environment
     * others can be just insert
     *
     * \param the char to replace
     */
    virtual void replace_code_char(const char& c)=0;

    /** \brief event to replace and insert the given c to the buffer
     *
     * Some special chars has to be replaced like &lt; and &gt; for the code environment
     * others can be just insert
     *
     * \param the string to replace
     */
    virtual void replace_code_char(string& line)=0;

    /** \brief event to replace a given char and insert it into the buffer
     *
     * Some chars need to be replaced before inserting into the buffer
     *
     * \param the char to analyze and insert into the buffer
     */
    virtual void replace_char(const char& c)=0;

    /** \brief checks if the given char needed to be escape and will do so
     *
     * \param the char which was escaped
     * \return 0 if it was not escaped or 1 if it was
     */
    virtual int  check_escaped_char( const char& c)=0;

    /** \brief takes a string containing text and will return an hexcode representation of that text
     *
     * \param the text to hide
     * \return return the hidden text
     */
    virtual string hide_chars(string line)=0;


    /** \brief generate an link to an given item
     *
     * \param a reference struct containing link target and title if any
     * \param the name to display
     */
    virtual void generate_link( Ref ref, string& name)=0;

    /** \brief generate an link to an given item, without further char replacement
     *
     * \param a reference struct containing link target and title if any
     * \param the name to display
     */
    virtual void generate_link_no_replace( Ref ref, string& name)=0;

    /** \brief generate an link to an given image
     *
     * \param a reference struct containing link target and title if any
     * \param the alt text for the image
     */
    virtual void generate_img( Ref src, string& alt)=0;

    /** @brief generate an image with a link inline
     *
     * @param src Ref the link part
     * @param alt containing the image part to call parse again
     */
    virtual void generate_link_img( Ref src, string& alt)=0;

    /** @brief set the footnote exact at this position, maybe the description should go somewhere
     * else
     *
     * @param the footnote content has to be parse again
     */
    virtual void footnote_event(string& footnote)=0;

    /** @brief for classes they need a special place for there footnotes.
     * this routine is called after all parsing is done, but before the footer jumps in
     *
     */
    virtual void footnote_end_event()=0;

    int level; // ground level
    bool m_line_sensitiv; ///< insert some command to force an line brake in some environments

  private:

    map<string,Ref> m_refs; /// referencen will be stored here
    map<string,string> m_footnote_refs; /// referencen will be stored here

    regex m_bold;   /// regex to find bold manipulation
    regex m_italic; /// regex to find italic manipulation
    regex m_unicodeChar; /// regex to find unicode chars
    regex m_comment; /// regex to find inline comments
    regex m_htmlTags; /// regex to find open and closing html tags
    regex m_htmlOpenTag; /// regex to find open html tags
    regex m_htmlblock; /// regex to find a html block
    regex m_htmlcomment; /// regex to find a html comment block
    regex m_htmlCloseTag; /// regex to find an html closing tag
    regex m_htmlTag; /// regex to find a html tag
    regex m_inline_code; /// regex to find inline code
    regex m_inline_code_block; /// regex to find inline code block
    regex m_headline; /// regex to find headline in the lines
    regex m_header_rule1; /// regex to find markdown headlines
    regex m_header_rule2;/// regex to find markdown headlines
    regex m_horizontal_rule1; /// regex to find markdown horizontal line
    regex m_horizontal_rule2; /// regex to find markdown horizontal line
    regex m_hrule; /// regex to find an hr html
    regex m_anylist; /// regex to find find any list
    regex m_ulist; /// regex to find unordered list
    regex m_olist; /// regex to find ordered list
    regex m_preformated;/// regex to find an preformated block
    regex m_code; /// regex to find a code block
    regex m_code_end; /// regex to find an end of code block
    regex m_reference_db; /// regex to find reference in text [] [] style
    regex m_reference_inline; /// regex to find reference in text [] () style
    regex m_links; /// regex to find reference in text <>
    regex m_force_line;/// regex to find a line brake force in paragraph
    regex m_ref; /// regex to find lines containing references
    regex m_word; /// regex to find a normal number or word
    regex m_email; /// regex to find email block
    regex m_table; /// regex find header or normal line
    regex m_table_empty; /// regex to find an empty table line
    regex m_table_align; /// regex to find the most important line of a table
    regex m_footnote_db;/// regex to find a footnote db entry
    regex m_footnote; /// regex to find a footnote in text

    smatch m_match; /// get regex information
    smatch m_match2; /// get regex information ( table)

    bool m_headers; /// flag for calling \c header_event or \c footer_event
    bool m_ordered; /// if a list is an ordered list
    int m_level;
};

#endif // MARKDOWN_PARSER_H_

