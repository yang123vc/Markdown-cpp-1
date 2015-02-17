#include "latexParser.h"

#include <iostream>

LatexParser::LatexParser()
{

}

LatexParser::~LatexParser()
{

}

void LatexParser::add_header()
{
    insert("\\documentclass{article}");
    insert("\\begin{document}");
}

void LatexParser::add_footer()
{
    insert("\\end{document}");
}
