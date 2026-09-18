//11227125 第19組 趙軒正
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <unordered_set>
#include <stack>
using namespace std;            
const float INF = 2.0; // weight only in (0,1)

struct Student_Read {
    char ID1[12];
    char ID2[12];
    float weight;
};
struct Pair {
    string ID;
    float weight;
};
struct AdjList {
    string ID;
    vector<Pair> get;
};

bool isFloat(const string& str) {
    if (str[0] == '-' || str[0] == '+') {
        return false;
    }
    istringstream iss(str); 
    float f;
    iss >> f; // try to transform string to float
    return !iss.fail() && iss.eof(); // fail, check if it can be transformed. eof, check if there are other characters after the float.
}

class Graph {
  private:
    void Processing(stack<string>& s, unordered_set<string>& visited, vector<AdjList>& adjlist, const string& id) {
        int i = 0;
        for (; i < adjlist.size(); i++) {
            if (adjlist[i].ID == id) break;
        }
        if (i == adjlist.size()) return;
        visited.insert(adjlist[i].ID);
        for (const auto& g : adjlist[i].get) {
            if (visited.find(g.ID) == visited.end()) { 
                visited.insert(g.ID); 
                s.push(g.ID);
            }
        }
    }
    void Insert(unordered_set<string> &record, Pair &p, string ID, vector<AdjList> &adjlist) {
        if (record.find(ID) == record.end()) { // 沒此ID
            AdjList list_temp;
            list_temp.ID = ID;
            list_temp.get.push_back(p);
            record.insert(ID);
            adjlist.push_back(list_temp);
        } else { // 有此ID
            for (auto& item : adjlist) {
                if (item.ID == ID) {
                    item.get.push_back(p);
                    break;
                }
            }         
        }
    }
  public:
    /*================ output ================*/
    void Output_adj(vector<AdjList> &adjlist, string &filename, string &w) {
        filename = "pairs" + filename + "_" + w + ".adj";
        ofstream adj_file(filename);
        int node = 0;
        adj_file << "<<< There are " << adjlist.size() << " IDs in total. >>>" << endl;
        for (int i = 0; i < adjlist.size(); i++) {
            adj_file << "[" << setw(3) << i + 1 << "] " << adjlist[i].ID << ": \n";
            int j = 0;
            for (; j < adjlist[i].get.size(); j++) {
                adj_file << "\t(" << setw(2) << j + 1 << ") "
                << adjlist[i].get[j].ID << "," << setw(7) << adjlist[i].get[j].weight; 
                node++;
                if ((j + 1) % 12 == 0) adj_file << endl;
            }
            adj_file << endl;
        }
        cout << endl << "<<< There are " << adjlist.size() << " IDs in total. >>>" << endl;
        adj_file << "<<< There are " << node << " nodes in adjacency lists. >>>" << endl;
        cout << endl << "<<< There are " << node << " nodes in adjacency lists. >>>" << endl;
    }
    void Output_cc(vector<vector<string>> &connection, string &filename, string &w) {
        filename = "pairs" + filename + "_" + w + ".cc";
        ofstream cc_file(filename);
        cout << "\n<<< There are " << connection.size() << " connected components in total. >>>" << endl;
        cc_file << "<<< There are " << connection.size() << " connected components in total. >>>" << endl;
        for (int i = 0; i < connection.size(); i++) {
            cc_file << "{" << setw(2) << i + 1 << "} Connected Component: size = " << connection[i].size() << endl;
            cout << "{" << setw(2) << i + 1 << "} Connected Component: size = " << connection[i].size() << endl;
            int j = 0;
            for (; j < connection[i].size(); j++) {
                cc_file << " \t(" << setw(3) << j + 1 << ") " << connection[i][j]; 
                if ((j + 1) % 8 == 0) cc_file << endl;
            }
            cc_file << endl;
        }
    }
    void Output_ds(vector<pair<string, float>> &path, string &filename, string &w, string &ID) {
        filename = "pairs" + filename + "_" + w + ".ds";
        ofstream ds_file(filename, ios::app);
        ds_file << "\norigin: " << ID << endl;
        for (int i = 0; i < path.size(); i++) {
            ds_file << "\t(" << setw(2) << i + 1 << ") "
            << path[i].first << "," << setw(7) << path[i].second; 
            if ((i + 1) % 12 == 0) ds_file << endl;
            
        }
        ds_file << endl;
    }
    /*================ mission 0 ================*/
    vector<AdjList> Set_adj_list(vector<Student_Read> &student, float &thershold, unordered_set<string> &record) {
        vector<AdjList> adjlist;
        for (auto& stu : student) {
            if (stu.weight > thershold) continue;
            Pair p;
            p.ID = string(stu.ID2);   
            p.weight = stu.weight;
            if (record.size() == 0) {
                record.insert(stu.ID1);
                AdjList list_temp;
                list_temp.ID = stu.ID1;
                list_temp.get.push_back(p);
                adjlist.push_back(list_temp);
            } else {
                Insert(record, p, stu.ID1, adjlist);
            }
            p.ID = string(stu.ID1);
            p.weight = stu.weight;
            Insert(record, p, stu.ID2, adjlist);

        }
        /*sort adjlist*/
        sort(adjlist.begin(), adjlist.end(), [](const AdjList& a, const AdjList& b) {
            return a.ID < b.ID;  
        });
        /*sort every items' get*/
        for (auto& item : adjlist) {
            sort(item.get.begin(), item.get.end(), [](const Pair& a, const Pair& b) {
                return a.ID < b.ID; 
            });           
        }
        
        return adjlist;
    }
    /*================ algorithms ================*/
    vector<vector<string>> DFS(vector<AdjList>& adjlist) {
        vector<vector<string>> connection;
        stack<string> s;
        int i = 0;
        unordered_set<string> visited;
        for (auto& item : adjlist) {
            if (visited.find(item.ID) == visited.end()) { // visited沒有此ID就放入stack
                s.push(item.ID);
            } else {
                continue;
            }
            visited.insert(item.ID);
            vector<string> temp;
            while (!s.empty()) {
                string top = s.top();
                s.pop();
                Processing(s, visited, adjlist, top);
                temp.push_back(top);
            }
            connection.push_back(temp);
        }
        for (auto& item : connection) {
            sort(item.begin(), item.end());  
        }

        sort(connection.begin(), connection.end(), [](const vector<string>& a, const vector<string>& b) {
            if (a.size() != b.size())
                return a.size() > b.size(); 
            else
                return a[0] > b[0];   
        });
        return connection;
    }
    vector<pair<string, float>> Dijkstras(vector<string> &connection_nodes, vector<AdjList> &adjlist, string &ID) {
        vector<pair<string, float>> path;
        unordered_set<string> visited;
        for (auto& node : connection_nodes) {
            if (node == ID) path.push_back({node, 0});
            else path.push_back({node, INF});
        }
        // if every node visited, loop finish
        while (visited.size() != path.size()) {
            int i = 0;
            float cheapest = INF;
            while (i + 1 != path.size()) { // find cheapest unknown vertex              
                if (cheapest > path[i].second && visited.find(path[i].first) == visited.end()) {
                    break;
                }
                i++;
            }
            string processing_ID = path[i].first;
            float processing_weight = path[i].second;
            visited.insert(processing_ID); // mark this id visited
            // check neighbors of this node
            i = 0;
            while (processing_ID != adjlist[i].ID) i++; // find processing ID index in adjlist
            for (auto& get : adjlist[i].get) {
                if (visited.find(get.ID) != visited.end()) continue; // only process unvisited ID
                for (auto& p : path) {
                    if (p.first == get.ID) {
                        float cost = processing_weight + get.weight;
                        if (p.second > cost) { 
                            p.second = cost;
                            break;
                        }

                    }
                }
            }
        }
        return path;
    }
    float Prim(vector<string> &nodes, vector<AdjList> &adjlist, string &ID) {
        vector<pair<string, float>> path;
        unordered_set<string> visited;
        for (auto& node : nodes) {
            if (node == ID) path.push_back({node, 0});
            else path.push_back({node, INF});
        }
        // if every node visited, loop finish
        while (visited.size() != path.size()) {
            int i = 0, j = 0;
            float cheapest = INF;
            while (i != path.size()) { // find cheapest unknown vertex              
                if (cheapest > path[i].second && visited.find(path[i].first) == visited.end()) {
                    cheapest = path[i].second;
                    j = i;
                }
                i++;
            }
            i = j;
            string processing_ID = path[i].first;
            float processing_weight = path[i].second;
            visited.insert(processing_ID); // mark this id visited
            // check neighbors of this node
            i = 0;
            while (processing_ID != adjlist[i].ID) i++; // find processing ID index in adjlist
            for (auto& get : adjlist[i].get) {
                if (visited.find(get.ID) != visited.end()) continue; // only process unvisited ID
                for (auto& p : path) {
                    if (p.first == get.ID) {
                        float cost = get.weight;
                        if (p.second > cost) { 
                            p.second = cost;
                            break;
                        }

                    }
                }
            }
        }
        float cost = 0.0;
        for (auto& p : path) {
            cost += p.second;
        }
        return cost;
    }
    /*================ data ================*/   
    void List_all_id(vector<AdjList> &adjlist) {
        cout << endl;
        int i = 1;
        for (auto& item : adjlist) {
            cout << setw(12) << item.ID;
            //if (i % 8 == 0) cout << endl;
            i++;
        }
    }
    void Get_Connections(vector<vector<string>> &connection, string &ID, vector<string> &connection_nodes) {
        for (int i = 0; i < connection.size(); i++) {
            for (int j = 0; j < connection[i].size(); j++) {
                if (ID == connection[i][j]) {
                    connection_nodes = connection[i];
                    break;
                }
            }
        }
    } 
};

