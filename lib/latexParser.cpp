#include "latexParser.h"
#include <algorithm>

LaTexParser::LaTexParser()
{
  first_quote = true;
  m_figure = true;
  m_language.push_back("abap");
  m_language.push_back("acsl");
  m_language.push_back("ada");
  m_language.push_back("algol");
  m_language.push_back("ant");
  m_language.push_back("assambler");
  m_language.push_back("awk");
  m_language.push_back("bash");
  m_language.push_back("basic");
  m_language.push_back("c");
  m_language.push_back("c++");
  m_language.push_back("caml");
  m_language.push_back("clean");
  m_language.push_back("cobol");
  m_language.push_back("comal");
  m_language.push_back("csh");
  m_language.push_back("delphi");
  m_language.push_back("eiffel");
  m_language.push_back("elan");
  m_language.push_back("erlanf");
  m_language.push_back("euphoria");
  m_language.push_back("fortran");
  m_language.push_back("gcl");
  m_language.push_back("gnuplot");
  m_language.push_back("haskell");
  m_language.push_back("html");
  m_language.push_back("idl");
  m_language.push_back("inform");
  m_language.push_back("java");
  m_language.push_back("jvmis");
  m_language.push_back("ksh");
  m_language.push_back("lisp");
  m_language.push_back("logo");
  m_language.push_back("make");
  m_language.push_back("mathematica");
  m_language.push_back("matlab");
  m_language.push_back("mercury");
  m_language.push_back("metapost");
  m_language.push_back("miranda");
  m_language.push_back("mizar");
  m_language.push_back("ml");
  m_language.push_back("modelica");
  m_language.push_back("modula-2");
  m_language.push_back("mupad");
  m_language.push_back("nastran");
  m_language.push_back("oberon-2");
  m_language.push_back("ocl");
  m_language.push_back("octave");
  m_language.push_back("oz");
  m_language.push_back("pascal");
  m_language.push_back("perl");
  m_language.push_back("php");
  m_language.push_back("pl/i");
  m_language.push_back("plasm");
  m_language.push_back("pov");
  m_language.push_back("prolog");
  m_language.push_back("promela");
  m_language.push_back("python");
  m_language.push_back("r");
  m_language.push_back("reduce");
  m_language.push_back("rexx");
  m_language.push_back("rsl");
  m_language.push_back("ruby");
  m_language.push_back("s");
  m_language.push_back("sas");
  m_language.push_back("scilab");
  m_language.push_back("sh");
  m_language.push_back("shelxl");
  m_language.push_back("simula");
  m_language.push_back("sql");
  m_language.push_back("tcl");
  m_language.push_back("tex");
  m_language.push_back("vbscript");
  m_language.push_back("verilog");
  m_language.push_back("vhdl");
  m_language.push_back("vrml");
  m_language.push_back("xml");
  m_language.push_back("xslt");
}

LaTexParser::~LaTexParser()
{

}

void LaTexParser::footer_event()
{
  insert("\\end{document}\n");
}

void LaTexParser::header_event()
{
  insert("\\documentclass{scrartcl}\n\n");
  insert("\\usepackage[utf8]{inputenc}\n");
  insert("\\usepackage[T1]{fontenc}\n");
  insert("\\usepackage[ngerman]{babel}\n");
  insert("\\usepackage{tikz}\n");
  insert("\\usepackage{hyperref}\n");
  insert("\\usepackage{graphicx}\n");
  insert("\\usepackage{listings}\n");
  insert("\\usepackage{multirow}\n");
  insert("\\usepackage{amssymb}\n");
  insert("\\setlength\\parindent{0pt}\n");
  insert("\n");
  insert("\\newcommand{\\comment}[1]{}\n");
  insert("\n");
  insert("\\begin{document}\n");
}

string LaTexParser::header_event(string line, int level, string paras)
{

  string header ="";
  switch( level)
  {
    case 1:
      header += "\\section{";
    break;

    case 2:
      header += "\\subsection{";
    break;

    case 3:
      header += "\\subsubsection{";
    break;

    case 4:
      header += "\\paragraph{";
    break;

    case 5:
      header += "\\subparagraph{";
    break;

    default:
      header += "\\textbf{";
    break;
  }
  header += line;
  header += "}";
  if( level < 6)
  {
    header += "\\label{";
    size_t pos;
    while( (pos = line.find("ß")) != string::npos)
      line.replace(pos, 2, "\\\"ss");
    header += line;
    header += "}";
  }
  return header;
}

string LaTexParser::horizontal_rule_event(string paras)
{
  string line = "\\rule{\\linewidth}{0.4pt}";
  return line;
}

void LaTexParser::table_begin_event(list<Align> table_header)
{
  insert("\\begin{tabular}{");
  table_cols = table_header.size();
  for( Align a : table_header)
  {
    if( a == CENTER || a == DEFAULT)
    {
      insert("c");
      m_align_cols.push_back('c');
    }
    else if( a == LEFT)
    {
      insert("l");
      m_align_cols.push_back('l');
    }
    else if( a == RIGHT)
    {
      insert("r");
      m_align_cols.push_back('r');
    }
  }
  insert("}\n");
}

void LaTexParser::table_end_event()
{
  insert("\\end{tabular}");
}

void LaTexParser::table_begin_row()
{
  current_col = 0;
}

void LaTexParser::table_end_row()
{
  insert("\\\\\n\\hline\n");
}

void LaTexParser::table_begin_col()
{

}

void LaTexParser::table_end_col()
{
  current_col++;
  if( current_col < table_cols)
    insert(" & ");
}

void LaTexParser::table_begin_header()
{
}

void LaTexParser::table_end_header()
{
  current_col++;
  if(current_col < table_cols)
    insert(" & ");
}

