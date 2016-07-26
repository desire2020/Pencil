#include "./includes/pen-utility.hpp"

void TError :: TMessage :: operator()(const string & _details)
{
    std :: cout << _details << std :: endl;
    exit(0);
}
void TError :: TDebugMessage :: operator ()(const string & _details)
{
    std :: cout << _details << std :: endl;
}

TScanner :: TToken :: TToken() {}
TScanner :: TToken :: TToken(TToken_name _token_name, size_t _attribute_value) : token_name(_token_name), attribute_value(_attribute_value) {}
bool TScanner :: TToken :: operator == (const TScanner :: TToken & rhs) const
{
    return token_name == rhs.token_name && attribute_value == rhs.attribute_value;
}

string TScanner :: TToolkit :: str(long long x)
{
    if (x == 0)
        return "0";
    if (x < 0)
        return "-" + str(-x);
    string ans;
    ans = "";
    while (x > 0)
    {
        ans = ((char)(x % 10 + '0')) + ans;
        x /= 10;
    }
    return ans;
}

long long TScanner :: TToolkit :: val(const std::__cxx11::string &x)
{
    if (x == "" || x == "0")
        return 0;
    if (x[0] == '-')
        return -val(x.substr(1, x.length()));
    long long ans = 0;
    if (x[0] != '0')
    {
        for (int i = 0; i <= x.length() - 1; ++i)
        {
            if (x[i] > '9' || x[i] < '0')
                Error.message("Invalid immediate number \"" + x + "\"");
            ans *= 10;
            ans += x[i] - '0';
        }
        return ans;
    } else {
        if (x[1] == 'x')
        {
            for (int i = 2; i <= x.length() - 1; ++i)
            {
                if (x[i] <= '9' && x[i] >= '0')
                {
                    ans *= 16;
                    ans += x[i] - '0';
                } else {
                    if (x[i] >= 'a' && x[i] <= 'f')
                    {
                        ans *= 16;
                        ans += x[i] - 'a' + 10;
                    } else {
                        if (x[i] >= 'A' && x[i] <= 'F')
                        {
                            ans *= 16;
                            ans += x[i] - 'A' + 10;
                        } else {
                            Error.message("Invalid immediate number \"" + x + "\"");
                        }
                    }
                }
            }
        } else {
            for (int i = 1; i <= x.length() - 1; ++i)
            {
                if (x[i] <= '7' && x[i] >= '0')
                {
                    ans *= 8;
                    ans += x[i] - '0';
                } else {
                    Error.message("Invalid immediate number \"" + x + "\"");
                }
            }
        }
    }
}
string TScanner :: TToolkit :: eschar_reinterpret(const std::__cxx11::string &src)
{
    string new_cp;
    for (int i = 0; i < src.length(); ++i)
    {
        if (src[i] != '\\')
        {
            new_cp += src[i];
        } else {
            ++i;
            if (i >= src.length())
            {
                Error.message("Invalid escape character occured.");
                return src;
            }
            switch (src[i])
            {
                case 'n' :
                    new_cp += '\n';
                break;
                case 't' :
                    new_cp += '\t';
                break;
                case '\\' :
                    new_cp += '\\';
                break;
                case '0' :
                    new_cp += '\0';
                break;
                case '\"' :
                    new_cp += '\"';
                break;
                case '\'' :
                    new_cp += '\'';
                break;
                case 'r' :
                    new_cp += '\r';
                break;
                case 'v' :
                    new_cp += '\v';
                break;
            }
        }
    }
    return new_cp;
}

