tcT > V<T> Transpose(const V<T>& v) {
    using U = typename T::value_type;
    if (v.empty()) return {};
    int H = v.size(), W = v[0].size();
    vector res(W, T(H, U{}));
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            res[j][i] = v[i][j];
        }
    }
    return res;
}

tcT > V<T> Rotate(const V<T>& v, int clockwise = true) {
    using U = typename T::value_type;
    int H = v.size(), W = v[0].size();
    vector res(W, T(H, U{}));
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (clockwise) {
                res[j][H - 1 - i] = v[i][j];
            } else {
                res[W - 1 - j][i] = v[i][j];
            }
        }
    }
    return res;
}
