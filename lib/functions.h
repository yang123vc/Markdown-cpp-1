#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;

// trim from start
static inline string& ltrim(string& s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

// trim from end
static inline string& rtrim(string& s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline string& trim(string& s)
{
  return ltrim(rtrim(s));
}

// make all letters small
static inline void toLower(string& s)
{
  for( string::iterator it = s.begin(); it != s.end(); ++it)
    *it = tolower(*it);
}

#endif // FUNCTIONS_H_

