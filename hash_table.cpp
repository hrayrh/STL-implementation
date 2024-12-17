#include <iostream>
#include <list>
#include <vector>
#include <string>

class HashTable {
private:
    std::vector<std::list<std::pair<int, std::string>>> table;
    int size;

    int hashFunction(int key) {
        return key % size;
    }

public:
    HashTable(int size) : size(size) {
        table.resize(size);
    }

    void insert(int key, const std::string& value) {
        int index = hashFunction(key);
        table[index].emplace_back(key, value);
    }

    void remove(int key) {
        int index = hashFunction(key);
        auto& cell = table[index];
        for (auto it = cell.begin(); it != cell.end(); ++it) {
            if (it->first == key) {
                cell.erase(it);
                return;
            }
        }
    }

    std::string search(int key) {
        int index = hashFunction(key);
        auto& cell = table[index];
        for (const auto& pair : cell) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        return "Not found";
    }

    void display() {
        for (int i = 0; i < size; ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& pair : table[i]) {
                std::cout << "[" << pair.first << ": " << pair.second << "] ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    HashTable ht(10);

    ht.insert(1, "one");
    ht.insert(2, "two");
    ht.insert(12, "twelve");

    ht.display();

    std::cout << "Search key 2: " << ht.search(2) << std::endl;
    std::cout << "Search key 12: " << ht.search(12) << std::endl;
    std::cout << "Search key 3: " << ht.search(3) << std::endl;

    ht.remove(2);
    ht.display();

    return 0;
}