void TScanner :: append(const std::__cxx11::string &src)
{
    int p = 0, last_p = 0;
    string symbol;
    while (last_p < src.length())
    {
        while (p < src.length()
            && src[p] != ' '
            && src[p] != '\t'
            && src[p] != '('
            && src[p] != ')'
            && src[p] != '['
            && src[p] != ']'
            && src[p] != '{'
            && src[p] != '}'
            && src[p] != '\"') ++p;
        symbol = src.substr(last_p, p - last_p);
        if (src[last_p] == '(')
        {
            ++p;
            lexemes.push_back(TToken(assign, _round_bracket_l));
        } else
            if (src[last_p] == ')')
            {
                ++p;
                lexemes.push_back(TToken(assign, _round_bracket_r));
            } else
                if (src[last_p] == '[')
                {
                    ++p;
                    lexemes.push_back(TToken(assign, _rect_bracket_l));
                } else
                    if (src[last_p] == ']')
                    {
                        ++p;
                        lexemes.push_back(TToken(assign, _rect_bracket_r));
                    } else
                        if (src[last_p] == '{')
                        {
                            ++p;
                            lexemes.push_back(TToken(assign, _italian_bracket_l));
                        } else
                            if (src[last_p] == '}')
                            {
                                ++p;
                                lexemes.push_back(TToken(assign, _italian_bracket_r));
                            } else
                                if (src[last_p] == '\"')
                                {
                                    ++p;
                                    while (p < src.length() && src[p] != '\"') ++p;
                                    if (p >= src.length())
                                        Error.message("Fatal : Quote mismatch.");
                                    ++last_p;
                                    seq_imm_str.push_back(std :: move(tools.eschar_reinterpret(src.substr(last_p, p - last_p))));
                                    lexemes.push_back(TToken(immediate_str, seq_imm_str.size() - 1));
                                    ++p;
                                } else
                                    if (symbol == "" || symbol == " " || symbol == "\t")
                                    {
                                        ++p;
                                    } else
                                        if (symbol[0] >= '0' && symbol[0] <= '9')
                                        {
                                            seq_imm_int.push_back(tools.val(symbol));
                                            lexemes.push_back(TToken(immediate_int, seq_imm_int.size() - 1));
                                        } else {
                                            seq_identifier.push_back(symbol);
                                            lexemes.push_back(TToken(id, seq_identifier.size() - 1));
                                        }
        last_p = p;
    }
}
void TScanner :: proc_import()
{
    if (lexemes.size() < 2)
        return;
    TToken rhs = lexemes[lexemes.size() - 2];
    TToken path = lexemes[lexemes.size() - 1];
    if (rhs.token_name != id || path.token_name != immediate_str) return;
    if (seq_identifier[rhs.attribute_value] == "import" && path.token_name == immediate_str)
    {
        lexemes.pop_back();
        lexemes.pop_back();
        ifstream fin(seq_imm_str[path.attribute_value].c_str());
        string op;
        while (!fin.eof())
        {
            std :: getline(fin, op);
            append(op);
        }
        fin.close();
    }
}

void TFile :: fopen(const std::__cxx11::string &path)
{
    ifstream fin(path.c_str());
    string op;
    while (!fin.eof())
    {
        std :: getline(fin, op);
        inside.push_back(op);
    }
    fin.close();
}
void TFile :: fopen(std::istream &src)
{
    string op;
    while (!src.eof())
    {
        std :: getline(src, op);
        inside.push_back(op);
    }
}

TFunction :: TFunction() : title(""), l(0), r(0) {}
TFunction :: TFunction(const std::__cxx11::string &_title, int _l, int _r) : title(_title), l(_l), r(_r) {}
TFunction :: TFunction(int _l, int _r) : title(""), l(_l), r(_r) {}

Package :: Package(const std::__cxx11::string &_str)
    :
      int_val(),
      str_val(new string(_str)),
      seq_val(),
      code_seg() {}
Package :: Package(long long _int)
    :
      int_val(new long long(_int)),
      str_val(),
      seq_val(),
      code_seg() {}
Package :: Package(const TSeq_arg & _seq)
    :
      int_val(),
      str_val(),
      seq_val(new TSeq_arg(_seq)),
      code_seg() {}
Package :: Package(TSeq_arg && _seq)
    :
      int_val(),
      str_val(),
      seq_val(new TSeq_arg(std :: move(_seq))),
      code_seg() {}
Package :: Package(const std::__cxx11::string &_title, int _l, int _r)
    :
      int_val(),
      str_val(),
      seq_val(),
      code_seg(new TFunction(_title, _l, _r)) {}
Package & Package :: operator = (const Package & rhs)
{
    this -> int_val = rhs.int_val;
    this -> str_val = rhs.str_val;
    this -> code_seg = rhs.code_seg;
    this -> seq_val = rhs.seq_val;
    return *this;
}
const Package & Package :: operator[](size_t idx) const
{
    if (seq_val != NULL)
    {
        return seq_val -> at(idx);
    }
    Error.message("Fatal : Trying to access a non-sequece object through index.");
}

