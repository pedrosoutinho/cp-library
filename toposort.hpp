#pragma once

#include "fastset.hpp"
#include "graph-base.hpp"

// 辞書順最小の toposort を返す
template <typename GT> vector<int> toposort(GT& G) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N;
    auto [indeg, outdeg] = G.deg_array_inout();
    FastSet que(N);
    vector<int> V;
    rep(v, N) if (indeg[v] == 0) que.insert(v);
    while (1) {
        int v = que.next(0);
        if (v == N) break;
        que.erase(v), V.eb(v);
        for (auto&& e : G[v]) {
            if (--indeg[e.to] == 0) que.insert(e.to);
        }
    }
    return (sz(V) < N ? vector<int>{} : V);
}
