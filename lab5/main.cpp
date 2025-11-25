#include <iostream>
#include <string>

#include "include/Memory_resourse.h"
#include "include/DoublyLinkedList.h"


using namespace std;

struct MyStruct {
    int id;
    double ratio;
    string name;
};

int main() {
    MemoryResource mr(1024 * 10);

    cout << "--- Тест1 ---\n";
    {
        DoublyLinkedList<int> listInt(&mr);
        
        listInt.push_back(100);
        listInt.push_back(200);
        listInt.push_back(300);

        for (auto val : listInt) {
            cout << val << " ";
        }
        cout << "\n";

        listInt.pop_back(); 
        listInt.push_back(400); 

        cout << "After modify: ";
        for (auto val : listInt) {
            cout << val << " ";
        }
        cout << "\n";
    }

    cout << "\n--- Тест2 ---\n";
    {
        DoublyLinkedList<MyStruct> listStruct(&mr);

        listStruct.push_back({1, 1.5, "Object_A"});
        listStruct.push_back({2, 2.8, "Object_B"});

        for (auto& item : listStruct) {
            cout << "Name: " << item.name 
                      << ", ID: " << item.id 
                      << ", Val: " << item.ratio << "\n";
        }
    }

    return 0;
}