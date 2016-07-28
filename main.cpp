#include <iostream>
#include "./includes/pen-utility.hpp"
using namespace std;
int pos = 0;
int ret_v;
int main(int argc, char *argv[])
{
    string op;
    ifstream fin;
    if (argc == 1)
    {
        cout << "Input the assembly code path:";
        getline(cin, op);
        Encoder.bind(op);
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
        Encoder.bind(argv[2]);
        while (!fin.eof())
        {
            getline(fin, op);
            Scanner.append(op);
            Scanner.proc_import();
        }
    }
/*    for (size_t i = 0; i < Scanner.lexemes.size(); ++i)
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
    Encoder.close();
    exit(ret_v);
}
