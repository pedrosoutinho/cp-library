#include "tree.hpp"
#include "unionfind.hpp"

template <typename GT> struct UnicyclicGraph {
    static_assert(!GT::is_directed);
    using T = typename GT::cost_type;
    GT& G0;
    int N;
    int root;
    int out_eid;
    T out_cost;
    vector<int> TO;
    vector<int> cycle;      // 根に向かうような頂点列
    vector<bool> in_cycle;  // vertex id -> bool

    UnicyclicGraph(GT& G) : G0(G), N(G.N) {
        assert(N == G.M);
        UnionFind uf(N);
        TO.assign(N, -1);
        for (int eid = 0; eid < N; eid++) {
            auto& e = G.edges[eid];
            if (uf.merge(e.frm, e.to)) continue;
            out_eid = eid, out_cost = e.cost;
            root = e.frm;
            TO[root] = e.to;
            break;
        }
        vector<bool> done(N);
        vector<int> que = {root};
        while (!que.empty()) {
            int v = que.back();
            que.pop_back();
            done[v] = 1;
            for (auto&& e : G[v]) {
                if (done[e.to] || e.id == out_eid) continue;
                TO[e.to] = v;
                que.emplace_back(e.to);
            }
        }
        cycle = {TO[root]};
        while (cycle.back() != root) cycle.emplace_back(TO[cycle.back()]);
        in_cycle.assign(N, 0);
        for (auto&& v : cycle) in_cycle[v] = 1;
    }

    // {G, tree}
    pair<Graph<T, 1>, Tree<Graph<T, 1>>> build(bool keep_eid = false) {
        Graph<T, 1> G(N);
        for (int eid = 0; eid < N; eid++) {
            if (eid == out_eid) continue;
            auto& e = G0.edges[eid];
            int a = e.frm, b = e.to;
            if (TO[a] == b) swap(a, b);
            assert(TO[b] == a);
            int k = (keep_eid ? eid : -1);
            G.add(a, b, e.cost, k);
        }
        G.build();
        Tree<decltype(G)> tree(G, root);
        return {G, tree};
    };

    template <typename TREE> int dist(TREE& tree, int a, int b) {
        int btm = TO[root];
        int ra = tree.lca(a, btm), rb = tree.lca(b, btm);
        int d = abs(tree.depth[ra] - tree.depth[rb]);
        d = min<int>(d, (int)cycle.size() - d);
        return d + tree.depth[a] + tree.depth[b] - tree.depth[ra] -
               tree.depth[rb];
    }

    template <typename TREE> T dist_weighted(TREE& tree, int a, int b) {
        int btm = TO[root];
        int ra = tree.lca(a, btm), rb = tree.lca(b, btm);
        vector<T>& D = tree.depth_weighted;
        T d = abs(D[ra] - D[rb]);
        d = min(d, D[btm] + out_cost - d);
        return d + D[a] + D[b] - D[ra] - D[rb];
    }
};