void LaTexParser::table_begin_multi_col(int cols)
{
  char buffer[128];
  sprintf(buffer, "\\multicolumn{%d}{%c}{", cols, m_align_cols[current_col]);
  insert(buffer);
}

void LaTexParser::table_end_multi_col()
{
  insert("}");

}

void LaTexParser::paragraph_begin_event()
{
  m_line_sensitiv = true;
  newline_count = 0;
}

void LaTexParser::paragraph_end_event()
{
  m_line_sensitiv = false;
  if( newline_count > 0)
    new_line_event();
}

void LaTexParser::blockquote_begin_event()
{
  insert("\\begin{quote}");
}

void LaTexParser::blockquote_end_event()
{
  insert("\\end{quote}");
}

void LaTexParser::list_item_begin_event()
{
  insert("\\item ");
}

void LaTexParser::list_item_end_event()
{
}

void LaTexParser::list_begin_event()
{
  insert("\\begin{itemize}");
}

void LaTexParser::list_end_event()
{
  insert("\\end{itemize}");
}

void LaTexParser::ordered_list_begin_event()
{
  insert("\\begin{enumerate}");
}

void LaTexParser::ordered_list_end_event()
{
  insert("\\end{enumerate}");
}

void LaTexParser::preformat_begin_event()
{

}

void LaTexParser::preformat_end_event()
{

}

void LaTexParser::code_begin_event(string lang)
{
  if( !lang.empty())
  {
    string lang_small;
    for(char c : lang)
      lang_small+=tolower(c);

    if( find(m_language.begin(), m_language.end(), lang_small) != m_language.end())
      insert("\\lstset{language="+lang+"}\n");
  }
  insert("\\begin{lstlisting}\n");
}

void LaTexParser::code_end_event()
{
  insert("\\end{lstlisting}");
}

void LaTexParser::code_inline_begin_event(string lang)
{
  insert("\\texttt{");
}

void LaTexParser::code_inline_end_event()
{
  insert("}");
}

void LaTexParser::comment_begin_event()
{
  insert("\\comment{");
}

void LaTexParser::comment_end_event()
{
  insert("}");
}

void LaTexParser::html_begin_event(string& tag, string& paras)
{

}

void LaTexParser::html_end_event(string& tag)
{

}

void LaTexParser::text_bold_begin_event()
{
  insert("\\textbf{");
}

void LaTexParser::text_bold_end_event()
{
  insert("}");
}

void LaTexParser::text_italic_begin_event()
{
  insert("\\emph{");
}

void LaTexParser::text_italic_end_event()
{
  insert("}");
}

void LaTexParser::new_line_event()
{
  m_content.pop_back();
  insert("\n");
  insert_level(level);
}

void LaTexParser::replace_code_char(const char& c)
{
  if( c == '#' || c == '&')
  {
    insert('\\');
    insert(c);
  }
  else if( c == '\"')
  {
    if( first_quote)
      insert("``");
    else
      insert("''");
    first_quote = !first_quote;
  }
  else
    insert(c);
}

void LaTexParser::replace_code_char(string& line)
{
  insert(line);
}

void LaTexParser::replace_char(const char& c)
{
  if( c == '#')
    insert("\\#");
  else if(c == '&')
    insert("\\&");
  else if( c == '\"')
  {
    if( first_quote)
      insert("``");
    else
      insert("''");
    first_quote = !first_quote;
  }
  else if( c == '$')
  {
    if( m_last_char == '$')
    {
      m_content.pop_back();
      m_content.pop_back();
      insert("$$");
    }
    else
      insert("\\$");
  }
  else
    insert(c);
  m_last_char = c;
  if( c == '\n')
    newline_count++;
}

int LaTexParser::check_escaped_char(const char& c)
{
  int len = 1;
  switch(c)
  {
    case '\\':
      insert(c);
      insert(c);
    break;
    case '"':
      insert(c);
    break;
    default:
      insert('\\');
      len = 0;
    break;
  }
  return len;
}

string LaTexParser::hide_chars(string line)
{
  return line;
}

void LaTexParser::generate_link(Ref ref, string& name)
{
  insert("\\href{");
  for( char c : ref.link)
    replace_char(c);
  insert("}{");
  parse_line(name);
  insert("}");
}

void LaTexParser::generate_link_no_replace(Ref ref, string& name)
{
  insert("\\href{");
  insert( ref.link);
  insert("}{");
  insert(name);
  insert("}");
}

void LaTexParser::generate_img(Ref src, string& alt)
{
  if( m_figure)
  {
    insert("\\begin{figure}[htbp]\n");
    insert_level(level+1);
    insert("\\centering\n");
    insert_level(level);
  }
  insert("\\includegraphics{");
  for( char c : src.link)
    replace_char(c);
  insert("}");
  if( m_figure)
  {
    insert("\n");
    insert_level(level);
    insert("\\caption{");
    for( char c : alt)
      replace_char(c);
    insert("}\n");
    insert_level(level-1);
    insert("\\end{figure}");
  }
}

void LaTexParser::generate_link_img(Ref src, string& name)
{
  insert("\\href{");
  for( char c : src.link)
    replace_char(c);
  insert("}{");
  m_figure = false;
  find_references(name);
  m_figure = true;
  if( m_content.back() == '\3')
    m_content.pop_back();
  insert("}");
}

void LaTexParser::footnote_event(string& footnote)
{
  insert("\\footnote{");
  parse_line(footnote);
  insert("}");
}

void LaTexParser::footnote_end_event()
{
}

void LaTexParser::toc_event()
{
  cout << "TABLE OF CONTENTS" << endl;
  insert("\\tableofcontents");
}
