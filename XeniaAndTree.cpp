#include <bits/stdc++.h>
#define pb push_back
using namespace std;

const int N=200005;
int n,m,a,b,r=1,u,v,root;
bool visited[N], visited2[N];
int ans[N];

struct node{
	int p;
	vector<int>c;
	int nchild;
	int pos;
	int head;
	int chain;
	int h;
} T[N], TD[N];

bool cmp(int a, int b){
	return T[a].nchild < T[b].nchild; 
}

int calChild(int u){
	int r = 0;
	visited[u] = true;
	if (T[u].c.size() == 0) return 0;
	int nc = 0;
	for(int i=0;i<T[u].c.size();i++){
		v = T[u].c[i];
		if(visited[v] or visited2[v]) continue;
		nc += calChild(v) + 1;
	}
	T[u].nchild = nc;
	return nc;
}

int centroid(int u){
	int n_ = T[u].nchild + 1;
	while(1){
		int spe = -1;
		for(int i=0;i < T[u].c.size();i++){
			v = T[u].c[i];
			if (T[v].nchild > T[u].nchild or visited2[v]) continue;
			if (T[v].nchild + 1 > n_/2){
				spe = v;
				break;
			}
		}
		if (spe == -1){
			break;
		}
		else u = spe;
	}
	return u;
	
}

int centroiDe(int u){
	int r = centroid(u);
	fill(visited+1, visited+n+1, 0);	
	visited2[r] = true;
	for (int i=0;i<T[r].c.size();i++){
		int v = T[r].c[i];
		if (visited2[v]) continue;
		if (T[u].nchild < T[v].nchild) calChild(v);
		int x = centroiDe(v);
		TD[r].c.pb(x);
		TD[x].p = r;
	}
	return r;
}

void bfs(int r){
	fill(visited+1, visited+n+1, 0);
	queue <int> Q;
	Q.push(r);
	while (!Q.empty()){
		int u = Q.front(); Q.pop();
		visited[u] = true;
		for(int i=0;i<T[u].c.size();i++){
			v = T[u].c[i];
			if (!visited[v]){
				T[v].p = u;
				T[v].h = T[u].h + 1;
				Q.push(v);
			}
		}
	}
}

void hld(int r){
	for(int i=1;i<=n;i++){
		sort(T[i].c.begin(), T[i].c.end(),cmp);
	}
	
	vector <int> S;
	S.push_back(r);
	int head = r;
	int pos = 1;
	int chain = 1;
	
	while(!S.empty()){
		u = S.back(); S.pop_back();
		T[u].head = head;
		T[u].pos = pos ++;
		T[u].chain = chain;
		if(T[u].c.size() == 1){
			u = S.back();
			head = u;
			pos = 1;
			chain ++;
		}
		else{
			for(int i=0;i<T[u].c.size();i++){
			v = T[u].c[i];
			if (T[u].p != v) S.push_back(v);
			}
		}
	}
}

int lca1(int u, int v){
     while (T[u].head != T[v].head)
     {
          if (T[T[u].head].h < T[T[v].head].h) v = T[T[v].head].p;
          else u = T[T[u].head].p;
     }
     
     if (T[u].h < T[v].h) return u;
     else return v;
}


int dist(int a, int b){
	return T[a].h + T[b].h - 2*T[lca1(a,b)].h;
}

void update(int x){
	int v = x;
	while (x != 0){
		ans[x] = min(dist(v,x),ans[x]);
		x = TD[x].p;
	}
}

int query(int x){
	int v = x;
	int r = ans[x];
	while (x!=0){
		r = min(r, ans[x] + dist(v,x));
		x = TD[x].p;
	}
	return r;
}
int main(){
	cin >> n >> m;
	for(int i=1;i<n;i++){
		cin >> a >> b;
		T[a].c.pb(b);
		T[b].c.pb(a);
	}
	calChild(1);
	root = centroid(1);
	fill(visited+1, visited+n+1, 0);
	calChild(root);
	centroiDe(root);
	bfs(root);
	T[root].c.push_back(0);
	T[0].nchild = N;
	hld(root);
	fill(ans+1,ans+n+1,1e9);
	update(1);
	for(int i=0;i<m;i++){
		cin >> a >> b;
		if (a == 1) update(b);
		else cout << query(b) << endl;
	}
}
