#include<vector>
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;


int main(){
    int s,t; cin >> s >> t;
    int n,m; cin >> n >> m;
    cout << s << " " << t << endl;
    cout << n << " " << m << endl;
    for(int i=0;i<m;i++){
        int a,b,c;
        cin >> a >> b >> c;
        cout << a << " " << b << " " << abs(c) << endl;
    }
    return 0;
}