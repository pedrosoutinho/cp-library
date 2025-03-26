/*
辞書順で高さを unique して、木にしている。
極大長方形アルゴリズムで線形時間構築。
*/
template <typename T, bool IS_MIN> struct CartesianTree {
    int n;
    V<T>& A;
    V<pair<int, int>> range;
    V<int> lch, rch, par;
    int root;

    CartesianTree(V<T>& A) : n(sz(A)), A(A) {
        range.assign(n, {-1, -1});
        lch.assign(n, -1);
        rch.assign(n, -1);
        par.assign(n, -1);
        if (n == 1) {
            range[0] = {0, 1};
            root = 0;
            return;
        }
        auto is_sm = [&](int i, int j) -> bool {
            if (IS_MIN) return (A[i] < A[j]) || (A[i] == A[j] && i < j);
            return (A[i] > A[j]) || (A[i] == A[j] && i < j);
        };
        V<int> st;
        rep(i, n) {
            while (!st.empty() && is_sm(i, st.back())) {
                lch[i] = st.back();
                st.pop_back();
            }
            range[i].fi = (st.empty() ? 0 : st.back() + 1);
            st.eb(i);
        }
        st.clear();
        per(i, n) {
            while (!st.empty() && is_sm(i, st.back())) {
                rch[i] = st.back();
                st.pop_back();
            }
            range[i].se = (st.empty() ? n : st.back());
            st.eb(i);
        }
        rep(i, n) if (lch[i] != -1) par[lch[i]] = i;
        rep(i, n) if (rch[i] != -1) par[rch[i]] = i;
        rep(i, n) if (par[i] == -1) root = i;
    }

    // (l, r, h)

    tuple<int, int, T> maximum_rectangle(int i) {
        auto [l, r] = range[i];
        return {l, r, A[i]};
    }

    // (l, r, h)

    T max_rectangle_area() {
        assert(IS_MIN);
        T res = 0;
        rep(i, n) {
            auto [l, r, h] = maximum_rectangle(i);
            ckmax(res, (r - l) * h);
        }
        return res;
    }

    ll count_subrectangle(bool baseline) {
        assert(IS_MIN);
        ll res = 0;
        rep(i, n) {
            auto [l, r, h] = maximum_rectangle(i);
            ll x = (baseline ? h : h * (h + 1) / 2);
            res += x * (i - l + 1) * (r - i);
        }
        return res;
    }
};
