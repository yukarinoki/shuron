#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

int kings[400][400][4];

bool valid(int i, int j,int k){
  if(k==0) return true;
  if(k==1 && i+1 <= 383) return  true;
  if(k==2 && i+1 <=383 && j+1 <= 383) return true;
  if(k==3 && j+1 <= 383) return true;
  return false;
}

int dx(int k){
  if(k==3 || k==0) return 0;
  else return 1;
}

int dy(int k){
  if(k==1 || k==0) return 0;
  else return 1;
}

void genjson(){
  int li=1000,lj,lk;
  for(int i=383;i>=0;i--) for(int j=383;j>=0;j--) for(int k=3;k>=0;k--){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      li = i; lj = j; lk = k;
      i=-1; j=-1; k=-1;
    }
  }

  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=0;k<4;k++){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      cout << "[" << i << "," << j << "," << i + dx(k) << "," << j+dy(k) << "," << kings[i][j][k] << "]";
      if(li!=i || lj!=j || lk!=k) cout << ",";
      cout << endl;
    }
  }
}


int main(){
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<4;k++){
    kings[i][j][k] = 0;
  }

  // gen all connect
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<3;k++){
    kings[i][j][k+1] = -1;
  }
  kings[0][0][0] = -7;
  kings[383][383][0] = +7;

  genjson();
  return 0;
}

