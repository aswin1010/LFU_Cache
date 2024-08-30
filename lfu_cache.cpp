#include <iostream>
#include <unordered_map>
#include <map>

using namespace std;

// LFUCache class as provided by you
class LFUCache {
public:

    class node{
        public:
        int key;
        int val;
        node* next;
        node* prev;

        node(int _key,int _val){
            key = _key;
            val = _val;
            next = prev =NULL;
        }
    };

    int cap=0;
    int flag = 1;
    int maxfreq=1;
    int minfreq = 1;

    map<int, pair<node *, node *>> freq; // frequency table
    unordered_map<int, node*> mpp; // key pointer
    map<int, int> k2f; // key frequency

    LFUCache(int capacity) {
        cap = capacity;
    }

    void delnode(node* temp){
        node* temp_prev = temp->prev;
        node* temp_next = temp->next;
        temp_prev->next = temp_next;
        temp_next->prev = temp_prev; 
    }

    void addnode(node* temp, node* head){
        node* p = head->next;
        temp->next = p;
        p->prev = temp;
        temp->prev = head;
        head->next = temp;
    }
    
    int get(int key) {
        if(mpp.find(key) != mpp.end()){
            node* temp = mpp[key];
            int res = temp->val;
            int f = k2f[key];
            delnode(temp);
            if(f == maxfreq){ 
                f++;
                maxfreq++;
                node * head = new node(-1, -1);
                node * tail = new node(-1, -1);
                head->next = tail;
                tail->prev = head;
                freq[maxfreq] = {head, tail};
                addnode(temp, head);
            }
            else{
                f++;
                node * head = freq[f].first;
                addnode(temp, head); 
            }
            k2f[key] = f;
            return res;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if(mpp.find(key) != mpp.end()){
            node* temp = mpp[key];
            temp->val = value;
            int f = k2f[key];
            delnode(temp);
            if(f == maxfreq){ 
                f++;
                maxfreq++;
                node * head = new node(-1, -1);
                node * tail = new node(-1, -1);
                head->next = tail;
                tail->prev = head;
                freq[maxfreq] = {head, tail};
                addnode(temp, head);
            }
            else{
                f++;
                node * head = freq[f].first;
                addnode(temp, head); 
            }
            k2f[key] = f;
        }
        else{
            if(mpp.size() == cap){
                int mini = 1;
                for(int i = 1; i <= maxfreq; i++){
                    if(freq.find(i) != freq.end() && (freq[i].first)->next != (freq[i].second)){
                        mini = i;
                        break;
                    }
                }

                node * head = freq[mini].first;
                node * tail = freq[mini].second;
                node * temp = tail->prev;
                mpp.erase(temp->key);
                k2f.erase(temp->key);
                delnode(temp);
            }
            
            if(flag){
                node * head = new node(-1, -1);
                node * tail = new node(-1, -1);
                head->next = tail;
                tail->prev = head;
                freq[1] = {head, tail};
                flag = 0;
            }
        
            node * head = freq[1].first;
            node* temp = new node(key, value);
            addnode(temp, head);
            mpp[key] = temp;
            k2f[key] = 1;
        }
    }
};

int main() {
    int capacity;
    cout << "Enter the capacity of the LFU cache: ";
    cin >> capacity;

    LFUCache cache(capacity);

    while (true) {
        string operation;
        cout << "Enter operation (get or put) or 'exit' to quit: ";
        cin >> operation;

        if (operation == "exit") {
            break;
        } else if (operation == "get") {
            int key;
            cout << "Enter the key to get: ";
            cin >> key;
            int result = cache.get(key);
            cout << "Get(" << key << ") -> " << result << endl;
        } else if (operation == "put") {
            int key, value;
            cout << "Enter the key to put: ";
            cin >> key;
            cout << "Enter the value to put: ";
            cin >> value;
            cache.put(key, value);
            cout << "Put(" << key << ", " << value << ") executed." << endl;
        } else {
            cout << "Invalid operation." << endl;
        }
    }

    return 0;
}
