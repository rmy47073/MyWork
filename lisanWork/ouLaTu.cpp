#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>

using namespace std;

const int N = 7; 
vector<int> adj[N]; 
bool edgeExist[N][N]; 
int degree[N]; 

//判断删除边后u和v是否连通
void dfsCheck(int u, int parent, vector<bool>& visited, bool tempEdge[N][N]) {
    //实现基础DFS遍历逻辑
    visited[u] = true;
    for (int v : adj[u]) {
        if (v != parent && tempEdge[u][v] && !visited[v]) {
            dfsCheck(v, u, visited, tempEdge);
        }
    }
}

//判断边是否为桥边
bool isBridge(int u, int v) {
    //复制边标记→临时删除边→DFS检查连通性→返回结果
    bool tempEdge[N][N];
    memcpy(tempEdge, edgeExist, sizeof(tempEdge));
    tempEdge[u][v] = tempEdge[v][u] = false; // 临时删除边
    
    vector<bool> visited(N, false);
    dfsCheck(u, -1, visited, tempEdge);
    return !visited[v];
}

//判断整个图是否连通
bool isGraphConnected() {
    // DFS遍历所有有边的顶点，判断是否连通
    vector<bool> visited(N, false);
    int start = -1;
    // 找第一个有边的顶点
    for (int i = 0; i < N; i++) {
        if (degree[i] > 0) {
            start = i;
            break;
        }
    }
    if (start == -1) return true; 

    // 匿名DFS函数
    function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (edgeExist[u][v] && !visited[v]) {
                dfs(v);
            }
        }
    };
    dfs(start);

    // 检查所有有边的顶点是否都被访问
    for (int i = 0; i < N; i++) {
        if (degree[i] > 0 && !visited[i]) {
            return false;
        }
    }
    return true;
}

bool isEulerGraph() {
    // 节后补充：1.所有顶点度数为偶数 2.图连通
    for (int i = 0; i < N; i++) {
        if (degree[i] % 2 != 0) {
            return false;
        }
    }
    return isGraphConnected();
}

vector<int> fleuryAlgorithm(int start) {
    vector<int> circuit; // 存储欧拉回路
    int currentV = start;
    int totalEdges = 0;

    for (int i = 0; i < N; i++) totalEdges += degree[i];
    totalEdges /= 2;
    int visitedEdges = 0;

    while (visitedEdges < totalEdges) {
        circuit.push_back(currentV); // 加入当前顶点
        int nextV = -1;

        for (int v : adj[currentV]) {
            if (edgeExist[currentV][v]) {
                if (!isBridge(currentV, v)) {
                    nextV = v;
                    break;
                }
            }
        }

        if (nextV == -1) {
            for (int v : adj[currentV]) {
                if (edgeExist[currentV][v]) {
                    nextV = v;
                    break;
                }
            }
        }

        edgeExist[currentV][nextV] = edgeExist[nextV][currentV] = false;
        visitedEdges++;
        currentV = nextV;
    }

    circuit.push_back(currentV); 
    return circuit;
}

void buildGraph() {
    // 初始化
    memset(edgeExist, false, sizeof(edgeExist));
    memset(degree, 0, sizeof(degree));
    for (int i = 0; i < N; i++) adj[i].clear();

    vector<pair<int, int>> edges = {
        {0,1}, {1,2}, {2,0}, {2,3}, {3,4}, {4,2}, {4,5}, {5,6}, {6,4}
    };
    for (auto& e : edges) {
        int u = e.first, v = e.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edgeExist[u][v] = edgeExist[v][u] = true;
        degree[u]++;
        degree[v]++;
    }
}

int main() {
    buildGraph();

    cout << "简单无向图的边集合：" << endl;
    cout << "E = {";
    bool first = true;
    for (int u = 0; u < N; u++) {
        for (int v : adj[u]) {
            if (u < v) { // 避免重复输出无向边
                if (!first) cout << ", ";
                cout << "(" << u << "," << v << ")";
                first = false;
            }
        }
    }
    cout << "}" << endl << endl;

    //判断是否为欧拉图
    if (!isEulerGraph()) {
        cout << "该图不是欧拉图！" << endl;
        return 0;
    }
    cout << "该图是欧拉图，欧拉回路为：" << endl;

    // 起始顶点
    int start = 0;
    for (int i = 0; i < N; i++) {
        if (degree[i] > 0) {
            start = i;
            break;
        }
    }

    //执行弗勒里算法
    vector<int> circuit = fleuryAlgorithm(start);

    //输出结果
    for (int i = 0; i < circuit.size(); i++) {
        cout << circuit[i];
        if (i != circuit.size() - 1) cout << " -> ";
    }
    cout << endl;

    return 0;
}