#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

vector<string> not_visited, set;


//Ordena o vetor de strings de acordo com o tamanho da string
vector<string> setSort(vector<string> original){
    vector<string> set;
    while (original.size()>0){
        int major_pos=0;
        for(int i=0; i<original.size();i++){
            if(original[i].size()>original[major_pos].size())
                major_pos = i;
        }
        set.push_back(original[major_pos]);
        original.erase(original.begin() + major_pos);
        major_pos=0;
    }
    return  set;
}

//Verifica se um conjunto é sub-grupo de outro, as condições já foram verificadas
bool isSubGroup(string sub, string group){
    for(int i=0; i<sub.size();i++){
        bool flag = false;
        for(int j=0; j<group.size();j++){
            if(sub.c_str()[i]==group.c_str()[j])
                flag=true;
        }
        if (!flag){
            return false;
        }
    }
    return true;
}

//Imprime uma conexão no formado GraphViz
string lineOk(string a, string b){
    string result="";
    result += "   \"{ ";
    result +=  a[0] ;
    result +=  " " ;
    for(int i=1;i<a.size();i++){
        result += ", " ;
        result +=  a[i] ;
        result +=  " " ;
    }
    result += "}\" -> \"{ " ;
    result +=  b[0] ;
    result +=  " " ;
    for(int i=1;i<b.size();i++){
        result += ", " ;
        result +=  b[i] ;
        result +=  " " ;
    }
    result += "}\"\n";
    return result;
}

//Verifica se todos os subgrupos já acharam um "pai"
bool allVisited(){
    bool result = true;
    for (int i=0; i<not_visited.size(); i++){
        if (not_visited[i]!=""){
            result = false;
        }
    }
    return result;
}

//Verifica se entre dois subgrupos já existe um caminho
bool hasPath(string a, string b){
    for(int i=0; i<set.size(); i++){
        if (a.size()<set[i].size()&&b.size()>set[i].size()){
            if(isSubGroup(a,set[i])&&isSubGroup(set[i],b)){
                return true;
            }
        }
    }
    return false;
}

int main(){
    vector<string> set_prev;
    string previous, actual;
    //Percorre os 8 arquivos fornecidos pela primeira parte do exercício
    for(int i=1 ; i<=8 ; i++ ){
        ifstream myFile;
        ofstream myFileOutput;
        cout << "Iteração: " << i <<endl; 
        stringstream ss;
        ss << i;
        myFile.open("subgroups table " + ss.str() + ".txt",ifstream::in);
        //Inicia o algoritmo se existir o arquivo
        if(myFile){
            myFileOutput.open("grafo " + ss.str() + ".txt",ifstream::out);
            // Gera o conjunto prévio de strings
            while (!myFile.eof()){
                string line;
                getline(myFile,line);
                set_prev.push_back(line);
            }
            // Ordena as strings por tamanho
            set = setSort(set_prev);
            // Vetor auxiliar para verificação de fim do programa
            not_visited = set;
            // Início do arquivo de saída
            myFileOutput << "digraph mygraph{\n";
            set.pop_back();
            // Verifica para todas as strings do conjunto quais caminhos possui
            while (set.size() >1){
                // Seleciona a última da fila
                actual = set.back();
                set.pop_back();
                int next_order = 1;
                // Percorre as outras strings do conjunto verificando caminhos possíveis, em níveis de ordem crescente
                while(!(allVisited()||next_order>30)){
                    for (int j=0; j<set.size(); j++){
                        // Adiciona cada caminho se for um Subgrupo, for da ordem esperada nesse loop, e não possuir outro caminho entre as pontas 
                        if (isSubGroup(actual,set[j])&&(set[j].size()==next_order)&&!hasPath(actual,set[j])){
                            //Com as condições cumpridas adiciona o arquivo à saída
                            //cout << lineOk(actual,set[j]);
                            myFileOutput << lineOk(actual,set[j]);
                        }
                    }
                    next_order++;
                }
                
            }
            //Fecha os arquivos
            myFileOutput << "}\n";    
            myFileOutput.close();
            set.clear();
            set_prev.clear();
        }
        myFile.close();
    }
}