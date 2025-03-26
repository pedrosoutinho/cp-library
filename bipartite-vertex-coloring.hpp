#pragma once

#include "graph-base.hpp"
#include "unionfind.hpp"

// 二部グラフでなかった場合には empty

template <typename GT> V<int> bipartite_vertex_coloring(GT& G) {
    assert(!GT::is_directed);
    assert(G.is_prepared());

    int n = G.N;
    UnionFind uf(2 * n);
    for (auto&& e : G.edges) {
        int u = e.frm, v = e.to;
        uf.merge(u + n, v), uf.merge(u, v + n);
    }

    V<int> color(2 * n, -1);
    rep(v, n) if (uf[v] == v && color[uf[v]] < 0) {
        color[uf[v]] = 0;
        color[uf[v + n]] = 1;
    }
    rep(v, n) color[v] = color[uf[v]];
    color.resize(n);
    rep(v, n) if (uf[v] == uf[v + n]) return {};
    return color;
}
