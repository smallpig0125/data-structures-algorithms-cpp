//11227125 趙軒正
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream> // for divide string's words
#include <algorithm>
using namespace std;

struct Data {
    int serial_num;
    string school;
    string major;
    string day_or_night;
    string degree;
    string student_num;
};

class T3tree {
  public:
    vector<vector<struct Data>> major; // 每個 vector 表示同一個 key 對應的資料集
    T3tree* left = nullptr;
    T3tree* middle_tem = nullptr;
    T3tree* middle = nullptr;
    T3tree* right = nullptr;
    T3tree* parent = nullptr;

    // 建構子：建立 root 節點
    T3tree(vector<struct Data> data) {
        major.push_back(data);
    }
    // 建構子：非 root 節點，指定 parent
    T3tree(T3tree* parent, vector<struct Data> data) {
        major.push_back(data);
        this->parent = parent;
    }
    // 建構子：僅設定 parent
    T3tree(T3tree* parent) {
        this->parent = parent;
    }
};

class AVLtree {
  public:
    AVLtree* left = nullptr;
    AVLtree* right = nullptr;
    vector<struct Data> data;
    int height = 1;
    AVLtree(struct Data data) {
        this->data.push_back(data);
    }
};

class T3treeSimulator {
  private:
    T3tree* root = nullptr;

    void Split(T3tree* node, T3tree* left, T3tree* right) {
        if (node->parent == nullptr) {
            T3tree* newroot = new T3tree(node->parent, node->major[1]);
            left->parent = newroot;
            right->parent = newroot;
            newroot->left = left;
            newroot->right = right;
            root = newroot;
            delete node;
        } else if (node->parent->major.size() == 1) {
            if (node->major[1][0].major > node->parent->major[0][0].major) { // right
                node->parent->major.push_back(node->major[1]);
                node->parent->middle = left;
                left->parent = node->parent;
                right->parent = node->parent;
                node->parent->right = right;
            } else { // left
                node->parent->major.insert(node->parent->major.begin(), node->major[1]);
                node->parent->middle = right;
                right->parent = node->parent;
                left->parent = node->parent;
                node->parent->left = left;
            }
        } else if (node->parent->major.size() == 2) {
            vector<T3tree*> vec_tem;
            if (node->major[0][0].major < node->parent->major[0][0].major) { // left split
                node->parent->major.insert(node->parent->major.begin(), node->major[1]);
                vec_tem = {left, right, node->parent->middle, node->parent->right};
            } else if (node->major[0][0].major > node->parent->major[1][0].major) { // right split
                node->parent->major.push_back(node->major[1]);
                vec_tem = {node->parent->left, node->parent->middle, left, right};
            } else { // middle split
                node->parent->major.insert(node->parent->major.begin() + 1, node->major[1]);
                vec_tem = {node->parent->left, left, right, node->parent->right};
            }
            T3tree* cur_left = new T3tree(node->parent, node->parent->major[0]);
            T3tree* cur_right = new T3tree(node->parent, node->parent->major[2]);
        
            cur_left->left = vec_tem[0];
            cur_left->right = vec_tem[1];
            cur_right->left = vec_tem[2];
            cur_right->right = vec_tem[3];
            vec_tem[0]->parent = cur_left;
            vec_tem[1]->parent = cur_left;
            vec_tem[2]->parent = cur_right;
            vec_tem[3]->parent = cur_right;
            delete node;


            Split(node->parent, cur_left, cur_right);
        }
    }


    void Insert(T3tree* node, vector<struct Data> data) {
        if (node->major.size() == 1) { // 節點只有一個 key
            if (data[0].major < node->major[0][0].major) { // 插入到左邊
                node->major.insert(node->major.begin(), data);
            } else { // 插入到右邊
                node->major.push_back(data);
            }
        } else if (node->major.size() == 2) { // 節點已有兩個 key，需要分裂
            if (data[0].major < node->major[0][0].major) {
                node->major.insert(node->major.begin(), data);
            } else if (data[0].major > node->major[1][0].major) {
                node->major.push_back(data);
            } else {
                node->major.insert(node->major.begin() + 1, data);
            }
            T3tree* left = new T3tree(nullptr, node->major[0]);
            T3tree* right = new T3tree(nullptr, node->major[2]);
            Split(node, left, right);
        }
    }

    int TreeHeight(T3tree* n) {
        int i = 0;
        while (n != nullptr) {
            i++;
            n = n->left;
        }
        return i;
    }

    void Traverse(T3tree* node, vector<struct Data> data) {
        while (node != nullptr) {
            if (Samenode(node, data)) {
                break;
            }
            if (node->left == nullptr) {
                Insert(node, data);
                break;
            }
            if (node->major.size() == 1) {
                if (data[0].major < node->major[0][0].major) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            } else if (node->major.size() == 2) {
                if (data[0].major < node->major[0][0].major) {
                    node = node->left;
                } else if (data[0].major > node->major[1][0].major) {
                    node = node->right;
                } else {
                    node = node->middle;
                }
            }
        }
    }

