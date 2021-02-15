#include <atcoder/all>
#include <bits/stdc++.h>

using namespace std;

int main(){
    int s,t; cin >> s >> t;
    int n,m; cin >> n >> m;
    atcoder::mf_graph<int> g(n);
    for(int i=0;i<m;i++){
        int l,r,w; cin >> l >> r >> w;
        g.add_edge(l-1, r-1, w);
        g.add_edge(r-1, l-1,w);
    }

    cout << g.flow(s-1,t-1) << endl;
}