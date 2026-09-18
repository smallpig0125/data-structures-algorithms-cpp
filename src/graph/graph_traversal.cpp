//11227125 第19組 趙軒正
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring> // for strcpy
#include <iomanip>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <stack>
using namespace std;

struct Student_Read {
    char putID[12];
    char getID[12];
    float weight;
};
struct Pair {
    char getID[12];
    float weight;
};
struct Student_List {
    char ID[12];
    vector<Pair> get;
};

bool isFloat(const string& str) {
    if (str[0] == '-' || str[0] == '+') {
        return false;
    }
    istringstream iss(str);
    float f;
    iss >> f;
    return !iss.fail() && iss.eof(); 
}

class Graph {
  private:
    void Processing(queue<Pair>& q, unordered_set<string>& visited, vector<Student_List>& student_list, const string& id) {
        int i = 0;
        for (; i < student_list.size(); i++) {
            string currentID(student_list[i].ID);
            if (currentID == id) break;
        }
        if (i == student_list.size()) return;

        for (const auto& g : student_list[i].get) {
            string getid(g.getID);
            if (visited.find(getid) == visited.end()) { 
                visited.insert(getid); 
                q.push(g);
            }
        }
    }
    void Processing_inf(stack<Pair>& s, unordered_set<string>& visited, vector<Student_List>& student_list, const string& id, float weight) {
        int i = 0;
        for (; i < student_list.size(); i++) {
            string currentID(student_list[i].ID);
            if (currentID == id) break;
        }
        if (i == student_list.size()) return;
        for (const auto& g : student_list[i].get) {
            string getid(g.getID);
            if (visited.find(getid) == visited.end() && g.weight >= weight) { 
                visited.insert(getid); 
                s.push(g);
            }
        }
    }
  public:
    vector<Student_List> Set_adj_list(vector<Student_Read> student) {
        vector<Student_List> student_list;
        for (auto& stu : student) {
            Pair p;
            strcpy(p.getID, stu.getID);   
            p.weight = stu.weight;
            bool have_data = false;
            for (auto& stu_list : student_list) { // process putID
                if (strcmp(stu.putID, stu_list.ID) == 0) {
                    int i = 0;
                    for (; i < stu_list.get.size() && strcmp(p.getID, stu_list.get[i].getID) > 0; i++);
                    stu_list.get.insert(stu_list.get.begin() + i, p);
                    have_data = true;
                    break;
                }
            }
            if (!have_data) {
                Student_List sl;   
                strcpy(sl.ID, stu.putID);
                sl.get.push_back(p);
                if (student_list.size() == 0) {
                    student_list.push_back(sl);
                } else {
                    int i = 0;
                    for (; i < student_list.size() && strcmp(sl.ID, student_list[i].ID) > 0; i++);
                    student_list.insert(student_list.begin() + i, sl);
                }

            }
            have_data = false;

            for (auto& stu_list : student_list) { // process getID
                if (strcmp(stu.getID, stu_list.ID) == 0) {
                    have_data = true;
                    break;
                }
            }
            if (!have_data) {
                Student_List sl;
                strcpy(sl.ID, stu.getID);
                int i = 0;
                for (; i < student_list.size() && strcmp(sl.ID, student_list[i].ID) > 0; i++);
                student_list.insert(student_list.begin() + i, sl);  
            }  
        }
        return student_list;
    }
    void Output_adj(vector<Student_List> student_list, string filename) {
        filename = "pairs" + filename + ".adj";
        ofstream adj_file(filename);
        int node = 0;
        adj_file << "<<< There are " << student_list.size() << " IDs in total. >>>" << endl;
        for (int i = 0; i < student_list.size(); i++) {
            adj_file << "[" << setw(3) << i + 1 << "] " << student_list[i].ID << ": \n";
            int j = 0;
            for (; j < student_list[i].get.size(); j++) {
                
                adj_file << "\t(" << setw(2) << j + 1 << ") "
                << student_list[i].get[j].getID << "," << setw(7) << student_list[i].get[j].weight; 
                node++;
                if (j == 11) adj_file << endl;
            }
            adj_file << endl;
        }
        adj_file << "<<< There are " << node << " nodes in total. >>>\n"; 
        cout << endl << "<<< There are " << student_list.size() << " IDs in total. >>>" << endl;
        cout << endl << "<<< There are " << node << " nodes in total. >>>" << endl;
    }
    void Output_cnt_inf(vector<Student_List> stu_list, string filename, int type) { //type 2: cnt, 3: inf
        if (type == 2) filename = "pairs" + filename + ".cnt";
        if (type == 3) filename = "pairs" + filename + ".inf";
        ofstream cnt_file(filename);
        vector<Student_List> student_list;
        for (int i = 0; i < stu_list.size(); i++) {
            int j = 0;
            for (; j < student_list.size(); j++) {
                if (stu_list[i].get.size() > student_list[j].get.size()) {
                    break;
                }
            }
            student_list.insert(student_list.begin() + j, stu_list[i]);
        }
        cout << "\n<<< There are " << student_list.size() << " IDs in total. >>>" << endl;
        cnt_file << "<<< There are " << student_list.size() << " IDs in total. >>>" << endl;
        for (int i = 0; i < student_list.size(); i++) {
            cnt_file << "[" << setw(3) << i + 1 << "] " << student_list[i].ID << "(" << student_list[i].get.size() << "): \n";
            int j = 0;
            for (; j < student_list[i].get.size(); j++) {
                
                cnt_file << "\t(" << setw(2) << j + 1 << ") "
                << student_list[i].get[j].getID; 
                if ((j + 1) % 12 == 0) cnt_file << endl;
            }
            cnt_file << endl;
        }

    }
    vector<Student_List> BFS(vector<Student_List>& student_list) {
        vector<Student_List> connection;
        queue<Pair> q;
        int i = 0;

        for (const auto& stu_list : student_list) {
            unordered_set<string> visited;  
            string id(stu_list.ID);
            connection.push_back(stu_list);
            visited.insert(id);  
            for (const auto& g : stu_list.get) {
                string getid(g.getID);
                visited.insert(getid); 
                q.push(g);
            }
            while (!q.empty()) {
                Pair front = q.front();
                string frontID(front.getID);
                Processing(q, visited, student_list, frontID);
                auto it = lower_bound(connection[i].get.begin(), connection[i].get.end(), front, [](const Pair& a, const Pair& b) {
                    return strcmp(a.getID, b.getID) < 0; 
                });
                if (it == connection[i].get.end() || strcmp(it->getID, front.getID) != 0) {
                    connection[i].get.insert(it, front); 
                }
                q.pop();
            }
            i++;
        }
        return connection;
    }
    vector<Student_List> DFS(vector<Student_List>& student_list, float weight) {
        vector<Student_List> connection;
        stack<Pair> s;
        int i = 0;

        for (const auto& stu_list : student_list) {
            unordered_set<string> visited;  
            string id(stu_list.ID);
            Student_List sl;
            strcpy(sl.ID, stu_list.ID);
            connection.push_back(sl);
            visited.insert(id);  
            for (const auto& g : stu_list.get) {
                string getid(g.getID);
                visited.insert(getid); 
                if (g.weight < weight) continue;
                s.push(g);
            }
            if (s.empty()) {
                connection.pop_back();
                continue;
            }
            while (!s.empty()) {
                Pair top = s.top();
                s.pop();
                if (top.weight < weight) continue;
                string topID(top.getID);
                
                Processing_inf(s, visited, student_list, topID, weight);
                auto it = lower_bound(connection[i].get.begin(), connection[i].get.end(), top, [](const Pair& a, const Pair& b) {
                    return strcmp(a.getID, b.getID) < 0; 
                });
                if (it == connection[i].get.end() || strcmp(it->getID, top.getID) != 0) {
                    connection[i].get.insert(it, top); 
                }
                
            }
            i++;
        }
        return connection;        
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
    string filename;
    Graph graph;
    vector<Student_List> student_list;
    bool Operate() {
        vector<vector<string>> data;
        string filenum;
        bool have_file = false;
        cout << "\nInput a file number ([0] Quit): ";
        cin >> filenum;
        if (filenum == "0") return false;
        filename = filenum;
        have_file = FP.Have_file(filename);
        if (!have_file) return false;
        student_list.clear();
        vector<Student_Read> student = FP.Get_Student_From_Bin(filename); 
        student_list = graph.Set_adj_list(student);
        graph.Output_adj(student_list, filename);
        return true;
    }
  public:
    void list() {
        cout << endl << "**** Graph data manipulation *****";
        cout << endl << "* 0. QUIT                        *";
        cout << endl << "* 1. Build adjacency lists       *";
        cout << endl << "* 2. Compute connection counts   *";
        cout << endl << "* 3. Estimate influence values   *";
        cout << endl << "**********************************";
        cout << endl << "Input a choice(0, 1, 2, 3): ";
    }

    void Mission1() {
        have_built = Operate();
    }
    void Mission2() {
        if (!have_built) {
            cout << "### There is no graph and choose 1 first. ###\n";
            return;
        }
        vector<Student_List> conncection = graph.BFS(student_list);
        graph.Output_cnt_inf(conncection, filename, 2);
    }
    void Mission3() {
        if (!have_built) {
            cout << "### There is no graph and choose 1 first. ###\n";
            return;
        }
        float weight;
        while (true) {
            string w = "NULL";
            cout << "\nInput a real number in [0,1]: ";
            cin >> w;
            if (!isFloat(w)) continue;
            weight = stof(w);
            if (weight < 0 || weight > 1) {
                cout << "\n### " << weight << " is NOT in [0,1] ###\n";
            } else {
                break;
            }
        }
        vector<Student_List> conncection = graph.DFS(student_list, weight);
        graph.Output_cnt_inf(conncection, filename, 3);
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
            cout << endl << "Command does not exist!" << endl;
        }
    }
}
