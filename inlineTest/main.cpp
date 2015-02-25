#include <iostream>
#include <string>
#include <list>

using namespace std;

int get_tag_balance(int& a)
{
    a++;
}

int main(int argc, char** argv)
{

    int a = 0;

    get_tag_balance(a);

    cout << a << endl;

    return 0;
}
