#pragma once
#include "graph-base.hpp"

#include "tree.hpp"

template <typename TREE, typename Data> struct Rerooting_dp {
    static_assert(!TREE::Graph_type::is_directed);
    TREE& tree;
    V<Data> dp_1;  // 辺 pv に対して、部分木 v

    V<Data> dp_2;  // 辺 pv に対して、部分木 p

    V<Data> dp;  // full tree

    template <typename F1, typename F2, typename F3>
    Rerooting_dp(TREE& tree, F1 f_ee, F2 f_ev, F3 f_ve, const Data unit)
        : tree(tree) {
        build(f_ee, f_ev, f_ve, unit);
    }

    // v を根としたときの full tree

    Data operator[](int v) { return dp[v]; }

    // root を根としたときの部分木 v

    Data get(int v, int root) {
        if (root == v) return dp[v];
        if (!tree.in_subtree(root, v)) {
            return dp_1[v];
        }
        int w = tree.jump(v, root, 1);
        return dp_2[w];
    }

    template <typename F1, typename F2, typename F3>
    void build(F1 f_ee, F2 f_ev, F3 f_ve, const Data unit) {
        int N = tree.N;
        // dp1: subtree

        dp_1.assign(N, unit);
        per(i, N) {
            int v = tree.V[i];
            for (auto&& e : tree.G[v]) {
                if (e.to == tree.parent[v]) continue;
                dp_1[v] = f_ee(dp_1[v], f_ve(dp_1[e.to], e));
            }
            dp_1[v] = f_ev(dp_1[v], v);
        }

        // dp2[v]: subtree of p, rooted at v

        dp_2.assign(N, unit);
        // dp[v]: fulltree, rooted at v

        dp.assign(N, unit);
        rep(i, N) {
            int p = tree.V[i];
            V<int> ch;
            V<Data> ch_data;
            Data x = unit;
            for (auto&& e : tree.G[p]) {
                if (e.to == tree.parent[p]) {
                    x = f_ve(dp_2[p], e);
                } else {
                    ch.eb(e.to);
                    ch_data.eb(f_ve(dp_1[e.to], e));
                }
            }
            int n = sz(ch);
            if (!n) {
                dp[p] = f_ev(x, p);
                continue;
            }
            V<Data> prod_left(n, x);
            rep(i, n - 1) prod_left[i + 1] = f_ee(prod_left[i], ch_data[i]);
            Data prod_right = unit;
            per(i, n) {
                dp_2[ch[i]] = f_ev(f_ee(prod_left[i], prod_right), p);
                prod_right = f_ee(prod_right, ch_data[i]);
            }
            dp[p] = f_ev(f_ee(x, prod_right), p);
        }
    }
};
