#include "file.h"

string get_file_contents(const char *filename)
{
  ifstream in(filename, ios::in | ios::binary);
  if (in)
  {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

void set_file_contents(const char* filename, const string& content)
{
    fstream out(filename, ios::out | ios::binary);
    if( out){
        out.write(content.c_str(), content.length());
        out.close();
    }
    else
        throw(errno);
}