Package :: Package(int _l, int _r)
    :
      int_val(),
      str_val(),
      seq_val(),
      code_seg(new TFunction(_l, _r)) {}
Package :: ~Package() {}
bool Package :: empty() const
{
    return int_val == NULL && str_val == NULL && seq_val == NULL && code_seg == NULL;
}
ostream & operator <<(ostream & fout, const Package & rhs)
{
    if (rhs.int_val != NULL)
        fout << *rhs.int_val;
    if (rhs.str_val != NULL)
        fout << *rhs.str_val;
    if (rhs.seq_val != NULL)
    {
        deque<Package> & tg = *rhs.seq_val;
        for (auto i = tg.begin(); i != tg.end(); ++i)
        {
            fout << *i;
        }
    }
    if (rhs.code_seg != NULL)
    {
        fout << "<#procedure# " << rhs.code_seg -> title;
        fout << " ranging from " << rhs.code_seg -> l << " to " << rhs.code_seg -> r << ">";
    }
    return fout;
}


ostream & operator <<(ostream &fout, const TScanner :: TToken & rhs)
{
    switch(rhs.token_name)
    {
        case TScanner :: id :
            fout << "<identifier #" << Scanner.seq_identifier[rhs.attribute_value] << "#> ";
        break;
        case TScanner :: immediate_int :
            fout << "<immediate number " << Scanner.seq_imm_int[rhs.attribute_value] << "> ";
        break;
        case TScanner :: immediate_str :
            fout << "<immediate str \"" << Scanner.seq_imm_str[rhs.attribute_value] << "\"> ";
        break;
        case TScanner :: assign :
            fout << "<assign ";
            switch (rhs.attribute_value)
            {
                case _round_bracket_l :
                    fout << "\"(\"";
                break;
                case _round_bracket_r :
                    fout << "\")\"";
                break;
                case _rect_bracket_l :
                    fout << "\"[\"";
                break;
                case _rect_bracket_r :
                    fout << "\"]\"";
                break;
                case _italian_bracket_l :
                    fout << "\"{\"";
                break;
                case _italian_bracket_r :
                    fout << "\"}\"";
                break;
            }
            fout << "> ";
        break;
    }
    return fout;
}


TParser :: TParser()
{
    keyword_vtable["print"] = new TProcessor_print();
    keyword_vtable["def"]   = new TProcessor_def();
    keyword_vtable["static_def"] = new TProcessor_static_def();
    keyword_vtable["lambda"] = new TProcessor_lambda();
    keyword_vtable["function"] = new TProcessor_function();
    keyword_vtable["arg"]   = new TProcessor_arg();
    keyword_vtable["cond"]  = new TProcessor_cond();
    keyword_vtable["?"]     = new TProcessor_cond();
    keyword_vtable["<"]     = new TProcessor_less();
    keyword_vtable["<="]    = new TProcessor_lesseq();
    keyword_vtable[">"]     = new TProcessor_greater();
    keyword_vtable[">="]    = new TProcessor_greatereq();
    keyword_vtable["!="]    = new TProcessor_ineq();
    keyword_vtable["=="]    = new TProcessor_eq();
    keyword_vtable["eq"]    = new TProcessor_eq();
    keyword_vtable["+"]     = new TProcessor_add();
    keyword_vtable["-"]     = new TProcessor_sub();
    keyword_vtable["*"]     = new TProcessor_mul();
    keyword_vtable["/"]     = new TProcessor_div();
    keyword_vtable["link"]  = new TProcessor_link();
    keyword_vtable["substr"] = new TProcessor_substr();
    keyword_vtable["nextInt"] = new TProcessor_nextInt();
    keyword_vtable["nextStr"] = new TProcessor_nextStr();
    keyword_vtable["cons"]  = new TProcessor_cons();
    keyword_vtable["push_back"] = new TProcessor_push_back();
    keyword_vtable["push_front"] = new TProcessor_push_front();
    keyword_vtable["makeseq"]   = new TProcessor_makeseq();
    keyword_vtable["subseq"]    = new TProcessor_subseq();
    keyword_vtable["at"]    = new TProcessor_at();
}
TParser :: ~TParser()
{
    for (auto i = keyword_vtable.begin(); i != keyword_vtable.end(); ++i)
    {
        delete (i -> second);
    }
}

