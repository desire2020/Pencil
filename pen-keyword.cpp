#include "./includes/pen-utility.hpp"

Package TParser :: TProcessor_print :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    while (true)
    {
        next = Parser.execute(pos);
        if (next.empty())
            break;
        std :: cout << next;
        std :: cout.flush();
    }
    return Package(0);
}
Package TParser :: TProcessor_macro :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package title;
    Package content;
    if (pos >= lexemes.size())
        return Package();
    title = std :: move(Parser.execute(pos));
    content = std :: move(Parser.execute(pos));
    ++pos;
    if (title.code_seg == NULL || content.code_seg == NULL)
        Error.message("Invalid arguments specified for <procedure #def#>.");
    Parser.symbol_table[title.code_seg -> title] = TFunction(title.code_seg -> title, content.code_seg -> l, content.code_seg -> r);
    return Package(0);
}
Package TParser :: TProcessor_def :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package title;
    Package content;
    if (pos >= lexemes.size())
        return Package();
    title = std :: move(Parser.execute(pos));
    content = std :: move(Parser.execute(pos));
    ++pos;
    if (title.code_seg == NULL || content.code_seg == NULL)
        Error.message("Invalid arguments specified for <procedure #def#>.");
    Parser.symbol_table[title.code_seg -> title] = TFunction(title.code_seg -> title, content.code_seg -> l, content.code_seg -> r);
    Parser.static_def_list[title.code_seg -> title] = TState();
    Encoder.encode_func(TFunction(title.code_seg -> title, content.code_seg -> l, content.code_seg -> r));
    return Package(0);
}
Package TParser :: TProcessor_new :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    next = std :: move(Parser.execute(pos));
    if (next.empty())
    {
        Error.message("Too few arguments specified for <procedure #var#>");
    }
    Encoder.encode_escchar_output(next.code_seg->title + " ");
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
    }
    return Package(0);
}
Package TParser :: TProcessor_deref :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package label;
    if (pos >= lexemes.size())
        return Package();
    label = Parser.execute(pos);
    Parser.execute(pos);
    return Package("[" + label.code_seg -> title + "]", 0, 0);
}
Package TParser :: TProcessor_cast :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package type, target;
    if (pos >= lexemes.size())
        return Package();
    type = Parser.execute(pos);
    target = Parser.execute(pos);
    Parser.execute(pos);
    return Package(type.code_seg -> title + " " + target.code_seg -> title, 0, 0);
}

Package TParser :: TProcessor_constexpr :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package title;
    if (pos >= lexemes.size())
        return Package();
    title = Parser.execute(pos);
    Parser.symbol_table[title.code_seg -> title] = TFunction(title.code_seg -> title, pos, pos + 1);
    Parser.execute(pos);
    Parser.execute(pos);
    return Package(0);
}

Package TParser :: TProcessor_comma :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    bool flag = false;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        if (flag)
            Encoder.comma();
        flag = true;
        Encoder.output(next);
    }
    Encoder.nextln();
    return Package(0);
}
Package TParser :: TProcessor_let :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package id, itv;
    int p = Parser.initializer_stack.size();
    id = Parser.execute(pos);
    itv = Parser.execute(pos);
    Parser.execute(pos);
    Parser.initializer_stack.push_back(std::move(itv));
    return Package(id.code_seg -> title, -1, p);
}
string get_type_instr(const string & title)
{
    if (title == "byte" || title == "string")
        return "db";
    else if (title == "halfword")
        return "dw";
    else if (title == "word")
        return "dd";
    else
        Error.message("Fatal : unsupported type " + title + "specified.");
}
Package TParser :: TProcessor_var :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package type, id;
    deque<Package> todo;
    string title;
    while (true)
    {
        id = Parser.execute(pos);
        title = id.code_seg -> title;
        if (id.code_seg -> title == ":")
            break;
        todo.push_back(id);
    }
    type = Parser.execute(pos);
    Parser.execute(pos);
    for (auto i = todo.begin(); i != todo.end(); ++i)
    {
        if (Parser.var_table.find(i -> code_seg -> title) != Parser.var_table.end())
            Error.message("Fatal : Varible " + i -> code_seg -> title + " redefined.");
        Parser.var_table[i -> code_seg -> title] = type.code_seg -> title;
        Package init;
        if (i -> code_seg -> l == -1)
        {
            init = Parser.initializer_stack[i -> code_seg -> r];
        } else {
            init = Package(0);
        }
        Encoder.output_title(i -> code_seg -> title);
        Encoder.output_title(get_type_instr(type.code_seg -> title));
        Encoder.output(init);
        Encoder.nextln();
    }
    Parser.initializer_stack.clear();
    return Package(0);
}

