#pragma once

#include "bfs01.hpp"
#include "restore-path.hpp"

template <typename T, typename GT> pair<T, vector<int>> tree_diameter(GT& G) {
    assert(G.is_prepared());
    int A, B;
    auto [distA, parA] = bfs01<T>(G, 0);
    A = max_element(all(distA)) - distA.begin();
    auto [dist, par] = bfs01<T>(G, A);
    B = max_element(all(dist)) - dist.begin();
    vector<int> P = restore_path(par, B);
    return {dist[B], P};
}
