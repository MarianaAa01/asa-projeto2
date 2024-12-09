#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <limits>
using namespace std;


//_______________________________________________FUNÇÕES AUXILIARES_______________________________________________

//findPairs encontra todos os pares possíveis entre estações
vector<tuple<int, int>> findPairs(int numStations) {
    // Cria os pares
    vector<tuple<int, int>> pairs;
    for (int i = 1; i <= numStations; ++i) {
        for (int j = i + 1; j <= numStations; ++j) {
            pairs.push_back({i, j}); // Adiciona o par como um vetor de dois elementos
        }
    }
    return pairs;
}

// verifica conectividade com o DFS
bool isConnected(int numStations, const vector<tuple<int, int, int>>& connections) {
    // Cria matriz de adjacências
    vector<vector<int>> adjMatrix(numStations, vector<int>(numStations, 0));
    for (const auto& conn : connections) {
        int station1 = get<1>(conn) - 1; 
        int station2 = get<2>(conn) - 1; 
        adjMatrix[station1][station2] = 1;
        adjMatrix[station2][station1] = 1;
    }

    // Vetor para rastrear estações visitadas
    vector<bool> visited(numStations, false);

    // Pilha para implementar DFS
    vector<int> stack;
    stack.push_back(0); // Começa pela estação 0

    // Realiza DFS iterativo
    while (!stack.empty()) {
        int station = stack.back();
        stack.pop_back();

        if (!visited[station]) {
            visited[station] = true;

            // Adiciona todas as conexões da estação atual à pilha
            for (int neighbor = 0; neighbor < numStations; ++neighbor) {
                if (adjMatrix[station][neighbor] == 1 && !visited[neighbor]) {
                    stack.push_back(neighbor);
                }
            }
        }
    }
    // Verifica se todas as estações foram visitadas
    for (bool v : visited) {
        if (!v) return false; // Se alguma estação não foi visitada, o grafo não é conexo
    }

    return true;
}




//DFS ITERATIVO


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

    // Gera todos os pares (e1,e2) possíveis para depois analisarmos como ir de e1 para e2
    vector<tuple<int, int>> pairs = findPairs(numStations);


    
    // prints para eu ir percebendo o que está a acontecer:
    cout << "pares: " << endl;
    for (const auto& p : pairs) {
        cout << "(" << get<0>(p) << ", " << get<1>(p) << ")" << endl;
    }

    cout << "numStations: " << numStations << ", " << "numConections: " << numConections << ", " << "Número de linhas (l): " << numLines << endl;
    cout << "\n" << endl;
    cout << "Conexões:" << endl;
    for (const auto& conn : connections) {
        cout << "Linha " << get<0>(conn) << ": " << get<1>(conn) << " <-> " << get<2>(conn) << endl;
    }

    // Verifica se o grafo é conexo
    if (isConnected(numStations, connections)) {
        cout << "\nO grafo é conexo.\n";
    } else {
        cout << "\nO grafo não é conexo.\n";
        cout << "\n-1\n"; //"Se existirem pelo menos duas estações e1, e2 sem ligação entre si, output esperado é -1."
    }

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