//11227125 第19組 趙軒正
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream> // for divide string's words
#include <iomanip>
using namespace std;

struct Student {
    int hvalue = 0;
    char sid[10] = {};
    char sname[10] = {};
    unsigned char score[6] = {};
    float ave = 0;
    Student() {
        
    }
    Student (string sid, string sname, unsigned char score[], float ave) {
        for (int i = 0; i < sid.size(); i++) {
            this->sid[i] = sid[i];
        }
        for (int i = 0; i < sname.size(); i++) {
            this->sname[i] = sname[i];
        }
        for (int i = 0; i < 6; i++) {
            this->score[i] = score[i];
        }        
        this->ave = ave;
    }
};
    
class Hash {
  private:
    int Find_Smallest_Prime(int n) {
        n++;
        bool is_prime = false;
        while (!is_prime) {
            for (int i = 2; i <= n; i++) {
                if (i == n) {
                    is_prime = true;
                } else if (n % i == 0) {
                    break;
                }
            }
            if (is_prime) {
                break;
            }
            n++;
        }
        return n;
    }
    int Get_hashtable_size(int data_size) {
        int table_size = data_size * 1.02;
        table_size = Find_Smallest_Prime(table_size);
        return table_size;
    }
    int Find_key(long long value, int table_size) {
        return value % table_size;
    }
    int Step_Function(long long value, int highest_step) {
        
        int step = highest_step - (value % highest_step);
        return step;
    }
    int Linear_Probing(vector<Student> &table, int key, Student student) {
        int times = 1;
        while (table[key].sname[0] != '\0') { // this address is occupied
            key++;
            times++;
            if (key == table.size()) {
                key = 0;
            } 
        }
        table[key] = student;
        return times;
    }    
    int Double_Hash(vector<Student> &table, int step, int key, Student student) {
        int times = 1;
        while (table[key].sname[0] != '\0') { // this address is occupied
            key += step;
            times++;
            if (key >= table.size()) {
                key %= table.size();
            } 
        }
        table[key] = student;     
        return times;   
    }
    int Quadratic(vector<Student> &table, Student student) {
        int times = 1;
        int num = 1;
        bool is_full = false;
        int key = student.hvalue;
        while (table[key].sname[0] != '\0') { // this address is occupied

            int step = num * num;
            key = (student.hvalue + step) %  table.size();
            times++;
            if (step >= table.size() *  table.size()) {
                is_full = true;
                break;
            }

            num++;
        }
        if (is_full) {
            cout << "Over " << table.size() *  table.size() << " steps [" << setw(3) << student.hvalue << "] still cannot find a place." << endl;
        } else {
            table[key] = student;
        }
        
        return times;
    }
  public:
    vector<Student> Hash_Function(vector<Student> student, const int mission, float& times) {
        int table_size = Get_hashtable_size(student.size());
        vector<Student> table(table_size);
        int highest_step = 0, step = 0;
        if (mission == 2) {
            highest_step = Find_Smallest_Prime(student.size() / 5);
        }
        for (int i = 0; i < student.size(); i++) {
            long long value = student[i].sid[0];
            int v = student[i].sid[0];
            for (int j = 1; student[i].sid[j] != '\0' && j < 10; j++) { // find every single value
                v = student[i].sid[j - 1];
                value *= student[i].sid[j];
            }
            if (mission == 1) {
                int key = Find_key(value, table_size);
                student[i].hvalue = key;
                times += Linear_Probing(table, key, student[i]);
            } else if (mission == 2) {
                int key = Find_key(value, table_size);
                student[i].hvalue = key;
                int step = Step_Function(value, highest_step);
                times += Double_Hash(table, step, key, student[i]);
            } else if (mission == 3) {
                int key = Find_key(value, table_size);
                student[i].hvalue = key;
                times += Quadratic(table, student[i]);     
            }
        }
        return table;
    }
    float Find_Unsuccessful_Search(vector<Student> table) {
        float total = 0;
        for (int i = 0; i < table.size(); i++) {
            int times = 0;
            int probe = i;
            while (table[probe].sid[0] != '\0') {
                probe = (probe + 1) % table.size();
                times++;
                if (probe == i) break;  // 已繞完一圈
            }
            total += times;
        }
        
        return total;
    }
    float Find_Unsuccessful_Search2(vector<Student> table) {
        float total = 0;
        for (int i = 0; i < table.size(); i++) {
            int times = 0;
            int probe = i;
            int num = 1;
            bool is_full = false;
            while (table[probe].sid[0] != '\0') {
                probe = (i + (num * num)) % table.size();
                
                if (times == table.size() * table.size() - 2) { 
                    is_full = true;
                    break; 
                } 
                num++;
                times++;
            }
            if (is_full) {
                cout << "\nOver " << table.size() *  table.size() << " steps [" << setw(4) << i << "] still cannot find a place.";
            }
            total += times;
        }
        
        return total;
    }
};

class File_Process {
  private:
    ifstream cur_file;
    vector<Student> student;
  public:
    bool Have_file(string filename, int &file_type) { // file_type == 1 is bin, 2 is txt;
        student.clear();
        string file_bin = "input" + filename + ".bin";
        cur_file.open(file_bin);
        if (cur_file.fail()) {
            cout << endl << "### " << file_bin << " does not exist! ###" << endl;
            cur_file.close();
            string file_txt = "input" + filename + ".txt";
            cur_file.open(file_txt);           
            if (cur_file.fail()) {
                cout << endl << "### " << file_txt << " does not exist! ###" << endl;
                return false;
            }
            file_type = 2;
            return true;
        }
        cur_file.close();
        file_type = 1;
        return true;
    }

