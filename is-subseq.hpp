template <typename STRING> bool is_subseq(STRING& S, STRING& T) {
    ll p = 0;
    for (auto&& s : S) {
        while (p < sz(T) && T[p] != s) ++p;
        if (p == sz(T)) return false;
        ++p;
    }
    return true;
}
