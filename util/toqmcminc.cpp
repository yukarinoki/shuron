#include<vector>
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;


int main(){
    int s,t; cin >> s >> t;
    int n,m; cin >> n >> m;
    cout << n << " " << m + 2 << endl;
    cout << s << " " << s << " " << 100 << endl;
    cout << t << " " << t << " " << -100 << endl;
    for(int i=0;i<m;i++){
        int a,b,c;
        cin >> a >> b >> c;
        cout << a << " " << b << " " << -abs(c) << endl;
    }

    return 0;
}