    vector<vector<string>> Divide_line() {
        string str;
        vector<vector<string>> data;
        while (getline(cur_file, str)) {
            vector<string> tem;
            string word;
            std::stringstream ss(str);
            while (getline(ss, word, '\t')) {
                tem.push_back(word);
            }
            data.push_back(tem);
        }
        cur_file.close();
        return data;
    }
    void Get_Student_From_Bin(string filename) {
        filename = "input" + filename + ".bin";
        cur_file.open(filename, ios::binary);
        Student temp;
        while (cur_file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
            student.push_back(temp);
        }
    }
    void Txt_to_bin(vector<vector<string>> data, string filename) {
        cur_file.close();
        filename = "input" + filename + ".bin";
        ofstream file(filename, std::ios::out | std::ios::binary);
        for (int i = 0; i < data.size(); i++) {
            unsigned char score[6];
            for (int j = 2; j < 8; j++) {
                score[j - 2] = static_cast<unsigned char>(stoi(data[i][j]));
            }
            Student temp(data[i][0], data[i][1], score, stof(data[i][8]));
            student.push_back(temp);
        }
        int size = student.size();
        for (const auto& stu : student) {
            file.write(reinterpret_cast<const char*>(&stu), sizeof(Student));
        }
        cout << endl << "---" << filename << " has been created ---" << endl;
        student.clear();
        file.close();
        
    }
    void Output_File(string filename, vector<Student> table, int mission) {
        if (mission == 1) filename = "linear" + filename + ".txt";
        else if (mission == 2) filename = "double" + filename + ".txt";
        else if (mission == 3) filename = "quadratic" + filename + ".txt";
        ofstream output_hash(filename);

        if (mission == 1) output_hash << " --- Hash table created by Linear probing    ---" << endl;
        else if (mission == 2) output_hash << " --- Hash table created by Double hashing    ---" << endl;
        else if (mission == 3) output_hash << " --- Hash table created by Quadratic probing ---" << endl;
        int num = 0;
        for (const auto& t : table) {
            output_hash << "[" << setw(3) << num << "] ";
            if (t.sid[0] != '\0') { // empty
                output_hash << setw(10) << t.hvalue << "," << setw(11) << t.sid 
                << "," << setw(11) << t.sname << "," << setw(11) << t.ave << endl;          
            } else {
                output_hash << endl;
            }
            num++;
        }
        output_hash << " ----------------------------------------------------- ";
    }
    vector<Student> Getstudent() {
        return student;
    }
};

class Cmd_Control {
  private:
    File_Process FP;
    Hash hash;
    bool have_built = false;
    
    string filename;
    bool Operate() {
        vector<vector<string>> data;
        string filenum;
        bool have_file = false;
        int file_type = 0;
        cout << "\nInput a file number ([0] Quit): ";
        cin >> filenum;
        if (filenum == "0") return false;
        filename = filenum;
        have_file = FP.Have_file(filename, file_type);
        if (!have_file) return false;
        if (file_type == 2) { // txt
            data = FP.Divide_line();
            FP.Txt_to_bin(data, filename);             
        }
        FP.Get_Student_From_Bin(filename); 
        float times = 0;
        vector<Student> student = FP.Getstudent();
        vector<Student> table = hash.Hash_Function(student, 1, times);
        FP.Output_File(filename, table, 1);
        float total = hash.Find_Unsuccessful_Search(table);
        float unsuccessful = total / table.size();
        float successful = times / student.size();
        cout << endl << "Hash table has been successfully created by Linear probing   " << endl;  
        cout << "\nunsuccessful search: " << fixed << setprecision(4) << unsuccessful << " comparisons on average" << endl;
        cout << "successful search: " << fixed << setprecision(4) << successful << " comparisons on average";
        return true;
    }
  public:
    void list() {
        cout << endl << endl << "******* Hash Table *****";
        cout << endl << "* 0. QUIT              *";
        cout << endl << "* 1. Linear probing    *";
        cout << endl << "* 2. Double hashing    *";
        cout << endl << "* 3. Quadratic probing *";
        cout << endl << "************************";
        cout << endl << "Input a choice(0, 1, 2, 3): ";
    }

    void Mission1() {
        have_built = Operate();
    }
    void Mission2() {
        if (!have_built) {
            cout << "### Command 1 first. ###\n";
            return;
        }
        float times = 0, size = FP.Getstudent().size();
        vector<Student> table = hash.Hash_Function(FP.Getstudent(), 2, times);
        FP.Output_File(filename, table, 2);
        float successful = times / size;
        cout << endl << "Hash table has been successfully created by Double hashing   " << endl;
        cout << "\nsuccessful search: " << fixed << setprecision(4) << successful << " comparisons on average";
    }
    void Mission3() {
        if (!have_built) {
            cout << "### Command 1 first. ###\n";
            return;
        }
        float times = 0;
        vector<Student> student = FP.Getstudent();
        vector<Student> table = hash.Hash_Function(student, 3, times);
        FP.Output_File(filename, table, 3);
        cout << endl << "Hash table has been successfully created by Quadratic probing" << endl; 
        float total = hash.Find_Unsuccessful_Search2(table);
        float unsuccessful = total / table.size();
        float successful = times / student.size();
         
        cout << "\nunsuccessful search: " << fixed << setprecision(4) << unsuccessful << " comparisons on average" << endl;
        cout << "successful search: " << fixed << setprecision(4) << successful << " comparisons on average";        
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
