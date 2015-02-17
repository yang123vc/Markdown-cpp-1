#ifndef LINKS_H_
#define LINKS_H_

#include <string>
#include <map>
#include <vector>


using namespace std;

struct sRef
{
    string link;
    string titel;
    vector<string> paras;
    string name;
};

typedef struct sRef Ref;

map<string,Ref> extract_references(vector<string>& lines);

#endif // LINKS_H_
