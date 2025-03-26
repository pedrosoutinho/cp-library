#include <bits/stdc++.h>
using namespace std;

#define int long long
using ll = long long;
using u32 = uint32_t;
using u64 = uint64_t;
using i128 = __int128;
using u128 = unsigned __int128;
#define str string
using db = long double;

template <class T> constexpr T infty = 0;
template <> constexpr int32_t infty<int32_t> = 1'000'000'000;
template <> constexpr ll infty<ll> = ll(infty<int32_t>) * infty<int32_t> * 2;
template <> constexpr u32 infty<u32> = infty<int32_t>;
template <> constexpr u64 infty<u64> = infty<ll>;
template <> constexpr i128 infty<i128> = i128(infty<ll>) * infty<ll>;
template <> constexpr double infty<double> = infty<ll>;
template <> constexpr long double infty<long double> = infty<ll>;

using pi = pair<int, int>;
using pd = pair<db, db>;
#define mp make_pair
#define fi first
#define se second

#define tcT template <class T
#define tcTU tcT, class U
tcT > using V = vector<T>;
tcT, size_t SZ > using AR = array<T, SZ>;
using vi = V<int>;
using vb = V<bool>;
using vd = V<db>;
using vs = V<string>;
using vpi = V<pi>;
using vpd = V<pd>;
tcT > using pq = priority_queue<T>;
tcT > using pqg = priority_queue<T, vector<T>, greater<T>>;

#define vv(type, name, h, ...) V<V<type>> name(h, V<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...) \
    V<V<V<type>>> name(h, V<V<type>>(w, V<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...) \
    V<V<V<V<type>>>> name(             \
        a, V<V<V<type>>>(b, V<V<type>>(c, V<type>(__VA_ARGS__))))

#define sz(x) (int)((x).size())
#define all(x) begin(x), end(x)
#define bg(x) begin(x)
#define rall(x) x.rbegin(), x.rend()
#define sor(x) sort(all(x))
#define rsz resize
#define ins insert
#define pb push_back
#define eb emplace_back
#define ft front()
#define bk back()

tcT > T POP(deque<T>& que) {
    T a = que.front();
    que.pop_front();
    return a;
}

tcT > T POP(queue<T>& que) {
    T a = que.front();
    que.pop();
    return a;
}

tcT > T POP(pq<T>& que) {
    T a = que.top();
    que.pop();
    return a;
}
tcT > T POP(pqg<T>& que) {
    T a = que.top();
    que.pop();
    return a;
}
tcT > T POP(V<T>& que) {
    T a = que.back();
    que.pop_back();
    return a;
}

#define lb lower_bound
#define ub upper_bound
tcT > int lwb(V<T>& a, const T& b) { return (int)(lb(all(a), b) - bg(a)); }
tcT > int upb(V<T>& a, const T& b) { return (int)(ub(all(a), b) - bg(a)); }

#define each(a, x) for (auto& a : x)
#define rep1(a) for (ll _ = 0; _ < (ll)(a); ++_)
#define rep2(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep3(i, a, b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define rep4(i, a, b, c) for (ll i = (ll)(a); i < (ll)(b); i += (c))
#define cut4(a, b, c, d, e, ...) e
#define rep(...) cut4(__VA_ARGS__, rep4, rep3, rep2, rep1)(__VA_ARGS__)
#define per1(n) for (ll _ = ((ll)n) - 1; _ >= 0; --_)
#define per2(i, n) for (ll i = ((ll)n) - 1; i >= 0; --i)
#define per3(i, a, b) for (ll i = ((ll)b) - 1; i >= (ll)(a); --i)
#define per4(i, a, b, c) for (ll i = ((ll)b) - 1; i >= (ll)(a); i -= (c))
#define per(...) cut4(__VA_ARGS__, per4, per3, per2, per1)(__VA_ARGS__)
#define rep_subset(i, s) \
    for (ll i = (s); i >= 0; i = (i == 0 ? -1 : (i - 1) & (s)))

// up, right, down, left, up-right, up-left, down-right, down-left
const int dx[8]{1, 0, -1, 0, 1, 1, -1, -1}, dy[8]{0, 1, 0, -1, 1, -1, 1, -1};

constexpr int pct(int x) { return __builtin_popcountll(x); }
constexpr int pct_sgn(int x) { return (__builtin_parity(x) & 1 ? -1 : 1); }
constexpr int topbit(int x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); }
constexpr int lowbit(int x) { return x == 0 ? 0 : __builtin_ctzll(x); }
constexpr int mask(int x) { return (1ll << x) - 1; }

