#pragma once
#include "segment-tree.hpp"
#include "tree.hpp"
#include "alg/monoid/monoid_reverse.hpp"

template <typename TREE, typename Monoid, bool edge> struct Tree_Monoid {
    using MX = Monoid;
    using X = typename MX::value_type;
    TREE& tree;
    int N;
    SegTree<MX> seg;
    SegTree<Monoid_Reverse<MX>> seg_r;

    Tree_Monoid(TREE& tree) : tree(tree), N(tree.N) {
        build([](int i) -> X { return MX::unit(); });
    }

    Tree_Monoid(TREE& tree, V<X>& dat) : tree(tree), N(tree.N) {
        build([&](int i) -> X { return dat[i]; });
    }

    template <typename F> Tree_Monoid(TREE& tree, F f) : tree(tree), N(tree.N) {
        build(f);
    }

    template <typename F> void build(F f) {
        if (!edge) {
            auto f_v = [&](int i) -> X { return f(tree.V[i]); };
            seg.build(N, f_v);
            if constexpr (!MX::commute) {
                seg_r.build(N, f_v);
            }
        } else {
            auto f_e = [&](int i) -> X {
                return (i == 0 ? MX::unit() : f(tree.v_to_e(tree.V[i])));
            };
            seg.build(N, f_e);
            if constexpr (!MX::commute) {
                seg_r.build(N, f_e);
            }
        }
    }

    void set(int i, X x) {
        if constexpr (edge) i = tree.e_to_v(i);
        i = tree.LID[i];
        seg.set(i, x);
        if constexpr (!MX::commute) seg_r.set(i, x);
    }

    void multiply(int i, X x) {
        if constexpr (edge) i = tree.e_to_v(i);
        i = tree.LID[i];
        seg.multiply(i, x);
        if constexpr (!MX::commute) seg_r.multiply(i, x);
    }

    X prod_path(int u, int v) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto&& [a, b] : pd) {
            val = MX::op(val, get_prod(a, b));
        }
        return val;
    }

    // uv path 上で prod_path(u, x) が check を満たす最後の x

    // なければ （つまり path(u,u) が ng ）-1

    template <class F> int max_path(F check, int u, int v) {
        if constexpr (edge) return max_path_edge(check, u, v);
        if (!check(prod_path(u, u))) return -1;
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto&& [a, b] : pd) {
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.V[b]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            if (a <= b) {
                // 下り

                auto i = seg.max_right(check_tmp, a);
                return (i == a ? u : tree.V[i - 1]);
            } else {
                // 上り

                int i = 0;
                if constexpr (MX::commute) i = seg.min_left(check_tmp, a + 1);
                if constexpr (!MX::commute)
                    i = seg_r.min_left(check_tmp, a + 1);
                if (i == a + 1) return u;
                return tree.V[i];
            }
        }
        return v;
    }

    X prod_subtree(int u, int root = -1) {
        if (root == u) return prod_all();
        if (root == -1 || tree.in_subtree(u, root)) {
            int l = tree.LID[u], r = tree.RID[u];
            return seg.query(l + edge, r);
        }
        assert(!edge);  // さぼり

        u = tree.jump(u, root, 1);
        int L = tree.LID[u], R = tree.RID[u];
        return MX::op(seg.query(0, L), seg.query(R, N));
    }

    X prod_all() { return prod_subtree(tree.V[0]); }

    inline X get_prod(int a, int b) {
        if constexpr (MX::commute) {
            return (a <= b) ? seg.query(a, b + 1) : seg.query(b, a + 1);
        }
        return (a <= b) ? seg.query(a, b + 1) : seg_r.query(b, a + 1);
    }

  private:
    template <class F> int max_path_edge(F check, int u, int v) {
        static_assert(edge);
        if (!check(MX::unit())) return -1;
        int lca = tree.lca(u, v);
        auto pd = tree.get_path_decomposition(u, lca, edge);
        X val = MX::unit();

        // climb

        for (auto&& [a, b] : pd) {
            assert(a >= b);
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.parent[tree.V[b]]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            int i = 0;
            if constexpr (MX::commute) i = seg.min_left(check_tmp, a + 1);
            if constexpr (!MX::commute) i = seg_r.min_left(check_tmp, a + 1);
            if (i == a + 1) return u;
            return tree.parent[tree.V[i]];
        }
        // down

        pd = tree.get_path_decomposition(lca, v, edge);
        for (auto&& [a, b] : pd) {
            assert(a <= b);
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.V[b]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            auto i = seg.max_right(check_tmp, a);
            return (i == a ? u : tree.V[i - 1]);
        }
        return v;
    }
};
