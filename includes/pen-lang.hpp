#ifndef PENLANG_HPP
#define PENLANG_HPP
#include "pen-interfaces.hpp"
#include <fstream>
using std :: ifstream;
using std :: ofstream;
using std :: istream;
using std :: ostream;

class TError
{
public:
    class TMessage
    {
    public:
        void operator()(const string & _details);
    } message;
    class TDebugMessage
    {
    public:
        void operator()(const string &_details);
    } debug;
};

class TFile
{
public:
    void fopen(const string & path);
    void fopen(istream & src);
    deque<string> inside;
};
class TFunction
{
public:
    string title;
    int l, r;
    TFunction();
    TFunction(const string & _title, int _l, int _r);
    TFunction(int _l, int _r);
};

class Package
{
public:
    shared_ptr<string> str_val;
    shared_ptr<long long> int_val;
    shared_ptr<deque<Package>> seq_val;
    shared_ptr<TFunction> code_seg;
    Package() = default;
    Package(const Package & rhs) = default;
    Package(Package && rhs) = default;
    Package(const string & _str);
    Package(long long _int);
    Package(const TSeq_arg & _seq);
    Package(TSeq_arg && _seq);
    Package(const string & _title, int _l, int _r);
    Package(int _l, int _r);
    ~Package();
    Package & operator =(const Package & rhs);
    const Package & operator [](size_t idx) const;
    bool empty() const;
};

ostream & operator <<(ostream &fout, const Package & rhs);

class TScanner
{
public:
    class TToolkit
    {
    public:
        string str(long long x);
        long long val(const string & x);
        string eschar_reinterpret(const string & src);
    } tools;

    enum TToken_name
    {
        id, immediate_int, immediate_str, assign
    };
    class TToken
    {
    public:
        TToken_name token_name;
        size_t attribute_value;
        TToken();
        TToken(TToken_name _token_name, size_t _attribute_value);
        bool operator==(const TToken & rhs) const;
        friend ostream & operator <<(ostream &fout, const TToken & rhs);
    };

    deque<string> seq_imm_str, seq_identifier;
    deque<long long> seq_imm_int;
    deque<TToken> lexemes;

    void append(const string & src);
    void proc_import();
};
const TScanner :: TToken token_round_bracket_l = TScanner :: TToken(TScanner :: assign, _round_bracket_l);
const TScanner :: TToken token_round_bracket_r = TScanner :: TToken(TScanner :: assign, _round_bracket_r);
const TScanner :: TToken token_rect_bracket_l = TScanner :: TToken(TScanner :: assign, _rect_bracket_l);
const TScanner :: TToken token_rect_bracket_r = TScanner :: TToken(TScanner :: assign, _rect_bracket_r);
const TScanner :: TToken token_italian_bracket_l = TScanner :: TToken(TScanner :: assign, _italian_bracket_l);
const TScanner :: TToken token_italian_bracket_r = TScanner :: TToken(TScanner :: assign, _italian_bracket_r);

#define DEF(x) class x : public TProcessor\
               { \
               public: \
                    Package proc(int & pos, deque<TScanner :: TToken> & lexemes); \
               }

ostream & operator <<(ostream &fout, const TScanner :: TToken & rhs);

class TParser
{
public:
    class TProcessor;
    class TProcessor_print;
    class TProcessor_macro;
    class TProcessor_def;
    class TProcessor_lambda;
    class TProcessor_function;
    class TProcessor_plain;
    class TProcessor_new;
    class TProcessor_var;
    class TProcessor_arg;
    class TProcessor_cast;
    class TProcessor_comma;
    class TProcessor_asm;
    class TProcessor_constexpr;
    class TProcessor_rawasm;
    class TProcessor_deref;
    class TProcessor_cond;
    class TProcessor_eq;
    class TProcessor_let;
    class TProcessor_less;
    class TProcessor_lesseq;
    class TProcessor_greater;
    class TProcessor_greatereq;
    class TProcessor_ineq;
    class TProcessor_add;
    class TProcessor_sub;
    class TProcessor_mul;
    class TProcessor_div;
    class TProcessor_link;
    class TProcessor_substr;
    class TProcessor_nextInt;
    class TProcessor_nextStr;
    class TProcessor_length;
    class TProcessor_at;
    class TProcessor_makeseq;
    class TProcessor_push_front;
    class TProcessor_push_back;
    class TProcessor_subseq;
    class TProcessor_cons;
protected:
    deque<TScanner :: TToken> * generated_tokens;
    unordered_map<string, TFunction> symbol_table;
    unordered_map<string, TProcessor *> keyword_vtable;
    unordered_map<string, TState> static_def_list;
    unordered_map<string, string> var_table;
    deque<Package> initializer_stack;
    deque<TSeq_arg> arg_stack;
    deque<TPro_table> arg_symbol_stack;
public:
    class TProcessor
    {
    public:
        virtual Package proc(int & pos, deque<TScanner :: TToken> & lexemes) = 0;
    };

    DEF(TProcessor_print);
    DEF(TProcessor_macro);
    DEF(TProcessor_def);
    DEF(TProcessor_lambda);
    DEF(TProcessor_function);
    DEF(TProcessor_plain);
    DEF(TProcessor_new);
    DEF(TProcessor_var);
    DEF(TProcessor_constexpr);
    DEF(TProcessor_arg);
    DEF(TProcessor_asm);
    DEF(TProcessor_deref);
    DEF(TProcessor_cast);
    DEF(TProcessor_rawasm);
    DEF(TProcessor_comma);
    DEF(TProcessor_cond);
    DEF(TProcessor_eq);
    DEF(TProcessor_let);
    DEF(TProcessor_less);
    DEF(TProcessor_lesseq);
    DEF(TProcessor_greater);
    DEF(TProcessor_greatereq);
    DEF(TProcessor_ineq);
    DEF(TProcessor_add);
    DEF(TProcessor_sub);
    DEF(TProcessor_mul);
    DEF(TProcessor_div);
    DEF(TProcessor_link);
    DEF(TProcessor_substr);
    DEF(TProcessor_nextInt);
    DEF(TProcessor_nextStr);
    DEF(TProcessor_length);
    DEF(TProcessor_at);
    DEF(TProcessor_makeseq);
    DEF(TProcessor_push_front);
    DEF(TProcessor_push_back);
    DEF(TProcessor_subseq);
    DEF(TProcessor_cons);
    TParser();
    ~TParser();
    Package execute(int & pos);
    Package & get_arg(size_t idx);
    void rebind(deque<TScanner :: TToken> & target);
};

class TEncoder
{
    ofstream fout;
public:
    void call(Package func);
    void bind(const std::string & path);
    void output_title(const std::string &title);
    void output(Package rhs);
    void comma();
    void nextln();
    void encode_escchar_output(const std::string & title);
    void encode_func(TFunction func);
    void close();
};

#endif // PENLANG_HPP
