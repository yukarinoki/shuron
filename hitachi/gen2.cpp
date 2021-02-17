#include <atcoder/all>
#include <bits/stdc++.h>
using namespace std;

int kings[400][400][5];


bool valid(int i, int j,int k){
  if(k==0) return true;
  if(k==1 && i+1 <= 383) return  true;
  if(k==2 && i+1 <=383 && j+1 <= 383) return true;
  if(k==3 && j+1 <= 383) return true;
  if(k==4 && i+i <= 383 && j-1 >= 0 && j-1 <= 383) return true;
  return false;
}

int dx(int k){
  if(k==3 || k==0 || k==4) return 0;
  else return 1;
}

int dy(int k){
  if(k==1 || k==0) return 0;
  else if(k==4) return -1;
  else return 1;
}

void genjson(string path){
  ofstream st(path);
  int li=1000,lj,lk;
  for(int i=383;i>=0;i--) for(int j=383;j>=0;j--) for(int k=4;k>=0;k--){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      li = i; lj = j; lk = k;
      i=-1; j=-1; k=-1;
    }
  }

  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=0;k<5;k++){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      st << "[" << i << "," << j << "," << i+dx(k) << "," << j+dy(k) << "," << kings[i][j][k] << "]";
      if(li!=i || lj!=j || lk!=k) st << ",";
      st << endl;
    }
  }
}

void sbox(int s){
  for(int i=0;i<s;i++) for(int j=0;j<s;j++){
    kings[i][j][0] = -7;
  }

  for(int i=0;i<s;i++) for(int j=0;j<s;j++) for(int k=1;k<5;k++){
    if(i + dx(k) < s && j + dy(k) < s) kings[i][j][k] = -7;
  }
}

void tbox(int s){
  for(int i=384-s; i<384;i++) for(int j=384-s;j<384;j++){
    kings[i][j][0] = +7;
  }

  for(int i=384-s; i<384;i++) for(int j=384-s;j<384;j++) for(int k=1; k<5;k++){
    if(i + dx(k) < 384 && j + dy(k) < 384 && j + dy(k) >= 384-s) kings[i][j][k] = -7;
  }
}

void rnderase(int s, int er){
  std::mt19937 rnd(111);
  int cnt = 0;
  while(er > cnt){
    int i = rnd() % 384, j = rnd() % 384, k = rnd() % 4 + 1;
    if(valid(i,j,k)){
      if(kings[i][j][k]!=-7 && kings[i][j][k]!=0){
        kings[i][j][k] = 0;
        cnt++;
      }
    }
  }
}

int toid(int i,int j,int k){
  return (i+dx(k))*384 + (j+dy(k));
}

int mincut(){
  atcoder::mf_graph<int> g(384*384);
  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=1;k<5;k++){
    int id = 384*i + j;
    if(valid(i,j,k) && kings[i][j][k] == -7){
      g.add_edge(id, toid(i,j,k), 10000000);
      g.add_edge(toid(i,j,k), id, 10000000);
    }
    if(valid(i,j,k) && kings[i][j][k] == -1){
      g.add_edge(id, toid(i,j,k), 1);
      g.add_edge(toid(i,j,k), id, 1);
    }
  }
  return g.flow(0, 384*384-1);
}

void  genformated_graph(string path){
  ofstream st(path);

  int edgec = 0;
  int vertexc = 384*384;
  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=1;k<5;k++){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      edgec++;
    }
  }

  st << vertexc << " " << edgec << endl;

  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=0;k<5;k++){
    if(valid(i,j,k) && kings[i][j][k]!=0){
      int id1 = 384*i + j;
      int id2 = 384*(i+dx(k)) + j+dy(k);
      st << id1 << " " << id2 << " " << kings[i][j][k] << endl;
    }
  }
}

int energy(){
  int e = 0;
  for(int i=0;i<384;i++) for(int j=0;j<384;j++) for(int k=0;k<5;k++){
    if(valid(i,j,k) && kings[i][j][k] != 0)  e += -abs(kings[i][j][k]);
  }
  return e;
}


int main(){
  int numedge = 320922;
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<5;k++){
    kings[i][j][k] = 0;
  }

  // gen all connect
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=0;k<5;k++){
    kings[i][j][k+1] = -1;
  }

  std::mt19937 rnd(111);
  int per; cin >> per;
  int er = ((100-per)*320922) / 100;
  kings[0][0][0] = -7;
  kings[383][383][0] = +7;
  sbox(5);
  tbox(5);
  rnderase(5,er);
  // mincut part
  genjson("./testdata/mincut/graph/mincgraph_" + to_string(per));
  ofstream mincut_solution_stream("./testdata/mincut/solution/mincsol_" + to_string(per));
  int mc = mincut();
  mincut_solution_stream << mc << endl;
  
  ofstream mincut_energy_stream("./testdata/mincut/energy/mincenergy_" + to_string(per));
  mincut_energy_stream << energy() + 2*mc << endl;

  //maxcut part
  for(int i=0;i<400;i++) for(int j=0;j<400;j++) {
    kings[i][j][0] = 0;
  }
  //maxcut part 100% positive
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=1;k<5;k++){
    if(kings[i][j][k]==-1) kings[i][j][k] = 1;
  }
  genjson("./testdata/maxcut/graph_100/maxcgraph_" + to_string(per));
  genformated_graph("./testdata/maxcut/formated_graph100/maxcformatedgraph_" + to_string(per));


  // maxcut part 50% positive 50% negative
  for(int i=0;i<400; i++) for(int j=0;j<400;j++) for(int k=1;k<5;k++){
    if(kings[i][j][k]==1 && (rnd()%2)==1) kings[i][j][k] = 1;
  }
  genjson("./testdata/maxcut/graph_50/maxcgraph_" + to_string(per));
  genformated_graph("./testdata/maxcut/formated_graph50/maxcformatedgraph_" + to_string(per));

  cout << "done" << endl;
  return 0;
}

