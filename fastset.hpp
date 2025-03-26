// 64-ary tree

// space: (N/63) * u64

struct FastSet {
    static constexpr u32 B = 64;
    int n, log;
    V<V<u64>> seg;

    FastSet() {}
    FastSet(int n) { build(n); }

    int size() { return n; }

    template <typename F> FastSet(int n, F f) { build(n, f); }

    void build(int m) {
        seg.clear();
        n = m;
        do {
            seg.push_back(vector<u64>((m + B - 1) / B));
            m = (m + B - 1) / B;
        } while (m > 1);
        log = sz(seg);
    }
    template <typename F> void build(int n, F f) {
        build(n);
        rep(i, n) { seg[0][i / B] |= u64(f(i)) << (i % B); }
        rep(h, log - 1) {
            rep(i, sz(seg[h])) {
                seg[h + 1][i / B] |= u64(bool(seg[h][i])) << (i % B);
            }
        }
    }

    bool operator[](int i) const { return seg[0][i / B] >> (i % B) & 1; }
    void insert(int i) {
        for (int h = 0; h < log; h++) {
            seg[h][i / B] |= u64(1) << (i % B), i /= B;
        }
    }
    void add(int i) { insert(i); }
    void erase(int i) {
        u64 x = 0;
        for (int h = 0; h < log; h++) {
            seg[h][i / B] &= ~(u64(1) << (i % B));
            seg[h][i / B] |= x << (i % B);
            x = bool(seg[h][i / B]);
            i /= B;
        }
    }
    void remove(int i) { erase(i); }

    // min[x,n) or n

    int next(int i) {
        assert(i <= n);
        ckmax(i, 0ll);
        for (int h = 0; h < log; h++) {
            if (i / B == (int)seg[h].size()) break;
            u64 d = seg[h][i / B] >> (i % B);
            if (!d) {
                i = i / B + 1;
                continue;
            }
            i += lowbit(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += lowbit(seg[g][i / B]);
            }
            return i;
        }
        return n;
    }

    // max [0,x], or -1

    int prev(int i) {
        assert(i >= -1);
        if (i >= n) i = n - 1;
        for (int h = 0; h < log; h++) {
            if (i == -1) break;
            u64 d = seg[h][i / B] << (63 - i % B);
            if (!d) {
                i = i / B - 1;
                continue;
            }
            i -= __builtin_clzll(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += topbit(seg[g][i / B]);
            }
            return i;
        }
        return -1;
    }

    bool any(int l, int r) { return next(l) < r; }

    // [l, r)

    template <typename F> void enumerate(int l, int r, F f) {
        for (int x = next(l); x < r; x = next(x + 1)) f(x);
    }

    string to_string() {
        string s(n, '?');
        for (int i = 0; i < n; ++i) s[i] = ((*this)[i] ? '1' : '0');
        return s;
    }
};
