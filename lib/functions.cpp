#include "functions.h"

#include <iostream>

vector<string> parameter(string line){
    vector<string> params;
    char para[512];
    int j = 0;
    bool wasSpace = false;
    for( size_t i= 0; i < line.length(); i++){
        char c = line[i];
        if( isspace(c)){
            if( !wasSpace){
                para[j] = '\0';
                params.push_back(para);
                j = 0;
                wasSpace = true;
            }
        }else if( c == '\"'){
            wasSpace = false;
            i++;
            while( line[i] != '\"' && i < line.length())
                para[j++] = line[i++];
        }else{
            para[j++] = c;
            wasSpace = false;
        }
    }
    if( j != 0){
        para[j] = '\0';
        params.push_back(para);
    }

    return params;
}

