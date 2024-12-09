#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

// algoritmo DFS
void dfs(int station, vector<bool>& visited, const vector<vector<int>>& adjMatrix) {
    visited[station] = true;

    // explora todas as conexões da estação atual
    for (size_t neighbor = 0; neighbor < adjMatrix.size(); ++neighbor) {
        if (adjMatrix[station][neighbor] == 1 && !visited[neighbor]) {
            dfs(neighbor, visited, adjMatrix);
        }
    }
}

// verifica conectividade com o DFS
bool isConnected(int n, const vector<tuple<int, int, int>>& connections) {
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
    vector<bool> visited(n, false);

    // matriz de adjacências com base nas conexões
    for (const auto& conn : connections) {
        int station1 = get<1>(conn) - 1; 
        int station2 = get<2>(conn) - 1; 
        adjMatrix[station1][station2] = 1;
        adjMatrix[station2][station1] = 1;
    }

    // começa a DFS pela estação 0
    dfs(0, visited, adjMatrix);

    // verifica se todas as estações foram visitadas
    for (bool v : visited) {
        if (!v) return false; // se alguma estação não foi visitada, o grafo não é conexo
    }

    return true;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n, m, l;
    cin >> n >> m >> l;

    // vetor de tuplos que guarda as conexões
    vector<tuple<int, int, int>> connections;

    for (int i = 0; i < m; ++i)
    {
        int line, station1, station2;
        cin >> line >> station1 >> station2;
        connections.emplace_back(line, station1, station2);
    }
/*
    cout << "\n" << endl;
    cout << "Número de estações (n): " << n << endl;
    cout << "Número de ligações (m): " << m << endl;
    cout << "Número de linhas (l): " << l << endl;
    cout << "\n" << endl;
    cout << "Conexões:" << endl;
    for (const auto& conn : connections) {
        cout << "Linha " << get<0>(conn) << ": Estação " << get<1>(conn) << " <-> Estação " << get<2>(conn) << endl;
    }
    cout << "\n" << endl;
    cout << "Como as conexões estão guardadas num vetor de tuplos:" << endl;
    for (const auto& conn : connections) {
        cout << "(" << get<0>(conn) << ", " << get<1>(conn) << ", " << get<2>(conn) << ")" << endl;
    }*/

    // Verifica se o grafo é conexo
    if (isConnected(n, connections)) {
        cout << "\nO grafo é conexo.\n";
    } else {
        //cout << "\nO grafo não é conexo.\n";
        cout << "\n-1\n";
    }

    return 0;
}



/*
n: número de estações (n>=2)
m: número de ligações (m>=0)
l: número de linhas da rede de metro (l>=0)

cada linha (i) do ficheiro tem l, x, y (x está diretamente ligada a y na linha l)

entre duas estações podem existir múltiplas conexões, correspondendo a diferentes linhas de metro

Exemplo 1:

    7 8 3    n=7; m=8; l=3
    3 2 1    a estação 3 está diretamente ligada à estação 2 na linha 1
    2 7 1
    7 5 1
    2 6 2
    6 4 2
    4 1 2
    4 1 3
    1 5 3


Exemplo 2:

    7 6 3 n=7; m=6; l=3
    3 2 1
    2 7 1
    7 5 1
    2 6 2
    6 4 2
    1 5 3


Para testar:
7 8 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
4 1 2
4 1 3
1 5 3


7 6 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
1 5 3


grafo não conexo:
8 7 3
3 2 1
2 7 1
7 5 1
2 6 2
6 4 2
1 4 3
1 5 3
*/