#include <bits/stdc++.h>
using namespace std;

int main(){
    int v,e;
    cin >> v >> e;
    cout << v << " " << e << endl;
    for(int i = 0; i<e; i++){
        int a;
        string ss;
        cin >> a >> ss;
        if(ss[ss.length()-2] == '-'){
            cout << a << " " << ss.replace(ss.length() - 2, 1, " -") << endl;
        }else{
            cout << a << " " << ss.replace(ss.length() - 1, 1, " " + ss.substr(ss.length()-1,1)) << endl;
        }
    }

    return 0;
}