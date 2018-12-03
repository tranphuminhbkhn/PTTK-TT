#include <bits/stdc++.h>

using namespace std;
int n,q,a,b,r=1,u,v,ans,res;
const int N = 100005;

struct node {
	int pos;
	int head;
	int p;
	vector <int> c;
	int nchild;
} T[N];

int H[N];
int IT[4*N];
int Ph[N];
int Pt[N];
bool visited[N];

bool cmp(int a, int b){
	return T[a].nchild < T[b].nchild; 
}


int calChild(int u){
	int r = 0;
	if (T[u].c.size() == 0) return 0;
	for(int i=0;i<T[u].c.size();i++){
		v = T[u].c[i];
		if(v == T[u].p) continue;
		T[u].nchild += calChild(v) + 1;
	}
	return T[u].nchild;
}

void bfs(){
	queue <int> Q;
	Q.push(r);
	
	while (!Q.empty()){
		u = Q.front(); Q.pop();
		visited[u] = true;
		for(int i=0;i<T[u].c.size();i++){
			v = T[u].c[i];
			if (!visited[v]){
				T[v].p = u;
				Q.push(v);
			}
		}
	}
}

void hld(){
	T[0].nchild = N;
	T[1].c.push_back(0);	
	for(int i=1;i<=n;i++){
		sort(T[i].c.begin(), T[i].c.end(),cmp);
	}
		
	vector <int> S;
	S.push_back(r);
	int head = r;
	int pos = 1;
	int pos_= 0;
	
	while(!S.empty()){
		u = S.back(); S.pop_back();
		H[pos_++] = u;
		T[u].head = head;
		T[u].pos = pos ++;
		
		if(T[u].c.size() == 1){
			u = S.back();
			head = u;
			pos = 1;
		}
		else{
			for(int i=0;i<T[u].c.size()-1;i++){
			v = T[u].c[i];
			S.push_back(v);
			}
		}
	}
}

void built(int k, int l, int r) {
    if (l == r){
        IT[k] = 1e9;
        Pt[l] = k;
        return;
    }
    int mid = (l + r)/2;
    built(k*2, l, mid);
    built(k*2+1, mid+1, r);
    IT[k] = 1e9;
}
 
void update(int u){
    int k = Pt[Ph[u]];
    if (IT[k] != 1e9) IT[k] = 1e9; else IT[k] = T[u].pos;
    k/=2;
    while (k != 0){
        IT[k] = min(IT[2*k], IT[2*k+1]);
        k/=2;
    }
}
 

int query(int k, int l, int r, int i, int j){
    if (r < i or l > j) return 1e9;
    if (i <= l && r <= j)
    {
        return IT[k];
    }
    int mid = (l + r)/2;
    return min(query(k*2, l, mid, i, j),query(k*2+1, mid+1, r, i, j));
}

int main(){
	cin >> n >> q;
	for(int i=1;i<n;i++){
		cin >> a >> b;
		T[a].c.push_back(b);
		T[b].c.push_back(a);	
	}
	
	bfs();
	calChild(r);
	hld();
	for(int i=0;i<n;i++) cout << H[i] << " ";
	for(int i=0;i<n;i++){
		Ph[H[i]] = i;
	}
	// 1 2 9 8 6 7 5 4 3
	
	built(1,0,n);
	
	for(int i=0;i<q;i++){
		cin >> u >> v;
		if (u == 0) update(v);
		else{
			res = -1;
			b = 0;
			while (b!=1){
				b = T[v].head;
				ans = 1e9;
				ans = query(1,0,n,Ph[b],Ph[v]);
				if (ans != 1e9) res = H[Ph[b]+ans-1];
				v = T[b].p;
			}	
			cout << res << endl;
		}
	}
	cout << endl;
}
