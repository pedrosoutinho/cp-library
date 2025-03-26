#include "graph-base.hpp"
#include "dual-segtree.hpp"

template <typename TREE, typename Monoid, bool edge> struct Dual_Tree_Monoid {
    using MX = Monoid;
    using X = typename MX::value_type;
    TREE& tree;
    int N;
    Dual_SegTree<MX> seg;

    Dual_Tree_Monoid(TREE& tree) : tree(tree), N(tree.N), seg(tree.N) {}

    X get(int i) {
        int v = i;
        if (edge) {
            auto&& e = tree.G.edges[i];
            v = (tree.parent[e.frm] == e.to ? e.frm : e.to);
        }
        return seg.get(tree.LID[v]);
    }

    V<X> get_all() {
        V<X> tmp = seg.get_all();
        V<X> res;
        rep(i, N) {
            if (edge && i == N - 1) break;
            int v = i;
            if (edge) {
                auto&& e = tree.G.edges[i];
                v = (tree.parent[e.frm] == e.to ? e.frm : e.to);
            }
            res.eb(tmp[tree.LID[v]]);
        }
        return res;
    }

    void apply_path(int u, int v, X x) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        for (auto&& [a, b] : pd) {
            (a <= b ? seg.apply(a, b + 1, x) : seg.apply(b, a + 1, x));
        }
        return;
    }

    void apply_subtree(int u, X x) {
        int l = tree.LID[u], r = tree.RID[u];
        return seg.apply(l + edge, r, x);
    }

    void apply_outtree(int u, X a) {
        int l = tree.LID[u], r = tree.RID[u];
        seg.apply(0 + edge, l + edge, a);
        seg.apply(r, N, a);
    }
};