Package TParser :: TProcessor_asm :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    next = Parser.execute(pos);
    if (!Parser.execute(pos).empty())
        Error.message("Too many instruction title specified for <procedure #asm>!");
    Encoder.output_title(next.code_seg->title);
    return Package("comma", 0, 0);
}
Package TParser :: TProcessor_rawasm :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    next = Parser.execute(pos);
    if (!Parser.execute(pos).empty())
        Error.message("Too many instruction title specified for <procedure #__asm__>!");
    Encoder.encode_escchar_output(*next.str_val);
    return Package(0);
}

Package TParser :: TProcessor_lambda :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    int l = pos;
    int sign = 1;
    for (++pos; sign != 0; ++pos)
    {
        if (lexemes[pos] == token_italian_bracket_l || lexemes[pos] == token_rect_bracket_l || lexemes[pos] == token_round_bracket_l)
        {
            ++sign;
        } else {
            if (lexemes[pos] == token_italian_bracket_r || lexemes[pos] == token_rect_bracket_r || lexemes[pos] == token_round_bracket_r)
            {
                --sign;
            }
        }
    }
    ++pos;
    return Package("__lambda__", l, pos);
}
Package TParser :: TProcessor_function :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Parser.arg_symbol_stack.push_back(TPro_table());
    Package next;
    Package copy;
    int no = 0;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (!next.empty())
            copy = next;
        else
            break;
        if (next.code_seg != NULL)
        {
            Parser.arg_symbol_stack[Parser.arg_symbol_stack.size() - 1][next.code_seg -> title] = no++;
        }
    }
    Parser.arg_symbol_stack.pop_back();
    return copy;
}
Package TParser :: TProcessor_plain :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    bool flag = false;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        flag = true;
        Encoder.output(next);
    }
    return Package(0);
}

