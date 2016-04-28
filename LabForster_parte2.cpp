#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

vector<string> not_visited;

vector<string> setSort(vector<string> original){
    vector<string> set;
    //cout << endl << endl << original.size() << endl;
    while (original.size()>0){

        int major_pos=0;
        for(int i=0; i<original.size();i++){
            if(original[i].size()>original[major_pos].size())
                major_pos = i;
        }
        //cout << "Major Pos: " << major_pos << endl;
        //cout << "Size: " << original.size() << endl;
        set.push_back(original[major_pos]);
        original.erase(original.begin() + major_pos);
        major_pos=0;
    }
    return  set;
}

bool isSubGroup(string sub, string group){
    for(int i=0; i<sub.size();i++){
        bool flag = false;
        for(int j=0; j<group.size();j++){
            if(sub.c_str()[i]==group.c_str()[j])
                flag=true;
        }
        if (!flag){
            //cout << sub << " não é subgrupo de " << group << endl;
            return false;
        }
    }
    //cout << sub << " é subgrupo de " << group << endl;
    return true;
    
}

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

void markVisited(string a, int b){
    cout << "Retirar da string " << not_visited[b] << " os campos visitados: " << a << endl;
    //cout << a.size() << endl << not_visited[b][0] << endl;
    for(int j=0; j<a.size();){
        for(int i=0; i<not_visited[b].size();i++){
            int k = i;
            //cout << not_visited[b].c_str()[i] << " = " << a[0] << "?\n" ; 
            if(a[0]==not_visited[b].c_str()[i]){
                //cout << "SIM!" << endl;
                not_visited[b].erase(not_visited[b].begin() + k);
                //i--;
            }
        }
        a.erase(a.begin());
    }
    cout << "Resultado: " <<not_visited[b]<< endl;
    //cout << lineOk("XXXXXXXXXXXXX",not_visited[b]);
}

bool allVisited(){
    bool result = true;
    for (int i=0; i<not_visited.size(); i++){
        if (not_visited[i]!=""){
            result = false;
            cout  << "Ainda não visitado em "<<i<<":"<<not_visited[i]<<endl;
        }
    }

    return result;
}

int main(){
    //isSubGroup("abc","acdb");
    //isSubGroup("1abc","abc ");
    vector<string> set, set_prev;
    string previous, actual;
    for(int i=1 ; i<=8 ; i++ ){
        ifstream myFile;
        ofstream myFileOutput;
        cout << "Iteração: " << i <<endl; 
        stringstream ss;
        ss << i;
        myFile.open("subgroups table " + ss.str() + ".txt",ifstream::in);
        if(myFile){
            myFileOutput.open("grafo " + ss.str() + ".txt",ifstream::out);
            while (!myFile.eof()){
                string line;
                getline(myFile,line);
                set_prev.push_back(line);
            }
            set = setSort(set_prev);
            not_visited = set;
            myFileOutput << "digraph mygraph{\n";
            set.pop_back();
            cout << set.back()<<endl;
            while (set.size() >0){
                //actual = set[set.size()-1];
                //set.pop_back();
                actual = set[0];
                set.erase(set.begin());
                int next_order = set.back().size() - 1;
                /*for (int j=0; j<set.size(); j++){
                    if(next_order==-1||(set[j].size()<next_order&&set[j].size()>actual.size()))
                        next_order=set[j].size();
                }*/
                while(!(allVisited()||next_order<0)){
                    for (int j=0; j<set.size(); j++){
                        if (isSubGroup(actual,set[j])&&(set[j].size()==next_order)&&set[j]!=""){
                            //cout << lineOk(actual,set[j]);
                            myFileOutput << lineOk(actual,set[j]);
                            markVisited(actual,j);
                            cout << j << endl;
                        }
                    }
                    cout << next_order << endl;
                    next_order--;
                }
                
            }
            /*previous = set[set.size()-1];
            set.pop_back();
            while (set.size() > 0){
                actual = set[set.size()-1];
                set.pop_back(); 
                myFileOutput << "   \"{ " << previous[0] << " " ;
                for(int i=1;i<previous.size();i++)
                    myFileOutput << ", " << previous[i] << " " ;
                myFileOutput << "}\" -> \"{ " << actual[0] << " " ;
                for(int i=1;i<actual.size();i++)
                    myFileOutput << ", " << actual[i] << " " ;
                myFileOutput << "}\"\n";
                previous = actual;
            }*/
            /*set.pop_back();
            previous = set[set.size()-1];
            set.pop_back();
            while (set.size() > 0){
                actual = set[set.size()-1];
                set.pop_back(); 
                myFileOutput << "   \"{ " << previous[0] << " " ;
                for(int i=1;i<previous.size();i++)
                    myFileOutput << ", " << previous[i] << " " ;
                myFileOutput << "}\" -> \"{ " << actual[0] << " " ;
                for(int i=1;i<actual.size();i++)
                    myFileOutput << ", " << actual[i] << " " ;
                myFileOutput << "}\"\n";
                previous = actual;
            }*/
            myFileOutput << "}\n";    
            myFileOutput.close();
            set.clear();
            set_prev.clear();
        }
        myFile.close();
    }
}
