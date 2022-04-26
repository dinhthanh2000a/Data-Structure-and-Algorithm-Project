#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T removeAt(int index) = 0;// Changed T& to T
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
};

#endif

// STUDENT ANSWER
template <class T>
    class FragmentLinkedList : IList<T>
    {
    public:
        class Node;
        class Iterator;

    protected:
        Node** fragmentPointers;
        int fragmentMaxSize;
        Node* head;
        Node* tail;
        Node** save;
        int count;

    public:
        FragmentLinkedList(int fragmentMaxSize = 5)
        {
            this->count = 0;
            this->fragmentMaxSize = fragmentMaxSize;
            this->fragmentPointers = new Node * [2];
            this->fragmentPointers[0] = NULL;
            this->fragmentPointers[1] = NULL;
            this->save = new Node * [2];
            this->save[0] = NULL;
            this->save[1] = NULL;
            this->head = NULL;
            this->tail = NULL;
        }
        virtual ~FragmentLinkedList();
        virtual void add(const T& element);
        virtual void add(int index, const T& element);
        virtual T removeAt(int index);
        virtual bool removeItem(const T& item);
        virtual bool empty();
        virtual int size();
        virtual void clear();
        virtual T get(int index);
        virtual void set(int index, const T& element);
        virtual int indexOf(const T& item);
        virtual bool contains(const T& item);
        virtual string toString();
        Iterator begin(int index = 0) {
            return Iterator(index, this, true);
        }
        Iterator end(int index = -1) {
            return Iterator(index, this, false);
        }
    

    public:
        class Node
        {
        private:
            T data;
            Node* next;
            Node* prev;
            friend class FragmentLinkedList<T>;

        public:
            Node()
            {
                next = 0;
                prev = 0;
            }
            Node(Node* next, Node* prev)
            {
                this->next = next;
                this->prev = prev;
            }
            Node(T data, Node* next, Node* prev)
            {
                this->data = data;
                this->next = next;
                this->prev = prev;
            }
        };

        class Iterator
        {
        private:
            FragmentLinkedList<T>* pList;
            Node* pNode;
            int index;

        public:
            Iterator(FragmentLinkedList<T>* pList = 0, bool begin = true);
            Iterator(int fragmentIndex, FragmentLinkedList<T>* pList = 0, bool begin = true);
            Iterator& operator=(const Iterator& iterator);
            T& operator*();
            bool operator!=(const Iterator& iterator);
            void remove();
            void set(const T& element);
            Iterator& operator++();
            Iterator operator++(int);
        };
    };


    template <class T>
    string FragmentLinkedList<T>::toString()
    {
        stringstream ss;
        ss << "[";
        Node* ptr = head;
        while (ptr != tail) {
            //if (ptr == head) ss << ptr->data ;
            ss << ptr->data << ", ";
            ptr = ptr->next;
        }

        if (count > 0)
            ss << ptr->data << "]";
        else
            ss << "]";
        return ss.str();
    }



    template <class T>
    void FragmentLinkedList<T>::add(const T& element) {
        Node* pNode = new Node;
        pNode->data = element;
        if (this->count == 0) { ///test node rong
            pNode->next = NULL;
            pNode->prev = NULL;
            this->head = pNode;
            this->tail = pNode;
        }
        else {
            pNode->prev = this->tail;
            this->tail->next = pNode;
            pNode->next = NULL;
            this->tail = pNode;
        }
        this->count++;
        if (count <= fragmentMaxSize) {
            fragmentPointers[0] = head;
            fragmentPointers[1] = tail;
        }
        else {
            if ((count - 1) % fragmentMaxSize != 0) fragmentPointers[((count - 1) / fragmentMaxSize) + 1] = tail;
            else {
                int sofm = count / fragmentMaxSize; //size of fragment
                if (count % fragmentMaxSize != 0) sofm++;
                this->save = new Node * [sofm - 1];
                for (int i = 0; i < sofm - 1; i++) {
                    this->save[i] = this->fragmentPointers[i];
                }
                delete[] fragmentPointers;
                this->fragmentPointers = new Node * [sofm + 1];
                for (int i = 0; i < sofm - 1; i++) {
                    this->fragmentPointers[i] = this->save[i];
                }
                delete[] save;
                fragmentPointers[sofm - 1] = tail;
                fragmentPointers[sofm] = tail;
            }
        }
    }
    template <class T>
    void FragmentLinkedList< T>::add(int index, const T& element) {
        Node* Nodeindex = new Node;
        Nodeindex->data = element;
        if (index > this->count || index < 0) throw std::out_of_range("The index is out of range!");

        int frg = index / fragmentMaxSize;
        int frgid = index % fragmentMaxSize;
        if (index % fragmentMaxSize == 0 && index != 0) {
            frg -= 1;
            frgid = fragmentMaxSize;
        }
        if (index == 0) {
            if (this->count == 0) {
                this->head = this->tail = Nodeindex;
            }
            else {
                Nodeindex->next = this->head;
                this->head->prev = Nodeindex;
                Nodeindex->prev = NULL;
                this->head = Nodeindex;
            }
        }
        else if (index == this->count) {
            Nodeindex->prev = tail;
            tail->next = Nodeindex;
            Nodeindex->next = NULL;
            tail = Nodeindex;
        }
        else {
            int k = 1;
            Node* Nodetest = new Node;
            Nodetest = fragmentPointers[frg];
            while (k < frgid) {
                Nodetest = Nodetest->next;
                k++;
            }
            Nodeindex->prev = Nodetest;
            Nodeindex->next = Nodetest->next;
            Nodetest->next->prev = Nodeindex;
            Nodetest->next = Nodeindex;
        }
        this->count++;
        int sofm = count / fragmentMaxSize; //size of fragment
        if (count % fragmentMaxSize != 0) sofm++;
        if (count <= fragmentMaxSize) {
            fragmentPointers[0] = head;
            fragmentPointers[1] = tail;
        }
        else {
            if ((count - 1) % fragmentMaxSize != 0) {
                fragmentPointers[sofm] = tail;
                if (index != count) {
                    int x = index / fragmentMaxSize;
                    if (index % fragmentMaxSize != 0) x += 1;
                    for (int i = x; i < sofm; i++) {
                        this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
                    }
                }
            }
            else {
                this->save = new Node * [sofm - 1];
                for (int i = 0; i < sofm - 1; i++) {
                    this->save[i] = this->fragmentPointers[i];
                }
                delete[] fragmentPointers;
                this->fragmentPointers = new Node * [sofm + 1];
                for (int i = 0; i < sofm - 1; i++) {
                    this->fragmentPointers[i] = this->save[i];
                }
                delete[] save;
                fragmentPointers[sofm - 1] = tail;
                fragmentPointers[sofm] = tail;
                if (index != count) {
                    int x = index / fragmentMaxSize;
                    if (index % fragmentMaxSize != 0) x += 1;
                    for (int i = x; i < sofm - 1; i++) {
                        this->fragmentPointers[i] = this->fragmentPointers[i]->prev;
                    }
                }
            }
        }
    }
    template <class T>
    T FragmentLinkedList< T>::removeAt(int index) {
        Node* pNode = new Node;
        if (head == NULL || index < 0 || index >= count)  throw std::out_of_range("The index is out of range!");
        else if (index == 0) {
            pNode = head;
            if (count != 1) {
                this->head = this->head->next;
            }
            else this->head = this->tail = NULL;
        }
        else if (index == count - 1) {
            pNode = tail;
            this->tail = this->tail->prev;
        }
        else {
            int frg = index / fragmentMaxSize;
            int frgid = index % fragmentMaxSize;
            if (index % fragmentMaxSize == 0 && index != 0) {
                frg -= 1;
                frgid = fragmentMaxSize;
            }
            Node* Nodetest = this->fragmentPointers[frg];
            int k = 0;
            while (k < frgid) {
                Nodetest = Nodetest->next;
                k++;
            }
            pNode = Nodetest;
            Nodetest->prev->next = Nodetest->next;
            Nodetest->next->prev = Nodetest->prev;

        }
        this->count--;
        int sofm = count / fragmentMaxSize; //size of fragment
        if (count % fragmentMaxSize != 0) sofm++;
        if (count <= fragmentMaxSize) {
            delete[] fragmentPointers;
            fragmentPointers = new Node * [2];
            fragmentPointers[0] = head;
            fragmentPointers[1] = tail;
        }
        else {
            if ((count) % fragmentMaxSize != 0) {
                fragmentPointers[sofm] = tail;
                if (index != count) {
                    int x = index / fragmentMaxSize;
                    if (index % fragmentMaxSize != 0) x += 1;
                    for (int i = x; i < sofm; i++) {
                        this->fragmentPointers[i] = this->fragmentPointers[i]->next;
                    }
                }

            }
            else {
                this->save = new Node * [sofm];
                for (int i = 0; i < sofm; i++) {
                    this->save[i] = this->fragmentPointers[i];
                }
                delete[] fragmentPointers;
                this->fragmentPointers = new Node * [sofm + 1];
                for (int i = 0; i < sofm; i++) {
                    this->fragmentPointers[i] = this->save[i];
                }
                delete[] save;
                fragmentPointers[sofm] = tail;
                if (index != count) {
                    int x = index / fragmentMaxSize;
                    if (index % fragmentMaxSize != 0) x += 1;
                    for (int i = x; i < sofm; i++) {
                        this->fragmentPointers[i] = this->fragmentPointers[i]->next;
                    }
                }
            }
        }
        if (count != 0) {
            this->head->prev = NULL;
            this->tail->next = NULL;
        }
        T datax = pNode->data;
        delete pNode;
        return datax;
    }
    template <class T>
    bool FragmentLinkedList< T>::removeItem(const T& item) {
        int a = indexOf(item);
        if (a != -1) {
            removeAt(a);
            return true;
        }
        else
            return false;
    }
    template <class T>
    bool FragmentLinkedList<T>::empty() {
        if (this->count == 0) return true;
        return false;
    }
    template <class T>
    int FragmentLinkedList< T>::size() {
        return count;
    }
    template <class T>
    void FragmentLinkedList< T>::clear() {
        for (int i = 0; i < count; i++) {
            Node* pNode = this->head;
            this->head = this->head->next;
            delete pNode;
        }
        this->head = NULL;
        this->tail = NULL;
        this->count = 0;
        delete[] fragmentPointers;
        fragmentPointers = new Node * [2];
        fragmentPointers[0] = fragmentPointers[1] = NULL;
    }
    template <class T>
    T FragmentLinkedList< T>::get(int index) {
        Node* pNode = new Node;
        if (index < 0 || index >= this->count) throw std::out_of_range("The index is out of range!");
        else {
            int i = 0;
            int frg = index / fragmentMaxSize;
            int frgid = index % fragmentMaxSize;
            if (index % fragmentMaxSize == 0 && index != 0) {
                frg -= 1;
                frgid = fragmentMaxSize;
            }
            else if (index % fragmentMaxSize == 0 && index == 0) {
                frg = 0;
                frgid = 0;
            }

            pNode = this->fragmentPointers[frg];
            while (i < frgid) {
                pNode = pNode->next;
                i++;
            }
        }
        return pNode->data;
    }
    template <class T>
    void FragmentLinkedList< T>::set(int index, const T& element) {
        Node* pNode = new Node;
        if (index < 0 || index >= this->count) throw std::out_of_range("The index is out of range!");
        else {
            int i = 0;
            int frg = index / fragmentMaxSize;
            int frgid = index % fragmentMaxSize;
            if (index % fragmentMaxSize == 0 && index != 0) {
                frg -= 1;
                frgid = fragmentMaxSize;
            }
            else if (index % fragmentMaxSize == 0 && index == 0) {
                frg = 0;
                frgid = 0;
            }
            pNode = this->fragmentPointers[frg];
            while (i < frgid) {
                pNode = pNode->next;
                i++;
            }
        }
        pNode->data = element;

    }
    template <class T>
    int FragmentLinkedList<T>::indexOf(const T& item) {
        int k = 0;
        Node* pNode = this->head;
        while (pNode != NULL) {
            if (pNode->data == item) return k;
            k++;
            pNode = pNode->next;
        }
        return -1;
    }
    template <class T>
    bool FragmentLinkedList<T>::contains(const T& item) {
        int a = (this->indexOf(item));
        if (a != -1) return true;
        else return false;
    }
    template <class T>
    FragmentLinkedList<T>:: ~FragmentLinkedList() {
        fragmentPointers = NULL;
        fragmentMaxSize = 0;
        this->count = 0;
        this->fragmentMaxSize = 0;
        delete[] fragmentPointers;
        this->head = NULL;
        this->tail = NULL;
        this->save = NULL;
        delete[] save;
    }

    template <class T>
    FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, bool begin) {
        this->pList = pList;
        if (begin == true) {
            if (!this->pList->empty()) {
                this->pNode = pList->head;
                this->index = 0;
            }
            else {
                this->pNode = NULL;
                this->index = -1;
            }
        }
        else {
            this->pNode = NULL;
            this->index = this->pList->count;

        }
    }
    template <class T>
    FragmentLinkedList<T>::Iterator::Iterator(int fragmentIndex, FragmentLinkedList<T>* pList, bool begin) {
        this->pList = pList;
        int sofm = this->pList->count / this->pList->fragmentMaxSize;
        if ((this->pList->count) % (this->pList->fragmentMaxSize) != 0) sofm++;
        if (fragmentIndex<-1 || fragmentIndex>sofm) throw std::out_of_range("Segmentation fault!");
        else if (this->pList->empty() == true) {
            this->pNode = NULL;

            this->index = -1;
        }
        else {
            if (begin == 1) {
                this->pNode = this->pList->fragmentPointers[fragmentIndex];

                this->index = fragmentIndex * this->pList->fragmentMaxSize;
                if (fragmentIndex == -1) {
                    this->pNode = this->pList->fragmentPointers[sofm - 1];
                    this->index = fragmentIndex * this->pList->fragmentMaxSize;
                }
            }
            else {
                if (fragmentIndex == -1) {
                    this->pNode = NULL;

                    this->index = this->pList->count;
                }
                else {
                    if (fragmentIndex == sofm || fragmentIndex == sofm - 1) {
                        this->pNode = NULL;

                        this->index = this->pList->count;
                    }
                    else {
                        this->pNode = this->pList->fragmentPointers[fragmentIndex + 1];

                        this->index = (fragmentIndex + 1) * this->pList->fragmentMaxSize;
                    }
                }
            }
        }
    }
    template <class T>
    typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator:: operator=(const Iterator& iterator) {
        this->pList = iterator.pList;
        this->pNode = iterator.pNode;

        this->index = iterator.index;
        return *this;
    }
    template <class T>
    T& FragmentLinkedList<T>::Iterator:: operator*() {



        if (this->pNode == NULL) {
            throw std::out_of_range("Segmentation fault!");
        }

        return this->pNode->data;
    }
    template <class T>
    bool FragmentLinkedList<T>::Iterator::operator!=(const Iterator& iterator) {
        if (this->pNode == NULL && iterator.pNode == NULL) return false;
        else if (this->pNode == iterator.pNode && this->index == iterator.index) {
            return false;
        }

        else return true;
    }
    template <class T>
    void FragmentLinkedList<T>::Iterator::remove() {



        if (this->pNode == NULL) throw std::out_of_range("Segmentation fault!");
        if (!index) {
            this->pNode = NULL;

            this->index = -1;

            this->pList->removeAt(0);
        }
        else {
            this->pNode = this->pNode->prev;

            this->pList->removeAt(this->index--);
        }
    }
    template <class T>
    void FragmentLinkedList<T>::Iterator::set(const T& element) {


        if (this->pNode == NULL) {
            throw std::out_of_range("Segmentation fault!");
        }

        else this->pNode->data = element;
    }
    template <class T>
    typename FragmentLinkedList<T> ::Iterator& FragmentLinkedList<T>::Iterator::operator++() {


        if ((this->pNode == NULL) && (this->index != -1)) throw std::out_of_range("Segmentation fault!");

        if (this->index == -1) {
            this->pNode = this->pList->head;
        }
        else {
            this->pNode = this->pNode->next;
        }
        this->index++;
        return *this;
    }
    template <class T>
    typename FragmentLinkedList<T> ::Iterator FragmentLinkedList<T>::Iterator ::operator++(int) {

        if ((this->pNode == NULL) && (this->index != -1)) throw std::out_of_range("Segmentation fault!");
        Iterator* Nodetest = new Iterator(this->pList, true);

        Nodetest->pNode = this->pNode;
        Nodetest->index = this->index;
        if (this->index == -1) {
            this->pNode = this->pList->head;
        }
        else {
            this->pNode = this->pNode->next;
        }
        this->index++;
        return *Nodetest;
    }
// END: STUDENT ANSWER

int main()
{
    FragmentLinkedList<int> pList=FragmentLinkedList<int>(15);
    for (int i=0;i<2;i++){
        pList.add(i);
    }
    auto it=FragmentLinkedList<int>::Iterator(&pList,true);
    auto it1=FragmentLinkedList<int>::Iterator(&pList,false);
    it.remove();
    cout<<*++it;

}