    bool Samenode(T3tree* node, vector<struct Data> data) {
        if (node->major.size() == 1) {
            if (node->major[0][0].major == data[0].major) {
                node->major[0].push_back(data[0]);
                return true;
            }
        } else if (node->major.size() == 2) {
            if (node->major[0][0].major == data[0].major) {
                node->major[0].push_back(data[0]);
                return true;
            } else if (node->major[1][0].major == data[0].major) {
                node->major[1].push_back(data[0]);
                return true;
            }
        }
        return false;
    }

  public:
    void Create(int serial_num, vector<string> d) {
        struct Data data;
        data.serial_num = serial_num;
        data.school = d[1];
        data.major = d[3];
        data.day_or_night = d[4];
        data.degree = d[5];
        data.student_num = d[6];
        vector<struct Data> tem;
        tem.push_back(data);
        if (root == nullptr) {
            root = new T3tree(tem);
        } else {
            Traverse(root, tem);
        }
    }

    void Print(T3tree* node) {
        int count = 1; 
        vector<vector<struct Data>> sort_print;
        cout << "Tree height = " << TreeHeight(node) << endl;
    
        sort_print.push_back(node->major[0]);
        if (node->major.size() == 2) {
            for (int i = 0; i < node->major[1].size(); i++) {
                sort_print[0].push_back(node->major[1][i]);
            }
        }
        // 對每一個 vector<struct Data> 進行內部排序
        for (auto& vec : sort_print) {
            // 使用 std::sort 來排序 vec 中的資料，依據 serial_num 升冪排序
            sort(vec.begin(), vec.end(), [](const struct Data& a, const struct Data& b) {
                return a.serial_num < b.serial_num;  // 比較 serial_num 進行升冪排序
            });
        }

        // 輸出排序後的資料
        for (int i = 0; i < sort_print.size(); i++) {
            for (int j = 0; j < sort_print[i].size(); j++) {
                cout << count << ": ";
                cout << "[" << sort_print[i][j].serial_num << "] ";
                cout << sort_print[i][j].school << ", ";
                cout << sort_print[i][j].major << ", ";
                cout << sort_print[i][j].day_or_night << ", ";
                cout << sort_print[i][j].degree << ", ";
                cout << sort_print[i][j].student_num << endl;
                count++;
            }
        }
        cout << endl;
    }
    T3tree* Get23tree() {
        return root;
    }
    void Clear() {
        root = nullptr;
    }
};

class AVLtreeSimulator {
    private:
        AVLtree* root = nullptr;
    
        int TreeHeight(AVLtree* node) {
            if (node == nullptr) {
                return 0;  // 節點為空，高度為 0
            }
            return node->height;  // 返回節點的高度
        }
    
        // 右旋（LL 旋轉）
        AVLtree* LL(AVLtree* node) {
            AVLtree* leftChild = node->left;
            AVLtree* rightSubtree = leftChild->right;
    
            // 旋轉
            leftChild->right = node;
            node->left = rightSubtree;
    
            // 更新高度
            node->height = max(TreeHeight(node->left), TreeHeight(node->right)) + 1;
            leftChild->height = max(TreeHeight(leftChild->left), TreeHeight(leftChild->right)) + 1;
    
            return leftChild;  // 返回新的根節點
        }
    
        // 左旋（RR 旋轉）
        AVLtree* RR(AVLtree* node) {
            AVLtree* rightChild = node->right;
            AVLtree* leftSubtree = rightChild->left;
    
            // 旋轉
            rightChild->left = node;
            node->right = leftSubtree;
    
            // 更新高度
            node->height = max(TreeHeight(node->left), TreeHeight(node->right)) + 1;
            rightChild->height = max(TreeHeight(rightChild->left), TreeHeight(rightChild->right)) + 1;
    
            return rightChild;  // 返回新的根節點
        }
    
        // 插入節點並保持平衡
        AVLtree* Insert(AVLtree* node, Data data) {
            if (!node) {
                return new AVLtree(data);  // 如果節點為空，插入新節點
            }
    
            // 根據 school 比較來決定插入位置
            if (data.school < node->data[0].school) {
                node->left = Insert(node->left, data);
            } else if (data.school > node->data[0].school) {
                node->right = Insert(node->right, data);
            } else {
                node->data.push_back(data);  // 如果已有相同資料，則直接加入
                return node;
            }
    
            // 更新高度
            node->height = 1 + max(TreeHeight(node->left), TreeHeight(node->right));
            int bf = TreeHeight(node->left) - TreeHeight(node->right);
            // 平衡操作
            if (bf > 1) {
                if (data.school < node->left->data[0].school) {
                    return LL(node);  // 左側過高，進行右旋
                } else {
                    node->left = RR(node->left);  // 左側不平衡，先左旋再右旋
                    return LL(node);
                }
            } else if (bf < -1) {
                if (data.school > node->right->data[0].school) {
                    return RR(node);  // 右側過高，進行左旋
                } else {
                    node->right = LL(node->right);  // 右側不平衡，先右旋再左旋
                    return RR(node);
                }
            }
    
            return node;  // 返回更新後的節點
        }
    
