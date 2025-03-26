#pragma once

#include "fenwick-tree.hpp"

template <typename AbelGroup, typename XY, bool SMALL_X = false>
struct Point_Add_Rectangle_Sum {
    using G = typename AbelGroup::value_type;
    using Point = tuple<XY, XY, G>;
    vector<Point> point;
    vector<tuple<XY, XY, XY, XY>> rect;

    Point_Add_Rectangle_Sum() {}

    void add_query(XY x, XY y, G w) { point.eb(x, y, w); }
    void sum_query(XY xl, XY xr, XY yl, XY yr) { rect.eb(xl, xr, yl, yr); }

    vector<G> calc() {
        int N = point.size(), Q = rect.size();
        if (N == 0 || Q == 0) return vector<G>(Q, AbelGroup::unit());
        // X 方向の座圧

        int NX = 0;
        if (!SMALL_X) {
            sort(all(point), [&](auto& x, auto& y) -> bool {
                return get<0>(x) < get<0>(y);
            });
            V<XY> keyX;
            keyX.reserve(N);
            for (auto&& [a, b, c] : point) {
                if (sz(keyX) == 0 || keyX.back() != a) {
                    keyX.eb(a);
                }
                a = sz(keyX) - 1;
            }
            for (auto&& [xl, xr, yl, yr] : rect) {
                xl = lwb(keyX, xl);
                xr = lwb(keyX, xr);
            }
            NX = sz(keyX);
        }
        if (SMALL_X) {
            XY mx = infty<XY>;
            for (auto&& [x, y, g] : point) ckmin(mx, x);
            for (auto&& [x, y, g] : point) x -= mx, ckmax(NX, x + 1);
            for (auto&& [xl, xr, yl, yr] : rect) {
                xl -= mx, xr -= mx;
                xl = max(0ll, min<int>(xl, NX));
                xr = max(0ll, min<int>(xr, NX));
            }
        }

        V<tuple<XY, int, int, int>> event(Q + Q);
        rep(q, Q) {
            auto& [xl, xr, yl, yr] = rect[q];
            event[2 * q] = {yl, xl, xr, 2 * q};
            event[2 * q + 1] = {yr, xl, xr, 2 * q + 1};
        }
        sort(all(point),
             [&](auto& x, auto& y) -> bool { return get<1>(x) < get<1>(y); });
        sort(all(event),
             [&](auto& x, auto& y) -> bool { return get<0>(x) < get<0>(y); });

        FenwickTree<AbelGroup> bit(NX);
        V<G> res(Q, AbelGroup::unit());
        int j = 0;
        for (auto&& [y, xl, xr, qq] : event) {
            while (j < N && get<1>(point[j]) < y) {
                bit.add(get<0>(point[j]), get<2>(point[j]));
                ++j;
            }
            G g = bit.sum(xl, xr);
            int q = qq / 2;
            if (qq % 2 == 0) g = AbelGroup::inverse(g);
            res[q] = AbelGroup::op(res[q], g);
        }
        return res;
    }
};
