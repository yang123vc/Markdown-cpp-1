#include "htmlParser.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>

HTMLParser::HTMLParser()
{
  //ctor
}

HTMLParser::~HTMLParser()
{
  //dtor
}

void HTMLParser::footer_event()
{

}

void HTMLParser::header_event()
{

}



string HTMLParser::header_event(string line, int level, string paras)
{
  char buffer[line.length() + 12 + paras.length()];
  sprintf(buffer, "<h%d%s>%s</h%d>", level, paras.c_str(), line.c_str(), level);
  return buffer;
}

string HTMLParser::horizontal_rule_event(string paras)
{
  if( paras.empty())
    return "<hr />";
  else
  {
    return "<hr" + paras + "/>";
  }
}

void HTMLParser::table_begin_event(list<Align> table_header)
{
  table_align.clear();
  insert("<table>");
  insert_line();
  for( Align th : table_header)
  {
    if( th != DEFAULT)
    {
      string temp = " style=\"text-align:";
      if( th == RIGHT)
        temp+= "right";
      else if( th == LEFT)
        temp += "left";
      else if( th == CENTER)
        temp += "center";
      temp += "\"";
      table_align.push_back(temp);
    }
    else
    {
      table_align.push_back("");
    }
  }
}

void HTMLParser::table_end_event()
{
  insert("</table>");
}

void HTMLParser::table_begin_row()
{
  insert("<tr>");
  insert_line();
  ta_it = table_align.begin();
}

void HTMLParser::table_end_row()
{
  insert("</tr>");
  insert_line();
}

void HTMLParser::table_begin_col()
{
  if( ta_it != table_align.end())
  {

    insert("<td");
    if( !(*ta_it).empty())
      insert(*ta_it);
    insert(">");
  }
}

void HTMLParser::table_end_col()
{
  if( ta_it != table_align.end())
  {
    insert("</td>");
    ta_it++;
    insert_line();
  }
}

void HTMLParser::table_begin_multi_col(int cols)
{
  if( ta_it != table_align.end())
  {

    insert("<td");
    insert(" colspan=\"");
    char buffer[5];
    sprintf( buffer, "%d", cols);
    insert(buffer);
    insert("\"");
    if( !(*ta_it).empty())
      insert(*ta_it);
    insert(">");
  }
}

void HTMLParser::table_end_multi_col()
{
  table_end_col();
}

void HTMLParser::table_begin_header()
{
  if( ta_it != table_align.end())
  {

    insert("<th");
    if( !(*ta_it).empty())
      insert(*ta_it);
    insert(">");
  }
}

void HTMLParser::table_end_header()
{
  if( ta_it != table_align.end())
  {
    insert("</th>");
    insert_line();
    ta_it++;
  }
}

void HTMLParser::paragraph_begin_event()
{
  insert( "<p>");
}

void HTMLParser::paragraph_end_event()
{
  insert( "</p>");
}

void HTMLParser::blockquote_begin_event()
{
  insert( "<blockquote>");
}

void HTMLParser::blockquote_end_event()
{
  insert( "</blockquote>");
}

void HTMLParser::list_item_begin_event()
{
  insert( "<li>");
}

void HTMLParser::list_item_end_event()
{
  insert( "</li>");
}


void HTMLParser::list_begin_event()
{
  insert( "<ul>");
}

void HTMLParser::list_end_event()
{
  insert( "</ul>");
}

void HTMLParser::ordered_list_begin_event()
{
  insert( "<ol>");
}

void HTMLParser::ordered_list_end_event()
{
  insert( "</ol>");
}

void HTMLParser::preformat_begin_event()
{
  insert( "<pre>");
}

void HTMLParser::preformat_end_event()
{
  insert( "</pre>");
}

void HTMLParser::code_begin_event(string lang)
{
  if( lang.empty())
    insert( "<code>");
  else
    insert( "<code class='"+lang+"'>");
}

void HTMLParser::code_end_event()
{
  insert( "</code>");
}

void HTMLParser::new_line_event()
{
  insert(" <br />");
}

void HTMLParser::comment_begin_event()
{
  insert("<!--");
}

void HTMLParser::comment_end_event()
{
  insert("-->");
}

void HTMLParser::html_begin_event(string& tag, string& paras)
{
  insert("<" + tag + paras + ">");
}

void HTMLParser::html_end_event(string& tag)
{
  insert("</"+tag+">");
}

