#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>

using namespace std;

// trim from start
static inline string &ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
}

// trim from end
static inline string &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline string &trim(string &s) {
        return ltrim(rtrim(s));
}

static inline void toLower(basic_string<char>& s) {
   for (basic_string<char>::iterator p = s.begin();
        p != s.end(); ++p) {
      *p = towlower(*p);
   }
}

vector<string> parameter( string line);

#endif // FUNCTIONS_H_
