#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <limits>
using namespace std;


//_______________________________________________FUNÇÕES AUXILIARES_______________________________________________
// verifica conectividade com o DFS
bool isConnected(int numStations, const vector<tuple<int, int, int>>& connections) {
    // matriz de adjacências
    vector<vector<int>> adjMatrix(numStations, vector<int>(numStations, 0));
    for (const auto& conn : connections) {
        int station1 = get<1>(conn) - 1; 
        int station2 = get<2>(conn) - 1; 
        adjMatrix[station1][station2] = 1;
        adjMatrix[station2][station1] = 1;
    }

    // vetor com estações visitadas
    vector<bool> visited(numStations, false);

    vector<int> stack;
    stack.push_back(0); // começamos na estação 0

    while (!stack.empty()) {
        int station = stack.back();
        stack.pop_back();

        if (!visited[station]) {
            visited[station] = true;

            // adiciona todas as conexões da estação atual à stack
            for (int neighbor = 0; neighbor < numStations; ++neighbor) {
                if (adjMatrix[station][neighbor] == 1 && !visited[neighbor]) {
                    stack.push_back(neighbor);
                }
            }
        }
    }
    // verifica se todas as estações foram visitadas
    for (bool v : visited) {
        if (!v) return false; // se alguma estação não foi visitada, o grafo não é conexo
    }

    return true;
}


//________________________________________________________________________________________________________________
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int numStations, numConections, numLines;
    cin >> numStations >> numConections >> numLines;

    //temos as conexões aqui neste vetor de tuplos
    vector<tuple<int, int, int>> connections;
    for (int i = 0; i < numConections; ++i) //este for é para guardar as conexões todas
    {
        int line, station1, station2;
        cin >> line >> station1 >> station2;
        connections.emplace_back(line, station1, station2); 
    }

    // prints para eu ir percebendo o que está a acontecer:
    /*
    cout << "pares: " << endl;
    for (const auto& p : pairs) {
        cout << "(" << get<0>(p) << ", " << get<1>(p) << ")" << endl;
    }
    
    cout << "numStations: " << numStations << ", " << "numConections: " << numConections << ", " << "numLines: " << numLines << endl;
    cout << "\n" << endl;
    cout << "Conexões:" << endl;
    for (const auto& conn : connections) {
        cout << "Linha " << get<0>(conn) << ": " << get<1>(conn) << " <-> " << get<2>(conn) << endl;
    }*/

    // Verifica se o grafo é conexo
    cout << (isConnected(numStations, connections) ? "0\n" : "-1\n");

    return 0;
}





/*
n: número de estações (n>=2) numStations
m: número de ligações (m>=0) numConections
l: número de linhas da rede de metro (l>=0) numLines

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