void TParser :: rebind(deque<TScanner :: TToken> &target)
{
    generated_tokens = &target;
}

Package & TParser :: get_arg(size_t idx)
{
    return arg_stack[arg_stack.size() - 1][idx];
}

Package TParser :: execute(int & pos)
{
    deque<TScanner :: TToken> & lst(*generated_tokens);
    switch (lst[pos].token_name)
    {
        case TScanner :: assign :
            if (lst[pos] == token_round_bracket_l || lst[pos] == token_rect_bracket_l || lst[pos] == token_italian_bracket_l)
            {
                auto tmp = execute(++pos);
                if (tmp.code_seg == NULL || tmp.code_seg -> title == "")
                   Error.message("Invalid expression occured.");
                string title = tmp.code_seg -> title;
                auto target = keyword_vtable.find(title);
                if (target != keyword_vtable.end())
                {
                    TProcessor * op = target -> second;
                    auto t = op -> proc(pos, lst);
                    return t;
                } else {
                    TSeq_arg in_pending;
                    Package next;
                    while (true)
                    {
                        next = execute(pos);
                        if (next.empty())
                            break;
                     //   std :: cerr << next << ' ';
                        in_pending.push_back(next);
                    }
                    //std :: cerr << "for function #" << title << "#" << std :: endl;
                    arg_stack.push_back(in_pending);
                    int p;
                    Package ret_p;
                    bool is_static;
                    bool is_renewable = true;
                    long long x;
                    long long y;
                    if (tmp.code_seg -> l == 0 && tmp.code_seg -> r == 0)
                    {
                        auto tg = symbol_table.find(title);
                        if (tg == symbol_table.end())
                            Error.message("Invalid operator #" + title + "# found.");
                        auto static_flag = static_def_list.find(title);
                        is_static = (static_flag != static_def_list.end());
                        x = (in_pending.size() > 0 && in_pending[0].int_val != NULL) ? (*in_pending[0].int_val) : 0;
                        y = (in_pending.size() > 1 && in_pending[1].int_val != NULL) ? (*in_pending[1].int_val) : 0;
                        if (is_static)
                        {
                            if (in_pending[0].int_val != NULL)
                            {
                                auto saved_valuex = static_flag -> second.find(*in_pending[0].int_val);
                                if (saved_valuex != static_flag -> second.end())
                                {
                                    auto saved_value = saved_valuex -> second.find(y);
                                    if (saved_value != saved_valuex -> second.end())
                                    {
                                        is_renewable = false;
                                        ret_p = saved_value -> second;
                                    }
                                }
                            } else {
                                Error.message("Fatal : A non-staticizeable function is under such a attempt.");
                            }
                        }
                        p = tg -> second.l;
                    } else {
                        p = tmp.code_seg -> l;
                    }
                    if (ret_p.empty())
                        ret_p = execute(p);
                    if (is_static && is_renewable)
                    {
                        static_def_list[title][x][y] = ret_p;
                    }
                    arg_stack.pop_back();
                    return ret_p;
                }
            } else {
                pos++;
                return Package();
            }
        break;
        case TScanner :: id :
            ++pos;
            if (arg_symbol_stack.empty() || arg_symbol_stack[arg_symbol_stack.size() - 1].find(Scanner.seq_identifier[lst[pos - 1].attribute_value]) == arg_symbol_stack[arg_symbol_stack.size() - 1].end())
                return Package(Scanner.seq_identifier[lst[pos - 1].attribute_value], 0, 0);
            else
                return get_arg(arg_symbol_stack[arg_symbol_stack.size() - 1][Scanner.seq_identifier[lst[pos - 1].attribute_value]]);
        break;
        case TScanner :: immediate_int :
            return Package(Scanner.seq_imm_int[lst[pos++].attribute_value]);
        break;
        case TScanner :: immediate_str :
            return Package(Scanner.seq_imm_str[lst[pos++].attribute_value]);
        break;
    }
    return Package();
}
