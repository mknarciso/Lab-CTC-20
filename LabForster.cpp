
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


using namespace std;
vector<string> subGroups;

//Fechamento: Verificar propriedade de fechamento do grupo
bool Fechamento (vector<string> table, vector<char> conjunto)
{
    //Fechamento: O elemento gerado por i * k deve pertencer ao conjunto
        bool fechamento = true;
        for (int i = 0; i < table.size() && fechamento; i++)
        {
            for (int j = 0; j < table.size() && fechamento; j++)
            {
                char temp = table [i][j];
                bool pertence = false;
                for (int k = 0; k < conjunto.size(); k++)
                {
                    if (conjunto[k] == temp)
                    {
                        pertence = true;
                        break;
                    }
                    else
                    {
                        if (k == conjunto.size() - 1 && !pertence)
                        {
                            fechamento = false;
                        }
                    }

                }

            }

        }
    return fechamento;
}

//ElementoInverso: Verificar propriedade do elemento inverso para todo elemento do grupo
bool ElementoInverso (vector<string> table)
{
    bool elementoInverso = true;
    for (int i = 0; i < table.size() && elementoInverso; i++)
    {
        elementoInverso = false;
        for (int j = 0; j < table[i].length(); j++)
        {
            if (table[i][j] == '1')
            {
                elementoInverso = true;
                break;
            }
        }
    }
    return elementoInverso;
}

//Associatividade: Verificar propriedade associativa em grupo
bool Associatividade (vector<string> table, vector<char> conjunto)
{
    bool associatividade = true;
    for (int i = 0; i < table.size() && associatividade; i++)
    {
        for (int j = 0; j < table.size() && associatividade; j++)
        {
            for (int k = 0; k < table.size() && associatividade; k++)
            {
                char temp1 = table[i][j];
                char temp2 = table [j][k];
                int result1, result2;
                for (int m = 0; m < table.size(); m++)
                {
                    if (temp1 == table[m][0])
                    {
                        result1 = m;
                    }
                    if (temp2 == table[m][0])
                    {
                        result2 = m;
                    }
                }
                if (table[result1][k] != table[i][result2])
                {
                    associatividade = false;
                }
            }
        }
    }
    return associatividade;
}

//Verificat: verifica propriedades de grupo
bool Verificar (vector<string> table, vector<char> conjunto, bool verificarAssociatividade)
{
    bool fechamento = Fechamento(table, conjunto);
    bool elementoInverso = ElementoInverso(table);
    bool associatividade = true;
    if (verificarAssociatividade) associatividade = Associatividade(table, conjunto);

    bool verificar = true;
    if (!fechamento || !elementoInverso || !associatividade)
    {
        verificar = false;
    }
    return verificar;
}
//findElementInTable: retorna o inteiro respectivo ao elemento (caractere)
int findElementInTable (vector<string> table, char element)
{
    for (int i = 0; i < table.size(); i++)
    {
        if (element == table[i][0])
            return i;
    }
}

//generateSubGroupTable: Gera a tabela do subgrupo
vector<string> generateSubGroupTable (string subGroup, vector <string> table)
{

    vector<string> subGroupTable;
    for (int i = 0; i < subGroup.size(); i++)
    {
        string rowSubGroupTable;
        int temp1 = findElementInTable (table, subGroup[i]);
        for (int j = 0; j < subGroup.size(); j++)
        {

            int temp2 = findElementInTable (table, subGroup[j]);
            rowSubGroupTable = rowSubGroupTable + table[temp1][temp2];
        }
        subGroupTable.push_back(rowSubGroupTable);
    }
    return subGroupTable;
}
//generateConjunto: gera o conjunto de elementos a partir da tabela
vector <char> generateConjunto (vector<char> conjunto, vector<string> table)
{
    for (int i = 0; i < table.size(); i++)

    {
        conjunto.push_back(table[i][0]);
    }
    return conjunto;
}

//generateSubGrupo: gera o subgrupo (string)
string generateSubGrupo (bool present[], int n, vector<char> conjunto)
{
    string newSubGroup;
    for (int i = 0; i < n +1; i++)
    {
        if (present[i] == true)
        {
            newSubGroup = newSubGroup + conjunto[i];
        }
    }
    return newSubGroup;
}

//testeSubGrupo: validando o fechamento para um possível subgrupo no backtrack
bool testeSubGrupo (bool present[],vector<string> table, int n)
{
    for (int i = 0; i <= n; i++)
    {
        if (present[i]){
            for (int j = 0; j <= n; j++){
                if (present[j]){

                    char temp1 = table[i][j];
                    int numb1 = findElementInTable(table, temp1);
                    if (numb1 <= n  && !present[numb1])
                        return false;
                }
            }
        }

    }
    return true;
}
//backtrack: algoritmo para percorrer todos os possíveis casos em uma árvore (com pruning)
void backtrack (bool present[], int n, vector<char> conjunto, vector<string> table)
{
    if (!present[0]) return;
    if (n == conjunto.size()){
        bool validade = testeSubGrupo(present, table, n-1);
        if (validade){ subGroups.push_back(generateSubGrupo(present,conjunto.size() -1, conjunto));}
        return;
    }
    present[n] = true;
    bool validade = testeSubGrupo(present, table, n);
    if (validade)
        backtrack(present, n+1, conjunto, table);

    present[n] = false;
    validade = testeSubGrupo(present, table, n);
    if (validade)
        backtrack (present, n+1, conjunto, table);

}

//generateSubGroupsBackTrack: faz a ponte entre a main e o algoritmo de backtrack
void generateSubGroupsBackTrack (vector <string> table, vector <char> conjunto)
{

    vector <string> subGroups;
    bool present [conjunto.size()];
    present [0] = true;
    backtrack (present,0, conjunto, table);
}
//main fuction
int main()
{
    for (int i = 1 ; i <= 8 ; i++)
    {
        //Read from files
        ifstream myFile;
        stringstream ss;
        ss << i;
        string nameTable = "table " + ss.str() + ".txt";
        const char* c = nameTable.c_str();
        myFile.open (c, ifstream::in);
        vector <string> table;
        string row;
        getline(myFile, row);
        table.push_back(row);
        while (!myFile.eof())
        {
            getline(myFile,row);
            table.push_back(row);
        }
        vector<string>::iterator it;
        cout << "Table " + ss.str() + ":" << endl;
        for (it = table.begin(); it != table.end(); it++)
        {
            cout << *it << endl;
        }
        cout << endl;
        //Conjunto:
        vector<char> conjunto;
        conjunto = generateConjunto(conjunto, table);
        //Verificação: Para cada elemento do grupo, checar as propriedades
        bool verificar = Verificar(table, conjunto, true);
        if (verificar){
            cout << "Verificacao: este eh um grupo!" << endl;
            //Encontrar subgrupos
            ofstream myFileOutput;
            stringstream ss;
            ss << i;
            string nameTable = "subgroups table " + ss.str() + ".txt";
            const char* c = nameTable.c_str();
            myFileOutput.open(c, ifstream::out);
            generateSubGroupsBackTrack (table, conjunto);
            vector<string>::iterator itSubGroup;
            for (itSubGroup = subGroups.begin(); itSubGroup != subGroups.end(); itSubGroup++)
            {
                myFileOutput << (*itSubGroup) << endl;
            }
            subGroups.clear();
        }
    }
    return 0;
}