    public:
        // 創建新節點並插入
        void Create(int serial_num, vector<string> d) {
            Data data{serial_num, d[1], d[3], d[4], d[5], d[6]};
            root = Insert(root, data);  // 插入新節點並保持平衡
        }
    
        // 打印樹的資料
        void Print(bool have_built) {
            if (have_built) {
                cout << "### AVL tree has been built. ###\n";
            }
            cout << "Tree height = " << TreeHeight(root) << endl;
            int count = 1;
            for (int i = 0; i < root->data.size(); i++) {
                cout << count << ": ";
                cout << "[" << root->data[i].serial_num << "] ";
                cout << root->data[i].school << ", ";
                cout << root->data[i].major << ", ";
                cout << root->data[i].day_or_night << ", ";
                cout << root->data[i].degree << ", ";
                cout << root->data[i].student_num << endl;
                count++;
            }
            cout << endl;
        }
    
        // 獲取 AVL 樹根
        AVLtree* GetAVLtree() {
            return root;
        }
    };
    
    
    
  

class File_Process {
  private:
    ifstream cur_file;
    vector<string> line;
    vector<vector<string>> data;
  public:
    bool Have_file(string filename) {
        cur_file.clear();
        line.clear();
        data.clear();
        cur_file.open(filename);
        if (cur_file.fail()) {
            cout << endl << "### " << filename << " does not exist! ###" << endl;
            return false;
        }
        return true;
    }
    // 讀取檔案
    void File_Input() {
        string str, foreword;
        for (int i = 0; i < 3; i++) {
            getline(cur_file, foreword);
        }
        while (getline(cur_file, str)) {
            line.push_back(str);
        }
        cur_file.close();
    }

    // 將讀入的字串 分開數字
    vector<vector<string>> Divide_line() {
        for (int i = 0; i < line.size(); i++) {
            std::stringstream ss(line[i]);
            vector<string> tem;
            string word;
            while (getline(ss, word, '\t')) {
                tem.push_back(word);
            }
            data.push_back(tem);
        }
        return data;
    }
};

class Cmd_Control {
  private:
    File_Process FP;
    bool had_executed = false;
    bool have_built = false;
    vector<vector<string>> data;
    T3treeSimulator T;
    AVLtreeSimulator AVL;
    bool Operate() {
        data.clear();
        T.Clear();
        string filenum;
        bool have_file = false;
        while (!have_file) {
            cout << "\nInput a file number ([0] Quit): ";
            cin >> filenum;
            if (filenum == "0") {
                break;
            }
            string filename = "input" + filenum + ".txt";
            have_file = FP.Have_file(filename);
        }
        if (filenum == "0") {
            return false;
        }
        FP.File_Input();
        data = FP.Divide_line();
        if (data.size() == 0) {
            return false;
        }
        return true;
    }

    int Number_Transfrom(string num) {
        int n = 0, ten_digit = 1;
        for (int i = num.size() - 1; i >= 0; i--) {
            if (num[i] != '"' && num[i] != ',') {
                n += (num[i] - '0') * ten_digit;
                ten_digit *= 10;
            }
        }
        return n;
    }
  public:
    void list() {
        cout << endl << "*** Search Tree Utilities **";
        cout << endl << "* 0. QUIT                  *";
        cout << endl << "* 1. Build 2-3 tree        *";
        cout << endl << "* 2. Build AVL tree        *";
        cout << endl << "*************************************";
        cout << endl << "Input a choice(0, 1, 2): ";
    }

    void Mission1() {
        if (!Operate()) return;

        for (int i = 0; i < data.size(); i++) {
            T.Create(i + 1, data[i]);
        }
        T.Print(T.Get23tree());
        have_built = false;
    }
    void Mission2() {
        if (data.size() == 0) {
            cout << "### Choose 1 first. ###\n";
            return;
        }
        for (int i = 0; i < data.size(); i++) {
            AVL.Create(i + 1, data[i]);
        }
        AVL.Print(have_built); 
        have_built = true;       
    }
    bool All_digit(string cmd) {
        for (int i = 0; i < cmd.length(); i++) {
            if (cmd[i] < '0' || cmd[i] > '9') {
                return false;
            }
        }
        return true;
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
        } else if (C.All_digit(cmd)) {
            cout << endl << "Command does not exist!" << endl;
        } else {
            break;
        }
    }
}
