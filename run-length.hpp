template <typename STRING = string, typename CHAR = char>
V<pair<CHAR, int>> run_length(STRING& S) {
    V<pair<CHAR, int>> res;
    for (auto&& x : S) {
        if (res.empty() || res.back().fi != x) {
            res.emplace_back(x, 0);
        }
        res.back().se++;
    }
    return res;
}