Package TParser :: TProcessor_arg :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    next = std :: move(Parser.execute(pos));
    Parser.execute(pos);
    next = Parser.get_arg(*next.int_val);
    if (next.int_val != NULL)
        return Package(*next.int_val);
    if (next.str_val != NULL)
        return Package(*next.str_val);
    if (next.seq_val != NULL)
        return Package(*next.seq_val);
    if (next.code_seg != NULL)
        return Package(next.code_seg->title, next.code_seg->l, next.code_seg->r);
    return Package();
}
Package TParser :: TProcessor_cond :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    next = std :: move(Parser.execute(pos));
    if (next.empty())
    {
        Error.message("Too few arguments specified for <procedure #?#>");
    }
    ans = *next.int_val;
    if (ans != 0)
    {
        next = std :: move(Parser.execute(pos));
        if (lexemes[pos].token_name == TScanner :: assign)
        {
            int l = pos;
            int sign = 1;
            for (++pos; sign != 0; ++pos)
            {
                if (lexemes[pos] == token_italian_bracket_l || lexemes[pos] == token_rect_bracket_l || lexemes[pos] == token_round_bracket_l)
                {
                    ++sign;
                } else {
                    if (lexemes[pos] == token_italian_bracket_r || lexemes[pos] == token_rect_bracket_r || lexemes[pos] == token_round_bracket_r)
                    {
                        --sign;
                    }
                }
            }
        } else {
            Parser.execute(pos);
            ++pos;
        }
    } else {
        if (lexemes[pos].token_name == TScanner :: assign)
        {
            int l = pos;
            int sign = 1;
            for (++pos; sign != 0; ++pos)
            {
                if (lexemes[pos] == token_italian_bracket_l || lexemes[pos] == token_rect_bracket_l || lexemes[pos] == token_round_bracket_l)
                {
                    ++sign;
                } else {
                    if (lexemes[pos] == token_italian_bracket_r || lexemes[pos] == token_rect_bracket_r || lexemes[pos] == token_round_bracket_r)
                    {
                        --sign;
                    }
                }
            }
        } else {
            Parser.execute(pos);
        }
        next = std :: move(Parser.execute(pos));
        ++pos;
    }
    if (next.int_val != NULL)
        return Package(*next.int_val);
    if (next.str_val != NULL)
        return Package(*next.str_val);
    if (next.seq_val != NULL)
        return Package(*next.seq_val);
    if (next.code_seg != NULL)
        return Package(next.code_seg->title, next.code_seg->l, next.code_seg->r);
    return Package(0);
}
#define REG_LOGI(processor_title, x) Package TParser :: processor_title :: proc(int & pos, deque<TScanner :: TToken> & lexemes) \
                    { \
                        Package a1; \
                        Package a2; \
                        if (pos >= lexemes.size()) \
                            return Package(0); \
                        a1 = std :: move(Parser.execute(pos)); \
                        Package ret_v(1); \
                        while (true) \
                        { \
                            a2 = std :: move(Parser.execute(pos)); \
                            if (a2.empty()) \
                            { \
                                break; \
                            } \
                            if (a1.int_val != NULL) \
                            { \
                                if (a2.int_val == NULL || !((*a1.int_val) x (*a2.int_val))) \
                                    ret_v = Package(0); \
                            } \
                            if (a1.str_val != NULL) \
                            { \
                                if (a2.str_val == NULL || !((*a1.str_val) x (*a2.str_val))) \
                                    ret_v = Package(0); \
                            } \
                        } \
                        return ret_v; \
                    }
REG_LOGI(TProcessor_eq, ==)
REG_LOGI(TProcessor_less, <)
REG_LOGI(TProcessor_lesseq, <=)
REG_LOGI(TProcessor_greater, >)
REG_LOGI(TProcessor_greatereq, >=)
REG_LOGI(TProcessor_ineq, !=)

Package TParser :: TProcessor_add :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        ans += *next.int_val;
    }
    return Package(ans);
}
Package TParser :: TProcessor_sub :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    next = std :: move(Parser.execute(pos));
    if (next.empty())
    {
        Error.message("Too few arguments specified for <procedure #-#>");
    }
    ans = *next.int_val;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        ans -= *next.int_val;
    }
    return Package(ans);
}
Package TParser :: TProcessor_mul :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 1;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        ans *= *next.int_val;
    }
    return Package(ans);
}
Package TParser :: TProcessor_div :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    next = std :: move(Parser.execute(pos));
    if (next.empty())
    {
        Error.message("Too few arguments specified for <procedure #/#>");
    }
    ans = *next.int_val;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        ans /= *next.int_val;
    }
    return Package(ans);
}
Package TParser :: TProcessor_link :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    string ans = "";
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        ans += *next.str_val;
    }
    return Package(ans);
}
Package TParser :: TProcessor_substr :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package src;
    Package spos, n;
    if (pos >= lexemes.size())
        return Package();
    src = std :: move(Parser.execute(pos));
    if (src.str_val == NULL)
        Error.message("Invalid argument specified for <procedure #substr#>");
    spos = std :: move(Parser.execute(pos));
    n = std :: move(Parser.execute(pos));
    if (!Parser.execute(pos).empty())
        Error.message("Too many arguments specified for <procedure #substr#>");
    string str_src = *(src.str_val);
    long long a(*spos.int_val), b(*n.int_val);
    return Package(str_src.substr(a, b));
}
Package TParser :: TProcessor_nextInt :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        std :: cin >> ans;
        if (next.empty())
        {
            break;
        }
    }
    return Package(ans);
}
Package TParser :: TProcessor_nextStr :: proc(int & pos, deque<TScanner :: TToken> & lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    string ans;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        std :: getline(std :: cin, ans);
        if (next.empty())
        {
            break;
        }
    }
    return Package(ans);
}
Package TParser :: TProcessor_length :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    long long ans;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
            break;
        if (next.seq_val != 0)
            ans = next.seq_val -> size();
        else
            if (next.str_val != 0)
                ans = next.str_val -> length();
    }
    return Package(ans);
}

