#include<vector>
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

bool a[100000];
bool b[100000];

int main(){
    int n,m; cin >> n >> m;
    vector<vector<int>> g(n);
    vector<int> sol;
    for(int i=0;i<n;i++) a[i] = false;
    for(int i=0;i<n;i++) b[i] = false;
    for(int i=0;i<m;i++){
        int l,r,w; cin >> l >> r >> w;
        g[l-1].push_back(r-1);
        g[r-1].push_back(l-1);
    }

    int s = 0;
    queue<int> q;
    a[0] = true;
    q.push(0);
    while(!q.empty()){
        s = q.front(); q.pop();
        for(auto x: g[s]){
            if(!a[x]){
                a[x] = true;
                q.push(x);
            }
        }
    }
    sol.push_back(s + 1);
    b[s] = true;
    q.push(s);
    while(!q.empty()){
        s = q.front(); q.pop();
        for(auto x: g[s]){
            if(!b[x]){
                b[x] = true;
                q.push(x);
            }
        }
    }
    sol.push_back(s + 1);
    sort(sol.begin(), sol.end());
    cout << sol[0] << " " << sol[1] << endl;
    return 0;
}