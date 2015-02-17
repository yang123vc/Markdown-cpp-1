#ifndef LATEX_PARSER_H_
#define LATEX_PARSER_H_

#include "parser.h"

class LatexParser : public Parser{

    public:
        LatexParser();
        virtual ~LatexParser();


        void add_header();
        void add_footer();
};

#endif // LATEX_PARSER_H_