int cdiv(int a, int b) { return a / b + ((a ^ b) > 0 && a % b); }

int fdiv(int a, int b) { return a / b - ((a ^ b) < 0 && a % b); }

int root2(int x) {
    int res = sqrtl(x) + 2;
    while (res * res > x) --res;
    return res;
}

tcT > bool ckmin(T& a, const T& b) { return b < a ? a = b, 1 : 0; }

tcT > bool ckmax(T& a, const T& b) { return a < b ? a = b, 1 : 0; }

tcTU > T fstTrue(T lo, T hi, U f) {
    ++hi;
    assert(lo <= hi);
    while (lo < hi) {
        T mid = lo + (hi - lo) / 2;
        f(mid) ? hi = mid : lo = mid + 1;
    }
    return lo;
}

tcTU > T lstTrue(T lo, T hi, U f) {
    --lo;
    assert(lo <= hi);
    while (lo < hi) {
        T mid = lo + (hi - lo + 1) / 2;
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}

tcT > void Unique(vector<T>& v) {
    sort(all(v));
    v.erase(unique(all(v)), end(v));
}

tcT > V<T> prefSum(V<T>& a, int off = 1) {
    int N = sz(a);
    V<T> ret(N + 1);
    rep(i, N) ret[i + 1] = ret[i] + a[i];
    if (off == 0) ret.erase(ret.begin());
    return ret;
}

tcT > V<T> sufSum(const V<T>& a) {
    V<T> ret = a;
    per(i, sz(ret) - 1) ret[i] += ret[i + 1];
    return ret;
}

// sorted[i] = v[idx[i]]
tcT > vi sortedIdx(const V<T>& v) {
    vi ret(sz(v));
    iota(all(ret), 0);
    sort(all(ret), [&](int i, int j) { return v[i] < v[j]; });
    return ret;
}

tcT > V<T> rearrange(const V<T>& v, const vi& idx) {
    V<T> ret(sz(idx));
    rep(i, sz(idx)) ret[i] = v[idx[i]];
    return ret;
}

template <typename T, typename... Vectors>
void concat(V<T>& first, const Vectors&... others) {
    V<T>& res = first;
    (res.insert(res.end(), others.begin(), others.end()), ...);
}

// ? = -1
tcT > vi str_to_vi(const str& S, T first_char) {
    vi A(sz(S));
    rep(i, sz(S)) { A[i] = (S[i] != '?' ? S[i] - first_char : -1); }
    return A;
}

tcT > T Sum(const V<T>& v) {
    T ret = 0;
    each(x, v) ret += x;
    return ret;
}

tcT > vi Count(const V<T>& v, int MX) {
    V<T> cnt(MX + 1);
    each(x, v)++ cnt[x];
    return cnt;
}

tcT > T Max(const V<T>& v) { return *max_element(all(v)); }

tcT > T Min(const V<T>& v) { return *min_element(all(v)); }

tcT > int MaxIdx(const V<T>& v) { return max_element(all(v)) - bg(v); }

tcT > int MinIdx(const V<T>& v) { return min_element(all(v)) - bg(v); }

inline namespace IO {
#define SFINAE(x, ...)                                            \
    template <class, class = void> struct x : std::false_type {}; \
    template <class T> struct x<T, std::void_t<__VA_ARGS__>> : std::true_type {}

SFINAE(DefaultI, decltype(std::cin >> std::declval<T&>()));
SFINAE(DefaultO, decltype(std::cout << std::declval<T&>()));
SFINAE(IsTuple, typename std::tuple_size<T>::type);
SFINAE(Iterable, decltype(std::begin(std::declval<T>())));

template <auto& is> struct Reader {
    template <class T> void Impl(T& t) {
        if constexpr (DefaultI<T>::value)
            is >> t;
        else if constexpr (Iterable<T>::value) {
            for (auto& x : t) Impl(x);
        } else if constexpr (IsTuple<T>::value) {
            std::apply([this](auto&... args) { (Impl(args), ...); }, t);
        } else
            static_assert(IsTuple<T>::value, "No matching type for read");
    }
    template <class... Ts> void read(Ts&... ts) { ((Impl(ts)), ...); }
};

template <class... Ts> void re(Ts&... ts) { Reader<cin>{}.read(ts...); }
#define def(t, args...) \
    t args;             \
    re(args);

#define Int(args...) def(int, args)
#define Str(args...) def(str, args)
#define Db(args...) def(db, args)
#define Vec(t, name, size) \
    V<t> name(size);       \
    re(name);

#define Vi(name, size) \
    vi name(size);     \
    re(name);

template <auto& os, bool debug, bool print_nd> struct Writer {
    string comma() const { return debug ? "," : ""; }
    template <class T> constexpr char Space(const T&) const {
        return print_nd && (Iterable<T>::value or IsTuple<T>::value) ? '\n'
                                                                     : ' ';
    }
    template <class T> void Impl(T const& t) const {
        if constexpr (DefaultO<T>::value)
            os << t;
        else if constexpr (Iterable<T>::value) {
            if (debug) os << '{';
            int i = 0;
            for (auto&& x : t)
                ((i++) ? (os << comma() << Space(x), Impl(x)) : Impl(x));
            if (debug) os << '}';
        } else if constexpr (IsTuple<T>::value) {
            if (debug) os << '(';
            std::apply(
                [this](auto const&... args) {
                    int i = 0;
                    (((i++) ? (os << comma() << " ", Impl(args)) : Impl(args)),
                     ...);
                },
                t);
            if (debug) os << ')';
        } else
            static_assert(IsTuple<T>::value, "No matching type for print");
    }
    template <class T> void ImplWrapper(T const& t) const { Impl(t); }
    template <class... Ts> void print(Ts const&... ts) const {
        ((Impl(ts)), ...);
    }
    template <class F, class... Ts>
    void print_with_sep(const std::string& sep,
                        F const& f,
                        Ts const&... ts) const {
        ImplWrapper(f), ((os << sep, ImplWrapper(ts)), ...), os << '\n';
    }
    void print_with_sep(const std::string&) const { os << '\n'; }
};

template <class... Ts> void pr(Ts const&... ts) {
    Writer<cout, false, true>{}.print(ts...);
}
template <class... Ts> void ps(Ts const&... ts) {
    Writer<cout, false, true>{}.print_with_sep(" ", ts...);
}
}  // namespace IO

inline namespace Debug {
template <typename... Args> void err(Args... args) {
    Writer<cerr, true, false>{}.print_with_sep(" | ", args...);
}

void err_prefix(string func, int line, string args) {
    cerr << func << ":" << line << " - " << "[" << args << "] = ";
}

#ifdef LOCAL
#define dbg(args...) err_prefix(__FUNCTION__, __LINE__, #args), err(args)
#else
#define dbg(...)
#endif

void setIO() {
    cin.tie(0)->sync_with_stdio(0);
    cout << fixed << setprecision(12);
}
}  // namespace Debug

void YES(bool t = 1) { ps(t ? "YES" : "NO"); }
void NO(bool t = 1) { YES(!t); }
void Yes(bool t = 1) { ps(t ? "Yes" : "No"); }
void No(bool t = 1) { Yes(!t); }
void yes(bool t = 1) { ps(t ? "yes" : "no"); }
void no(bool t = 1) { yes(!t); }

namespace std {

template <class Fun> class y_combinator_result {
    Fun fun_;

  public:
    template <class T>
    explicit y_combinator_result(T&& fun) : fun_(std::forward<T>(fun)) {}

    template <class... Args> decltype(auto) operator()(Args&&... args) {
        return fun_(std::ref(*this), std::forward<Args>(args)...);
    }
};

template <class Fun> decltype(auto) fun(Fun&& fun) {
    return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

}  // namespace std
