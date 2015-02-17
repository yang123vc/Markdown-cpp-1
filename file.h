#ifndef FILE_H__
#define FILE_H__

#include <fstream>
#include <string>
#include <cerrno>

using namespace std;

string get_file_contents(const char* filename);
void set_file_contents( const char* filename, const string& content);


#endif // FILE_H__