void HTMLParser::text_bold_begin_event()
{
  insert("<strong>");
}

void HTMLParser::text_bold_end_event()
{
  insert("</strong>");
}

void HTMLParser::text_italic_begin_event()
{
  insert("<em>");
}

void HTMLParser::text_italic_end_event()
{
  insert("</em>");
}


void HTMLParser::replace_code_char(const char& c)
{
  switch( c)
  {
    case '<':
      insert( "&lt;");
      break;
    case '>':
      insert( "&gt;");
      break;
    case '&':
      insert( "&amp;");
      break;

    case '\t':
      insert("    ");
      break;

    default:
      insert( c);
      break;
  }
}

void HTMLParser::replace_code_char(string& line)
{

  string temp = "";
  for( char c : line)
  {
    if( c == '<')
    {
      temp += "&lt;";
    }
    else if( c == '>')
    {
      temp += "&gt;";
    }
    else if( c == '&')
    {
      temp += "&amp;";
    }
    else if( c == '\t')
    {
      temp += "    ";
    }
    else
    {
      temp += c;
    }
  }
  insert(temp);
}

void HTMLParser::replace_char(const char& c)
{
  switch( c)
  {
    case '&':
      insert( "&amp;");
      break;

    case '<':
      insert( "&lt;");
      break;

    case '>':
      insert( "&gt;");
      break;

    case '\"':
      insert("&quot;");
      break;

    case '\t':
      insert("    ");
      break;

    case '^':
      insert("&circ;");
      break;

    case '~':
      insert("&tilde;");
      break;

    default:
      insert( c);
      break;
  }
}

int HTMLParser::check_escaped_char(const char& c)
{
  int len = 1;
  switch(c)
  {
    case '*':
    case '_':
    case '-':
    case '+':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '>':
    case '\\':
    case '#':
    case '`':
    case '.':
    case '!':
      insert(c);
      break;
    default:
      len = 0;
      break;
  }
  return len;
}

string HTMLParser::hide_chars(string line)
{
  string hidden ="";
  for( char c : line)
  {
    char buffer[20];
    sprintf(buffer, "&#x%x;", c);
    hidden += buffer;
  }
  return hidden;
}

void HTMLParser::generate_link_no_replace(Ref ref, string& name)
{
  insert("<a href='");
  insert( ref.link);
  if( !ref.title.empty())
  {
    insert("' title='");
    insert(ref.title);
  }
  insert("'>");
  insert( name);
  insert("</a>");
}

void HTMLParser::generate_link(Ref ref, string& name)
{
  insert("<a href='");
  for( char c : ref.link)
    replace_char(c);
  if( !ref.title.empty())
  {
    insert("' title='");
    for( char c : ref.title)
      replace_char(c);
  }
  insert("'>");
  for( char c : name)
    replace_char(c);
  insert("</a>");
}

void HTMLParser::generate_img(Ref src, string& alt)
{
  insert("<img src='");
  for( char c : src.link)
    replace_char(c);
  insert("' alt='");
  for( char c : alt)
    replace_char(c);
  insert("'/>");
}


void HTMLParser::generate_link_img(Ref ref, string& name)
{
  insert("<a href='");
  for( char c : ref.link)
    replace_char(c);
  if( !ref.title.empty())
  {
    insert("' title='");
    for( char c : ref.title)
      replace_char(c);
  }
  insert("'>");
  find_references(name);
  insert("</a>");
}

void HTMLParser::footnote_event(string& footnote)
{
  m_footer_content.push_back(footnote);
  insert("<sup><a href=\"#fn");
  char buf[5];
  sprintf(buf, "%d", m_footer_content.size());
  insert(buf);
  insert("\" id=\"ref");
  insert(buf);
  insert("\">");
  insert(buf);
  insert("</a></sup>");
}

void HTMLParser::footnote_end_event()
{
  char buf[5];
  int i= 1;
  for( string content : m_footer_content)
  {
    insert("<sup id=\"fn");
    sprintf(buf, "%d", i);
    insert(buf);
    insert("\">");
    insert(buf);
    insert(". ");
    parse_line(content);
    insert("<a href=\"#ref");
    insert(buf);
    insert("\" title=\"Jump back to footnote ");
    insert(buf);
    insert(" in the text.\">&#8617;</a></sup>");
    if( i != m_footer_content.size())
    {
      insert("<br>");
    }
    i++;
    insert_line();
  }
}
