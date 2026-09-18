//11227125 趙軒正
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream> // for divide string's words
#include <cmath>
using namespace std;

struct Data { // heap每個點裡的資料
    int serial_num, num; // serial_num是序號 num在maxheap時是畢業生數 在min-maxheap是學生數
};

// 處理maxheap
class Maxheap {
  private:
    vector<struct Data> heap;
  public:
    // 讓root從index 1開始 (方便計算，否則從0開始會需要加一或減一才能除二)
    void Createvoidroot() {
        struct Data tem;
        tem.serial_num = 0;
        tem.num = 0;
        heap.push_back(tem);        
    }
    // 建立maxheap
    void BuildMaxHeap(int serial_num, int num) {
        struct Data tem;
        tem.serial_num = serial_num;
        tem.num = num;
        heap.push_back(tem);
        while (serial_num > 1) { // 大於一是因為index 0沒有要參與交換
            if (heap[serial_num].num > heap[serial_num / 2].num) { //如果現在的數 > 父節點就交換
                swap(heap[serial_num], heap[serial_num / 2]);
            }
            serial_num /= 2;
        }
    }
    //印出maxheap的資訊
    void Print(int mission) {
        if (mission == 1) {
            cout << "<max heap>\n";
        } else if (mission == 2) {
            cout << "<min-max heap>\n";
        } else {
            cout << "<DEAP>\n";
        }
        
        int bottom = heap.size() - 1;
        int leftmost = 1;
        while (leftmost <= bottom) {
            leftmost *= 2;
        }
        leftmost /= 2;
        if (mission != 3) {
            cout << "root: [" << heap[1].serial_num << "] " << heap[1].num << endl;
        }
        cout << "bottom: [" << heap[bottom].serial_num << "] " << heap[bottom].num << endl;
        cout << "leftmost bottom: [" << heap[leftmost].serial_num << "] " << heap[leftmost].num << endl;
        heap.clear(); //清空heap清空heap
    }
};
// 處理minmaxheap
class MinMaxHeap {
  private:
    vector<struct Data> heap;
  public:
  void Createvoidroot() {
    struct Data tem;
    tem.serial_num = 0;
    tem.num = 0;
    heap.push_back(tem);        
}
    // odds number = min level, even = max.
    void BuildMinMaxHeap(int serial_num, int num) {
        struct Data tem;
        tem.serial_num = serial_num;
        tem.num = num;
        heap.push_back(tem);
        int n = 1, which_level = 0;
        while (n <= serial_num) {
            n *= 2;
            which_level++;
        }
        if (which_level % 2 == 0) { // max level
            // 父節點比現在的數"大" 且 index不能為0
            if (heap[serial_num / 2].num > heap[serial_num].num && serial_num / 2 != 0) { 
                swap(heap[serial_num / 2], heap[serial_num]);
                CheckMinLevel(serial_num / 2); //交換後確認父節點與其他minlevel的大小關係
            // 父節點比現在的數"小"
            } else { 
                CheckMaxLevel(serial_num); //確認現在的數與其他maxlevel的大小關係
            }
        } else { // min level
            // 父節點比現在的數"小" 且 index不能為0
            if (heap[serial_num / 2].num < heap[serial_num].num && serial_num / 2 != 0) {
                swap(heap[serial_num / 2], heap[serial_num]);
                CheckMaxLevel(serial_num / 2); //交換後確認父節點與其他maxlevel的大小關係
            // 父節點比現在的數"大"
            } else {
                CheckMinLevel(serial_num); //確認現在的數與其他minlevel的大小關係
            }
        }
    }
    //確認maxlevel的大小關係
    void CheckMaxLevel(int serial_num) {
        if (serial_num >= 4) { //必須超過4才能除以四(不能計算到index 0)
            while (serial_num > 1) {
                if (serial_num / 4 == 0) {
                    break;
                }
                if (heap[serial_num].num > heap[serial_num / 4].num) { //現在的數與上面的maxlevel比較
                    swap(heap[serial_num], heap[serial_num / 4]);
                }
                serial_num /= 4;
            }
        }
    }
    //確認minlevel的大小關係
    void CheckMinLevel(int serial_num) {
        if (serial_num >= 4) {
            while (serial_num > 1) {
                if (serial_num / 4 == 0) {
                    break;
                }
                if (heap[serial_num].num < heap[serial_num / 4].num) {
                    swap(heap[serial_num], heap[serial_num / 4]);
                }
                serial_num /= 4;
            }
        }
    }
    //印出maxheap的資訊
    void Print(int mission) {
        if (mission == 1) {
            cout << "<max heap>\n";
        } else if (mission == 2) {
            cout << "<min-max heap>\n";
        } else {
            cout << "<DEAP>\n";
        }
        
        int bottom = heap.size() - 1;
        int leftmost = 1;
        while (leftmost <= bottom) {
            leftmost *= 2;
        }
        leftmost /= 2;
        if (mission != 3) {
            cout << "root: [" << heap[1].serial_num << "] " << heap[1].num << endl;
        }
        cout << "bottom: [" << heap[bottom].serial_num << "] " << heap[bottom].num << endl;
        cout << "leftmost bottom: [" << heap[leftmost].serial_num << "] " << heap[leftmost].num << endl;
        heap.clear(); //清空heap清空heap
    }
};