class File_Process {
  private:
    
  public:
    bool Have_file(string filename) { 
        filename = "pairs" + filename + ".bin";
        ifstream cur_file;
        cur_file.open(filename, ios::binary);
        if (cur_file.fail()) {
            cout << endl << "### " << filename << " does not exist! ###" << endl;
            cout << "### There is no graph and try it again. ###" << endl;
            cur_file.close();
            return false;
        }
        cur_file.close();
        return true;
    }

    vector<Student_Read> Get_Student_From_Bin(string filename) {
        vector<Student_Read> student;
        filename = "pairs" + filename + ".bin";
        ifstream cur_file;
        cur_file.open(filename, ios::binary);
        Student_Read temp;
        while (cur_file.read(reinterpret_cast<char*>(&temp), sizeof(Student_Read))) {
            student.push_back(temp);
        }
        return student;
    }

};

class Cmd_Control {
  private:
    File_Process FP;
    bool have_built = false;
    string filename, w;
    Graph graph;
    vector<AdjList> adjlist;
    vector<vector<string>> connection;
    unordered_set<string> record;

    bool Operate(float &weight, string &w) {
        string filenum;
        bool have_file = false;
        cout << "\nInput a file number ([0] Quit): ";
        cin >> filenum;
        if (filenum == "0") {
            cout << "### There is no graph and try it again. ###" << endl;
            return false;
        }
        
        filename = filenum;
        have_file = FP.Have_file(filename);
        if (!have_file) return false;
        adjlist.clear();
        connection.clear();
        record.clear();
        vector<Student_Read> student = FP.Get_Student_From_Bin(filename); 
        adjlist = graph.Set_adj_list(student, weight, record);
        
        graph.Output_adj(adjlist, filename, w);
        connection = graph.DFS(adjlist);
        graph.Output_cc(connection, filename, w);
        return true;
    }
  public:
    void list() {
        cout << endl << "**********  Graph data applications  *********";
        cout << endl << "* 1. Build a graph and connected components  *";
        cout << endl << "* 2. Find shortest paths by Dijkstra         *";
        cout << endl << "* 3. Generate minimum spanning tree(s)       *";
        cout << endl << "**********************************************";
        cout << endl << "Input a choice(0, 1, 2, 3) [0: QUIT]: ";
    }

