#include<vector>
#include<iostream>
#include<algorithm>
#include <random>
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

void sbox(int s){
  for(int i=0;i<s;i++) for(int j=0;j<s;j++){
    kings[i][j][0] = -7;
  }

  for(int i=0;i<s;i++) for(int j=0;j<s;j++) for(int k=1;k<4;k++){
    if(i + dx(k) < s && j + dy(k) < s) kings[i][j][k] = -7;
  }
}

void tbox(int s){
  for(int i=384-s; i<384;i++) for(int j=384-s;j<384-s;j++){
    kings[i][j][0] = +7;
  }

  for(int i=384-s; i<384;i++) for(int j=384-s;j<384;j++) for(int k=1; k<4;k++){
    if(i + dx(k) < 384 && j + dy(k) < 384) kings[i][j][k] = -7;
  }
}

void rnderase(int s, int er){
  random_device rnd;
  int cnt = 0;
  while(er > cnt){
    int i = rnd() % 384, j = rnd() % 384, k = rnd() % 3 + 1;
    if(valid(i,j,k)){
      if(kings[i][j][k]!=-7 && kings[i][j][k]!=0){
        kings[i][j][k] = 0;
        cnt++;
      }
    }
  }
}

int main(){
  int numedge = 320922;
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<4;k++){
    kings[i][j][k] = 0;
  }

  // gen all connect
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<3;k++){
    kings[i][j][k+1] = -1;
  }
  random_device rnd;
  int per; cin >> per;
  int er = ((100-per)*320922) / 100;
  kings[0][0][0] = -7;
  kings[383][383][0] = +7;
  sbox(5);
  tbox(5);
  rnderase(5,er);
  genjson();
  return 0;
}

