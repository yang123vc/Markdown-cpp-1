#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "links.h"

using namespace std;

struct sFence{
    string opening;
    string closing;
};

typedef struct sFence Fence;

Fence register_Fence(string fence);
Fence register_Fence(string opening_fence, string closing_fence);

class Parser {

    public:
        Parser();
        virtual ~Parser();

        virtual void add_header();
        virtual void add_footer();

        virtual void new_line(string line);
        virtual void edit_block(string block);

        virtual void empty_line_event();
        virtual void mul_empty_line_event();
        virtual int spezial_char_event( const char* c);
        virtual int reference_event( const char* c);

        virtual bool isspezial( char c);
        virtual bool isreference( char c);

        virtual void flush();

        string get_content();

        void insert(string cont);
        void insert_block(string cont);
        void add(string cont);
        void add_block( string cont);
        void newline();

        void add_temp(string cont);

        void set_refs(map<string,Ref>* refs);
        map<string,Ref>::iterator find_ref(string item);
        bool found_ref(map<string,Ref>::iterator item);

    protected:

        string content;
        string block;
        string temp;

        map<string,Ref>* refs;

        bool empty_line;

};

#endif // PARSER_H_
