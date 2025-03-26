int mex(const V<int>& A) {
    int n = sz(A);
    V<bool> aru(n + 1);
    for (auto& x : A)
        if (x < n) aru[x] = 1;
    int mex = 0;
    while (aru[mex]) ++mex;
    return mex;
}