class Deap {
  private:
    vector<struct Data> heap;
  public:
  void Createvoidroot() {
    struct Data tem;
    tem.serial_num = 0;
    tem.num = 0;
    heap.push_back(tem);        
}
    void Build_Deap(int serial_num, int num) {
        struct Data tem;
        tem.serial_num = serial_num;
        tem.num = num;
        heap.push_back(tem);
        if (serial_num == 1) {
            return;
        }
        bool which_heap = false; // false == min, true == max
        int n = serial_num;
        while (n > 2) {
            n = (n - 1) / 2;
        }
        if (n == 2) {
            which_heap = true;
        }
        if (!which_heap) { // min heap
            int times = log(serial_num + 1) / log(2) - 1;
            int correspond = 2;
            for (int i = 0; i < times - 1; i++) {
                correspond *= 2;
            }
            correspond += serial_num;
            if (correspond > heap.size() - 1) {
                correspond = (correspond - 1) / 2;
            }
            /*cout << "\nin min correspond = " << correspond << endl;*/
            if (heap[serial_num].num > heap[correspond].num) {
                swap(heap[serial_num], heap[correspond]);
                CheckMax(correspond);
            } else {
                CheckMin(serial_num);
            }
        } else { // max heap
            int level = log(serial_num + 1) / log(2);
            int correspond = 2;
            for (int i = 0; i < level - 1; i++) {
                correspond *= 2;
            }
            correspond = serial_num - (correspond / 2);
            /*cout << "\nin max correspond = " << correspond << endl;*/
            if (heap[serial_num].num < heap[correspond].num) {
                swap(heap[serial_num], heap[correspond]);
                CheckMin(correspond);
            } else {
                CheckMax(serial_num);
            }            
        }
        /*for (int i = 0; i < heap.size(); i++) {
            cout << "serial = " << heap[i].serial_num << " num = " << heap[i].num;
            cout << " ";
        }
        cout << endl;*/
    }
    void CheckMax(int serial_num) {
        if (serial_num >= 5) { //必須超過4才能除以四(不能計算到index 0)
            /*cout << "check max!\n";*/
            while (serial_num > 1) {
                if ((serial_num - 1) / 2 == 0) {
                    break;
                }
                if (heap[serial_num].num > heap[(serial_num - 1) / 2].num) { //現在的數與上面的maxlevel比較
                    swap(heap[serial_num], heap[(serial_num - 1) / 2]);
                }
                serial_num = (serial_num - 1) / 2;
            }
        }
    }
    //確認minlevel的大小關係
    void CheckMin(int serial_num) {
        if (serial_num >= 3) {
            /*cout << "check min!\n";*/
            while (serial_num > 1) {
                if ((serial_num - 1) / 2 == 0) {
                    break;
                }
                if (heap[serial_num].num < heap[(serial_num - 1) / 2].num) {
                    swap(heap[serial_num], heap[(serial_num - 1) / 2]);
                }
                serial_num = (serial_num - 1) / 2;
            }
        }
    }
    //印出maxheap的資訊
    void Print(int mission) {
        if (mission == 1) {
            cout << "<max heap>\n";
        } else if (mission == 2) {
            cout << "<min-max heap>\n";
        } else {
            cout << "<DEAP>\n";
        }
        int bottom = heap.size() - 1;
        int leftmost = 1;
        while (leftmost <= bottom) {
            leftmost = leftmost * 2 + 1;
        }
        leftmost = (leftmost - 1) / 2;
        if (mission != 3) {
            cout << "root: [" << heap[1].serial_num << "] " << heap[1].num << endl;
        }
        cout << "bottom: [" << heap[bottom].serial_num << "] " << heap[bottom].num << endl;
        cout << "leftmost bottom: [" << heap[leftmost].serial_num << "] " << heap[leftmost].num << endl;
        heap.clear(); //清空heap清空heap
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
    Maxheap Max;
    MinMaxHeap MinMax;
    Deap deap;
    bool had_executed = false;
  public:
    void list() {
        cout << endl << "**** Heap Construction *****";
        cout << endl << "* 0. QUIT                  *";
        cout << endl << "* 1. Build a max heap      *";
        cout << endl << "* 2. Build a min-max heap  *";
        cout << endl << "* 3. Build a DEAP          *";
        cout << endl << "****************************";
        cout << endl << "Input a choice(0, 1, 2, 3): ";
    }
    bool All_digit(string cmd) {
        for (int i = 0; i < cmd.length(); i++) {
            if (cmd[i] < '0' || cmd[i] > '9') {
                return false;
            }
        }
        return true;
    }
    //畢業生數排序MaxHeap
    void Mission1() {
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
            return;
        }
        FP.File_Input();
        vector<vector<string>> data = FP.Divide_line();
        if (data.size() == 0) {
            return;
        }
        Max.Createvoidroot();
        for (int i = 0; i < data.size(); i++) {
            Max.BuildMaxHeap(i + 1, Number_Transfrom(data[i][8]));
        }
        Max.Print(1);

    }
    void Mission2() {
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
            return;
        }
        FP.File_Input();
        vector<vector<string>> data = FP.Divide_line();
        if (data.size() == 0) {
            return;
        }
        MinMax.Createvoidroot();
        for (int i = 0; i < data.size(); i++) {
            MinMax.BuildMinMaxHeap(i + 1, Number_Transfrom(data[i][6]));
        }
        MinMax.Print(2);        
    }
    void Mission3() {
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
            return;
        }
        FP.File_Input();
        vector<vector<string>> data = FP.Divide_line();
        if (data.size() == 0) {
            return;
        }
        deap.Createvoidroot();
        for (int i = 0; i < data.size(); i++) {
            deap.Build_Deap(i + 1, Number_Transfrom(data[i][8]));
        }
        deap.Print(3);          
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
        } else if (C.All_digit(cmd)) {
            cout << endl << "Command does not exist!" << endl;
        } else {
            break;
        }
    }
}
