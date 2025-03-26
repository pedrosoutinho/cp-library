#pragma once
#include "graph-base.hpp"

template <typename GT> pair<int, V<int>> strongly_connected_component(GT& G) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    int N = G.N;
    int C = 0;
    V<int> comp(N), low(N), ord(N, -1), path;
    int now = 0;

    auto dfs = [&](auto& dfs, int v) -> void {
        low[v] = ord[v] = now++;
        path.eb(v);
        for (auto&& [frm, to, cost, id] : G[v]) {
            if (ord[to] == -1) {
                dfs(dfs, to), ckmin(low[v], low[to]);
            } else {
                ckmin(low[v], ord[to]);
            }
        }
        if (low[v] == ord[v]) {
            while (1) {
                int u = POP(path);
                ord[u] = N, comp[u] = C;
                if (u == v) break;
            }
            ++C;
        }
    };
    rep(v, N) {
        if (ord[v] == -1) dfs(dfs, v);
    }
    rep(v, N) comp[v] = C - 1 - comp[v];
    return {C, comp};
}

template <typename GT> Graph<int, 1> scc_dag(GT& G, int C, V<int>& comp) {
    Graph<int, 1> DAG(C);
    V<vi> edges(C);
    for (auto&& e : G.edges) {
        int x = comp[e.frm], y = comp[e.to];
        if (x == y) continue;
        edges[x].eb(y);
    }
    rep(c, C) {
        Unique(edges[c]);
        for (auto&& to : edges[c]) DAG.add(c, to);
    }
    DAG.build();
    return DAG;
}