    void Mission1() {
        float weight;
        while (true) {
            cout << "\nInput a real number in (0,1]: ";
            cin >> w;
            if (!isFloat(w)) continue;

            if (stof(w) > 1 || stof(w) <= 0) {
                cout << "\n### It is NOT in (0,1] ###\n";
                continue;
            } else {
                break;
            }
        }
        weight = stof(w);
        if (weight == 1) {
            w = "1.";
        }
        have_built = Operate(weight, w);
    }
    void Mission2() {
        if (!have_built) {
            cout << "### There is no graph and choose 1 first. ###\n";
            return;
        }
        string ID;
        while (true) {
            /*list all id from adjlist*/
            graph.List_all_id(adjlist);
            cout << "\nInput a student ID [0: exit] ";
            cin >> ID;
            if (ID == "0") break;

            if (record.find(ID) == record.end()) {
                cout << "\n### the student id does not exist! ###\n";
                continue;
            }
            cout << endl;
            /*find this id's connection nodes from connection*/
            vector<string> connection_nodes;
            graph.Get_Connections(connection, ID, connection_nodes); 
            /*using Dijkstras' algorithm*/
            vector<pair<string, float>> path = graph.Dijkstras(connection_nodes, adjlist, ID);
            /*sort id by weight from small to big*/
            sort(path.begin(), path.end(), [](const pair<string, float>& a, const pair<string, float>& b) {
                return a.second > b.second;  
            });
            /*output*/
            path.pop_back(); // erase itself
            graph.Output_ds(path, filename, w, ID);
        }
    } 
    void Mission3() {
        if (!have_built) {
            cout << "### There is no graph and choose 1 first. ###\n";
            return;
        }
        for (int i = 0; i < connection.size(); i++) {
            float cost = graph.Prim(connection[i], adjlist, connection[i][0]);
            cout << "The MST cost of connected component {" << setw(2) << i + 1 << "} = "
            << fixed << setprecision(2) << cost << endl;

        }
    }
};

int main() {
    Cmd_Control C;
    while (true) {
        string cmd;
        C.list();
        cin >> cmd;
        if (cmd == "0") {
            break;
        } else if (cmd == "1") {
            C.Mission1();
        } else if (cmd == "2") {
            C.Mission2();
        } else if (cmd == "3") { 
            C.Mission3();
        } else {
            cout << endl << "The command does not exist!" << endl;
        }
    }
}
