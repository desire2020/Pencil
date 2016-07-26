#include <iostream>
#include "./includes/pen-utility.hpp"
using namespace std;
constexpr size_t size = 1800 << 20;
char p[size];


int pos = 0;
int ret_v;
int main(int argc, char *argv[])
{
//This assembly code expands the system stack space upto 1800MB. x86-64 only.
    __asm__("MOVQ %0, %%RSP\n" :: "r"(p + size));
    string op;
    ifstream fin;
    if (argc == 1)
    {
        while (true)
        {
            getline(cin, op);
            if (op == "exit")
                break;
            Scanner.append(op);
            Scanner.proc_import();
        }
    } else {
        fin.open(argv[1], ios :: in);
        while (!fin.eof())
        {
            getline(fin, op);
            Scanner.append(op);
            Scanner.proc_import();
        }
    }
    Scanner.append("(main)");
 /*   for (size_t i = 0; i < Scanner.lexemes.size(); ++i)
    {
        cout << "Token #" << i << ":\t" << Scanner.lexemes[i] << endl;
    }
    cout << endl;
    cout << "########################Token list end#############################" << endl;*/
    Parser.rebind(Scanner.lexemes);

    while (pos < Scanner.lexemes.size())
    {
        auto p = Parser.execute(pos).int_val;
        if (p != NULL)
           ret_v = *p;
        else break;
    }
    exit(ret_v);
}
