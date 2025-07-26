#undef _GLIBCXX_DEBUG

#pragma GCC optimize( \
    "Ofast,inline,unroll-loops,unswitch-loops,loop-interchange,loop-strip-mine,loop-block,move-loop-invariants,loop-strip-mine,predictive-commoning,if-conversion,peephole,auto-inc-dec,reorder-blocks,reorder-blocks-algorithm=stc,reorder-functions,align-functions,align-jumps,align-loops,align-labels,no-stack-protector,no-defer-pop,omit-frame-pointer")
#pragma GCC target("arch=tigerlake")

#include <bits/stdc++.h>

#ifdef _WIN32
#define getchar _getchar_nolock
#define putchar _putchar_nolock
#else
#define getchar getchar_unlocked
#define putchar putchar_unlocked
#endif

using namespace std;

template <typename T>
struct is_container_like
{
    template <typename U>
    static auto test(U *)
        -> decltype(declval<U>().begin(), declval<U>().end(), true_type{});
    template <typename U>
    static false_type     test(...);
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

inline int getChar()
{
    return getchar();
}
inline void putChar(char c)
{
    putchar(c);
}

template <typename T>
void putNum(const T &_x, int _prec = -1);

inline void putStr(const string &_s);

template <typename T>
inline void getNum(T &_var)
{
    _var = 0;
    int _c, _neg = 0;
    _c = getChar();
    while (_c != '-' && (_c < '0' || _c > '9')) _c = getChar();
    if (_c == '-')
    {
        _neg = 1;
        _c   = getChar();
    }
    while (_c >= '0' && _c <= '9')
    {
        _var = _var * 10 + (_c - '0');
        _c   = getChar();
    }
    if constexpr (is_floating_point_v<T>)
    {
        if (_c == '.')
        {
            long double _frac = 0, _div = 1;
            _c = getChar();
            while (_c >= '0' && _c <= '9')
            {
                _frac = _frac * 10 + (_c - '0');
                _div *= 10;
                _c = getChar();
            }
            _var += _frac / _div;
        }
    }
    _var = (_neg ? -_var : _var);
}

template <typename T>
inline void getStr(T &_s)
{
    _s.clear();
    int _c;
    while ((_c = getChar()) <= ' ' && _c != EOF);
    while (_c > ' ')
    {
        _s.push_back(static_cast<typename T::value_type>(_c));
        _c = getChar();
    }
}

template <typename T>
inline void getLine(T &_s)
{
    _s.clear();
    int _c;
    while ((_c = getChar()) != '\n' && _c != EOF)
    {
        _s.push_back(static_cast<typename T::value_type>(_c));
    }
}

inline void get()
{
}

template <typename T, typename... Ts>
inline void get(T &head, Ts &...tail)
{
    if constexpr (is_integral_v<T> && !is_same_v<T, char>) { getNum(head); }
    else if constexpr (is_floating_point_v<T>) { getNum(head); }
    else { getStr(head); }
    get(tail...);
}

struct _sep
{
    string value;
    _sep(char c) : value(1, c)
    {
    }
    _sep(const char *val) : value(val)
    {
    }
    _sep(const string &val) : value(val)
    {
    }
};

struct _end
{
    string value;
    _end(const char *val) : value(val)
    {
    }
    _end(const string &val) : value(val)
    {
    }
};

template <typename T>
inline void put_single_value(const T &head);

template <typename T>
inline void putNum(const T &_x, int _prec)
{
    if constexpr (is_integral_v<T>)
    {
        if (_x == 0)
        {
            putChar('0');
            return;
        }
        if constexpr (is_signed_v<T>)
        {
            if (_x < 0)
            {
                putChar('-');
                putNum(-_x, -1);
                return;
            }
        }
        char _buf[40];
        int  _len = 0;
        T    temp = _x;
        while (temp)
        {
            _buf[_len++] = char('0' + temp % 10);
            temp /= 10;
        }
        while (_len--) putChar(_buf[_len]);
    }
    else if constexpr (is_floating_point_v<T>)
    {
        if (isnan(_x))
        {
            putStr("nan");
            return;
        }
        if (isinf(_x))
        {
            putStr("inf");
            return;
        }

        long double _y = static_cast<long double>(_x);
        if (_y < 0)
        {
            putChar('-');
            _y = -_y;
        }
        long long   _ip = (long long)_y;
        long double _fp = _y - _ip;
        putNum(_ip, -1);

        if (_prec == -1)
        {
            if (abs(_fp) > 1e-18L)
            {
                putChar('.');
                for (int _i = 0; _i < 6 && abs(_fp) > 1e-18L; ++_i)
                {
                    _fp *= 10;
                    int _d = (int)(_fp + 1e-18L);
                    putChar(char('0' + _d));
                    _fp -= _d;
                }
            }
        }
        else if (_prec > 0)
        {
            putChar('.');
            for (int _i = 0; _i < _prec; ++_i)
            {
                _fp *= 10;
                int _d = (int)(_fp + 1e-18L);
                putChar(char('0' + _d));
                _fp -= _d;
            }
        }
    }
}

inline void putStr(const string &_s)
{
    for (const auto &_c : _s) putChar(_c);
}

template <typename T>
inline void put_single_value(const T &head)
{
    if constexpr (is_same_v<T, char>) { putChar(head); }
    else if constexpr (is_integral_v<T> && !is_same_v<T, char>)
    {
        putNum(head, -1);
    }
    else if constexpr (is_floating_point_v<T>) { putNum(head, -1); }
    else if constexpr (is_same_v<T, string>) { putStr(head); }
    else if constexpr (is_container_like<T>::value)
    {
        putChar('[');
        bool first = true;
        for (const auto &val : head)
        {
            if (!first) putStr(", ");
            put_single_value(val);
            first = false;
        }
        putChar(']');
    }
    else if constexpr (is_same_v<T, const char *> || is_same_v<T, char *>)
    {
        for (const char *s = head; *s; ++s) putChar(*s);
    }
    else
    {
        if constexpr (is_same_v<remove_cvref_t<T>,
                                pair<typename T::first_type,
                                     typename T::second_type>>)
        {
            putChar('(');
            put_single_value(head.first);
            putStr(", ");
            put_single_value(head.second);
            putChar(')');
        }
        else if constexpr (is_same_v<remove_cvref_t<T>,
                                     map<typename T::key_type,
                                         typename T::mapped_type>>)
        {
            putChar('{');
            bool first = true;
            for (const auto &p : head)
            {
                if (!first) putStr(", ");
                put_single_value(p.first);
                putStr(": ");
                put_single_value(p.second);
                first = false;
            }
            putChar('}');
        }
        else
        {
            for (const auto &c : head) { putChar(c); }
        }
    }
}

template <size_t N>
inline void put_single_value(const char (&str)[N])
{
    for (size_t i = 0; i < N - 1; ++i) { putChar(str[i]); }
}

template <typename T>
bool is_newline_arg(const T &arg)
{
    if constexpr (is_same_v<T, char>) { return arg == '\n'; }
    else if constexpr (is_same_v<T, const char *> || is_same_v<T, string>)
    {
        return string(arg).find('\n') != string::npos;
    }
    return false;
}

template <typename... Args, size_t... I>
void put_recursive(const tuple<Args...> &tup, const string &current_sep,
                   index_sequence<I...>)
{
    bool is_first_on_line = true;
    (
        [&]
        {
            if constexpr (!is_same_v<remove_cvref_t<decltype(get<I>(tup))>,
                                     _sep> &&
                          !is_same_v<remove_cvref_t<decltype(get<I>(tup))>,
                                     _end>)
            {
                if (is_newline_arg(get<I>(tup)))
                {
                    put_single_value(get<I>(tup));
                    is_first_on_line = true;
                }
                else
                {
                    if (!is_first_on_line) { putStr(current_sep); }
                    put_single_value(get<I>(tup));
                    is_first_on_line = false;
                }
            }
        }(),
        ...);
}

template <typename... Args, size_t... I>
void process_params(const tuple<Args...> &tup, string &current_sep,
                    string &end_val, index_sequence<I...>)
{
    (
        [&]
        {
            if constexpr (is_same_v<remove_cvref_t<decltype(get<I>(tup))>,
                                    _sep>)
            {
                current_sep = get<I>(tup).value;
            }
            if constexpr (is_same_v<remove_cvref_t<decltype(get<I>(tup))>,
                                    _end>)
            {
                end_val = get<I>(tup).value;
            }
        }(),
        ...);
}

template <typename... Ts>
void put(const Ts &...args)
{
    string current_sep = " ";
    string end_val     = "\n";
    auto   tup         = make_tuple(args...);

    process_params(tup, current_sep, end_val,
                   make_index_sequence<sizeof...(Ts)>{});

    put_recursive(tup, current_sep, make_index_sequence<sizeof...(Ts)>{});

    putStr(end_val);
}

#define putd(...)             \
    do {                      \
        putStr(#__VA_ARGS__); \
        putStr(" = ");        \
        put(__VA_ARGS__);     \
    } while (0)

signed main()
{
    __builtin_ia32_ldmxcsr(40896);
    cin.tie(0)->sync_with_stdio(0);
#ifdef LOCAL
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif

    return 0;
}