Package TParser :: TProcessor_at :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package bg, context;
    Package idx;
    if (pos >= lexemes.size())
        return Package();
    long long ans = 0;
    bg = std :: move(Parser.execute(pos));
    context = bg;
    if (context.seq_val == NULL)
        Error.message("Invalid context specified for <procedure #at#>.");
    while (true)
    {
        idx = std :: move(Parser.execute(pos));
        if (idx.empty())
        {
            break;
        }
        ans = *idx.int_val;
        context = context[ans];
    }
    return context;
}
Package TParser :: TProcessor_makeseq :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package next;
    if (pos >= lexemes.size())
        return Package();
    TSeq_arg tmp;
    long long ans = 0;
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        tmp.push_back(std :: move(next));
    }
    return Package(std :: move(tmp));
}
Package TParser :: TProcessor_subseq :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package bg;
    Package idx, n;
    if (pos >= lexemes.size())
        return Package();
    bg = std :: move(Parser.execute(pos));
    if (bg.seq_val == NULL)
        Error.message("Invalid context specified for <procedure #subseq#>.");
    TSeq_arg & context(*bg.seq_val);
    idx = std :: move(Parser.execute(pos));
    n = std :: move(Parser.execute(pos));
    TSeq_arg tmp;
    if (*idx.int_val >= context.size())
        return std :: move(tmp);
    int counter = *n.int_val;
    for (auto i = context.begin() + (*idx.int_val); i != context.end() && counter > 0; ++i, --counter)
    {
        tmp.push_back(*i);
    }
    return Package(std :: move(tmp));
}
Package TParser :: TProcessor_push_back :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package bg;
    Package n;
    if (pos >= lexemes.size())
        return Package();
    bg = std :: move(Parser.execute(pos));
    if (bg.seq_val == NULL)
        Error.message("Invalid context specified for <procedure #subseq#>.");
    TSeq_arg context(*bg.seq_val);
    n = std :: move(Parser.execute(pos));
    context.push_back(std :: move(n));
    return Package(std :: move(context));
}
Package TParser :: TProcessor_push_front :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    Package bg;
    Package n;
    if (pos >= lexemes.size())
        return Package();
    bg = std :: move(Parser.execute(pos));
    if (bg.seq_val == NULL)
        Error.message("Invalid context specified for <procedure #subseq#>.");
    TSeq_arg context(*bg.seq_val);
    n = std :: move(Parser.execute(pos));
    context.push_front(std :: move(n));
    return Package(std :: move(context));
}
Package TParser :: TProcessor_cons :: proc(int &pos, deque<TScanner::TToken> &lexemes)
{
    TSeq_arg tmp;
    Package next;
    if (pos >= lexemes.size())
        return Package();
    while (true)
    {
        next = std :: move(Parser.execute(pos));
        if (next.empty())
        {
            break;
        }
        TSeq_arg & context(*next.seq_val);
        for (auto i = context.begin(); i != context.end(); ++i)
        {
            tmp.push_back(*i);
        }
    }
    return Package(std :: move(tmp));
}
