#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

///
#include <unordered_map>
#include <sstream>
using namespace std;
class FragmentLinkedList
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
    int sizef;
public:
    FragmentLinkedList(int fragmentMaxSize = 0)
    {
        this->sizef = 1;
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node * [1];
        this->fragmentPointers[0] = NULL;
        this->save = new Node * [1];
        this->save[0] = NULL;
        this->head = NULL;
        this->tail = NULL;
    }
    ~FragmentLinkedList();
    int add(const string& CODE, const string& data, const string& CASE, int& TIMEA, int& TIMEB);
    int removeAt(const string& CODE, const string& CASE, int& TIMEA, int& TIMEB, int& space);;
    int CS(const string& CODE, const string& CASE, int& TIME1);
    int CE(const string& CODE, const string& CASE, int& TIME1);
    string GS(const string& CODE, const string& CASE, int& TIME1);
    string GE(const string& CODE, const string& CASE, int& TIME1);

public:
    class Node
    {
    private:
        string CODE;
        string data;
        string CASE;
        int TIMEA;
        int TIMEB;
        Node* next;
        Node* prev;
        friend class FragmentLinkedList;

    public:
        Node()
        {
            CODE = "";
            data = "";
            CASE = "0";
            TIMEA = 0;
            TIMEB = 0;
            next = NULL;
            prev = NULL;
        }
        Node(Node* next, Node* prev)
        {
            CODE = "";
            data = "";
            CASE = "0";
            TIMEA = 0;
            TIMEB = 0;
            this->next = next;
            this->prev = prev;
        }
        Node(string CODE, string data, string CASE, int TIMEA, int TIMEB, Node* next, Node* prev)
        {
            this->data = data;
            this->CODE = CODE;
            this->CASE = CASE;
            this->TIMEA = TIMEA;
            this->TIMEB = TIMEB;
            this->next = next;
            this->prev = prev;
        }
    };
};
class BusSystem :public FragmentLinkedList
{

public:
    string query(string instruction) {
        // TODO: Your implementation
        int space = 0, out = 0;
        for (int unsigned ii = 0; ii < instruction.size(); ii++) {
            if (instruction[ii] == ' ') space++;
            if (ii < instruction.size() - 1) {
                if (instruction[ii] == ' ' && instruction[ii + 1] == ' ') out++;
            }
        }
        if (instruction[0] == ' ' || instruction[instruction.size() - 1] == ' ' || space < 1 || out>0) return "-1";
        string str, str1;
        int idx;

        idx = instruction.find(' ');
        str = instruction.substr(0, idx);
        str1 = instruction.substr(idx + 1);
        if (str == "SQ") {
            int N = stoi(instruction.substr(idx + 1));
            if (N < 0) return "-1";
            fragmentMaxSize = N;
            return "1";
        }
        else if (str == "INS") {

            if (fragmentMaxSize < 1 || space > 5 || space < 4 || str1[0] == ' ' || out>0) return "-1";
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            string CODE = str;
            str1 = str1.substr(idx + 1);
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            string data = str;
            str1 = str1.substr(idx + 1);
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            string CASE = "0";
            if (space == 5) {
                CASE = str;
                str1 = str1.substr(idx + 1);
                idx = str1.find(' ');
                str = str1.substr(0, idx);

            }
            int testTime = 0;
            for (int unsigned i = 0; i < str1.size(); i++) {
                if ((static_cast<int16_t>(str1[i]) > 47 && static_cast<int16_t>(str1[i]) < 58) || static_cast<int16_t>(str1[i]) == 32) testTime = testTime;
                else testTime += 1;
            }
            if (testTime > 0) return "-1";
            int TIMEA = stoi(str);
            str1 = str1.substr(idx + 1);
            int TIMEB = stoi(str1);
            if (CODE.size() > 5 || data.size() > 10 || TIMEA >= TIMEB || TIMEA < 0 || TIMEB < 0 || (CASE != "" && CASE != "1" && CASE != "0")) return "-1";
            string x = to_string(add(CODE, data, CASE, TIMEA, TIMEB));
            return x;
        }
        else if (str == "DEL") {
            if (space > 3) return"-1";
            string CODE = "", CASE = "";
            int TIMEA = 0, TIMEB = 0;
            if (space == 3) {
                int idx;
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                CODE = str;
                str1 = str1.substr(idx + 1);
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) > 47 && static_cast<int16_t>(str1[i]) < 58) || static_cast<int16_t>(str1[i]) == 32) testTime = testTime;
                    else testTime += 1;
                }
                if (testTime > 0) return "-1";
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                TIMEA = stoi(str);
                str1 = str1.substr(idx + 1);
                TIMEB = stoi(str1);
            }
            else if (space == 2) {
                int idx;
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                CODE = str;
                str1 = str1.substr(idx + 1);
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) < 48 && static_cast<int16_t>(str1[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIMEA = stoi(str1);
            }
            else {
                CODE = str1;
            }
            if (TIMEA < 0 || TIMEB < 0) return "-1";
            string x = to_string(removeAt(CODE, CASE, TIMEA, TIMEB, space));
            return x;
        }
        else if (str == "CS") {
            if (space > 3 || space < 2) return"-1";
            string CODE = "", CASE = "";
            int TIME1 = 0;
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            CODE = str;
            str1 = str1.substr(idx + 1);

            if (space == 3) {
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                int testTime = 0;
                for (int unsigned i = 0; i < str.size(); i++) {
                    if ((static_cast<int16_t>(str[i]) < 48 && static_cast<int16_t>(str[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str);
                str1 = str1.substr(idx + 1);
                CASE = str1;
            }
            else if (space == 2) {
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) < 48 && static_cast<int16_t>(str1[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str1);
            }
            if (TIME1 < 0 || (CASE != "" && CASE != "1" && CASE != "0")) return "-1";
            string x = to_string(CS(CODE, CASE, TIME1));
            return x;
        }
        else if (str == "CE") {
            if (space > 3 || space < 2) return"-1";
            string CODE = "", CASE = "";
            int TIME1 = 0;
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            CODE = str;
            str1 = str1.substr(idx + 1);

            if (space == 3) {
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                int testTime = 0;
                for (int unsigned i = 0; i < str.size(); i++) {
                    if ((static_cast<int16_t>(str[i]) < 48 && static_cast<int16_t>(str[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str);
                str1 = str1.substr(idx + 1);
                CASE = str1;
            }
            else if (space == 2) {
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) < 48 && static_cast<int16_t>(str1[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str1);
            }
            if (TIME1 < 0 || (CASE != "" && CASE != "1" && CASE != "0")) return "-1";
            string x = to_string(CE(CODE, CASE, TIME1));
            return x;
        }
        else if (str == "GS") {
            if (space > 3 || space < 2) return"-1";
            string CODE = "", CASE = "";
            int TIME1 = 0;
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            CODE = str;
            str1 = str1.substr(idx + 1);

            if (space == 3) {
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                int testTime = 0;
                for (int unsigned i = 0; i < str.size(); i++) {
                    if ((static_cast<int16_t>(str[i]) < 48 && static_cast<int16_t>(str[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str);
                str1 = str1.substr(idx + 1);
                CASE = str1;
            }
            else if (space == 2) {
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) < 48 && static_cast<int16_t>(str1[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str1);
            }
            if (TIME1 < 0 || (CASE != "" && CASE != "1" && CASE != "0")) return "-1";
            return GS(CODE, CASE, TIME1);
        }
        else if (str == "GE") {
            if (space > 3 || space < 2) return"-1";
            string CODE = "", CASE = "";
            int TIME1 = 0;
            idx = str1.find(' ');
            str = str1.substr(0, idx);
            CODE = str;
            str1 = str1.substr(idx + 1);

            if (space == 3) {
                idx = str1.find(' ');
                str = str1.substr(0, idx);
                int testTime = 0;
                for (int unsigned i = 0; i < str.size(); i++) {
                    if ((static_cast<int16_t>(str[i]) < 48 && static_cast<int16_t>(str[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str);
                str1 = str1.substr(idx + 1);
                CASE = str1;
            }
            else if (space == 2) {
                int testTime = 0;
                for (int unsigned i = 0; i < str1.size(); i++) {
                    if ((static_cast<int16_t>(str1[i]) < 48 && static_cast<int16_t>(str1[i]) > 57)) testTime = testTime + 1;
                }
                if (testTime > 0) return "-1";
                TIME1 = stoi(str1);
            }
            if (TIME1 < 0 || (CASE != "" && CASE != "1" && CASE != "0")) return "-1";
            return GE(CODE, CASE, TIME1);
        }
        return "-1";
    }
};
int FragmentLinkedList::add(const string& CODE, const string& data, const string& CASE, int& TIMEA, int& TIMEB) {
    int bus = 0, ad = 0, testcode = 0, check = 0;
    Node* pNode = new Node;
    pNode->CODE = CODE;
    pNode->data = data;
    pNode->CASE = CASE;
    pNode->TIMEA = TIMEA;
    pNode->TIMEB = TIMEB;
    if (count > 0) {
        int i = 0;
        Node* Nodetest = fragmentPointers[0];
        for (i = 0; i < sizef; i++) {
            if (pNode->CODE == fragmentPointers[i]->CODE) {
                Nodetest = fragmentPointers[i];
                bus++;
                break;
            }
        }
        if (bus > 0) {
            if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
            else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;
        }

        if (ad == fragmentMaxSize) testcode += 1; //du phan tu trong fragment
        for (int j = 0; j < ad; j++) {
            if (pNode->TIMEA == Nodetest->TIMEA && pNode->CASE == Nodetest->CASE) testcode += 1;

            if (pNode->data == Nodetest->data) {
                if (pNode->TIMEA <= Nodetest->TIMEB ) testcode += 1;
            }
            Nodetest = Nodetest->next;
        }
    }
    if (testcode == 0) {
        if (this->count == 0) { ///test node rong
            pNode->next = NULL;
            pNode->prev = NULL;
            this->head = pNode;
            this->tail = pNode;
            fragmentPointers[0] = head;
        }
        else if (count > 0) {
            for (int i = 0; i < sizef; i++) {
                if (pNode->CODE == fragmentPointers[i]->CODE && sizef == i + 1) {
                    pNode->prev = tail;
                    tail->next = pNode;
                    tail = pNode;
                    check += 1;
                    break;
                }
                else if (pNode->CODE == fragmentPointers[i]->CODE && sizef > i + 1) {
                    pNode->next = fragmentPointers[i + 1];
                    pNode->prev = fragmentPointers[i + 1]->prev;
                    fragmentPointers[i + 1]->prev->next = pNode;
                    fragmentPointers[i + 1]->prev = pNode;
                    check += 1;
                    break;
                }

            }
            if (check == 0) {
                pNode->prev = tail;
                tail->next = pNode;
                tail = pNode;
                this->save = new Node * [sizef];
                for (int k = 0; k < sizef; k++) {
                    this->save[k] = this->fragmentPointers[k];
                }
                delete[] fragmentPointers;
                sizef = sizef + 1;
                this->fragmentPointers = new Node * [sizef];
                for (int k = 0; k < sizef - 1; k++) {
                    this->fragmentPointers[k] = this->save[k];
                }
                delete[] save;
                fragmentPointers[sizef - 1] = pNode;

            }

        }
        this->count++;
        return ad + 1;
    }
    else return -1;

}
int FragmentLinkedList::removeAt(const string& CODE, const string& CASE, int& TIMEA, int& TIMEB, int& space) {
    int dem = 0, ad = 0, ad1 = 0, i = 0, bus = 0;
    if (count == 0) return 0;
    else {
        Node* pNode = new Node;
        pNode->CODE = CODE;
        pNode->CASE = CASE;
        pNode->TIMEA = TIMEA;
        pNode->TIMEB = TIMEB;
        if (count == 1) {
            if ((space == 3 && pNode->CODE == head->CODE && pNode->TIMEA <= head->TIMEA && head->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == head->CODE && pNode->TIMEA == head->TIMEA) || (space == 1 && pNode->CODE == head->CODE)) {
                head = tail = NULL;
                fragmentPointers[0] = NULL;
                count--;
                dem++;
            }
        }
        else if (count > 1) {
            Node* Nodetest = fragmentPointers[0];
            for (i = 0; i < sizef; i++) {
                if (pNode->CODE == fragmentPointers[i]->CODE) {
                    Nodetest = fragmentPointers[i];
                    bus++;
                    break;
                }
            }
            if (bus > 0) {
                if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
                else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;
                ad1 = ad;

                for (int j = 0; j < ad; j++) {
                    if (count == 1) {
                        if ((space == 3 && pNode->CODE == head->CODE && pNode->TIMEA <= head->TIMEA && head->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == head->CODE && pNode->TIMEA == head->TIMEA) || (space == 1 && pNode->CODE == head->CODE)) {
                            head = tail = NULL;
                            fragmentPointers[0] = NULL;
                            count--;
                            ad1--;
                            dem++;
                            break;
                        }
                    }
                    else if (count > 1) {
                        if (Nodetest == fragmentPointers[i] && ad1 == 1 && Nodetest == head) { // xoa o dau tuyen la head va size =1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                head->next->prev = NULL;
                                //Nodetest->next->prev = NULL;
                                head = head->next;
                                //head->prev = NULL;
                                count--;
                                ad1--;
                                dem++;
                                this->save = new Node * [sizef];
                                for (int k = 0; k < sizef - 1; k++) {
                                    if (k < i) this->save[k] = this->fragmentPointers[k];
                                    else if (k >= i && k < sizef - 1) this->save[k] = this->fragmentPointers[k + 1];
                                    save[sizef - 1] = 0;
                                }
                                delete[] fragmentPointers;
                                sizef = sizef - 1;
                                this->fragmentPointers = new Node * [sizef];
                                for (int k = 0; k < sizef; k++) {
                                    this->fragmentPointers[k] = this->save[k];
                                }
                                delete[] save;

                            }

                        }
                        else if (Nodetest == fragmentPointers[i] && ad1 > 1 && Nodetest == head) { //xoa o dau tuyen la head va size >1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                head->next->prev = NULL;
                                head = head->next;
                                count--;
                                ad1--;
                                dem++;
                                fragmentPointers[i] = fragmentPointers[i]->next;

                            }

                        }
                        else if (Nodetest == fragmentPointers[i] && ad1 == 1 && Nodetest != head && Nodetest != tail) { //xoa o dau tuyen khi size =1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                Nodetest->next->prev = Nodetest->prev;
                                Nodetest->prev->next = Nodetest->next;
                                this->count--;
                                ad1--;
                                dem++;
                                this->save = new Node * [sizef];
                                for (int k = 0; k < sizef - 1; k++) {
                                    if (k < i) this->save[k] = this->fragmentPointers[k];
                                    else if (k >= i && k < sizef - 1) this->save[k] = this->fragmentPointers[k + 1];
                                    save[sizef - 1] = 0;
                                }
                                delete[] fragmentPointers;
                                sizef = sizef - 1;
                                this->fragmentPointers = new Node * [sizef];
                                for (int k = 0; k < sizef; k++) {
                                    this->fragmentPointers[k] = this->save[k];
                                }
                                delete[] save;

                            }

                        }
                        else if (Nodetest == fragmentPointers[i] && ad1 > 1 && Nodetest != head && Nodetest != tail) { //xoa o dau tuyen khi size >1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                Nodetest->prev->next = Nodetest->next;
                                Nodetest->next->prev = Nodetest->prev;
                                this->count--;
                                ad1--;
                                dem++;
                                fragmentPointers[i] = fragmentPointers[i]->next;

                            }

                        }
                        else if (Nodetest == fragmentPointers[i] && Nodetest == tail && ad1 == 1) { //xoa o dau tuyen la tail khi size=1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                tail->prev->next = NULL;
                                tail = tail->prev;
                                this->count--;
                                ad1--;
                                dem++;
                                this->save = new Node * [sizef];
                                for (int k = 0; k < sizef - 1; k++) {
                                    this->save[k] = this->fragmentPointers[k];
                                }
                                delete[] fragmentPointers;
                                sizef = sizef - 1;
                                this->fragmentPointers = new Node * [sizef];
                                for (int k = 0; k < sizef; k++) {
                                    this->fragmentPointers[k] = this->save[k];
                                }
                                delete[] save;

                            }

                        }
                        else if (Nodetest == fragmentPointers[i] && Nodetest == tail && ad1 > 1) { //xoa o dau tuyen la tail khi size >1
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                tail->prev->next = NULL;
                                tail = tail->prev;
                                this->count--;
                                ad1--;
                                dem++;

                            }
                        }
                        else if (Nodetest == tail) { //xoa o cuoi tuyen la tail
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                tail->prev->next = NULL;
                                tail = tail->prev;
                                this->count--;
                                ad1--;
                                dem++;

                            }
                        }
                        else if (Nodetest != fragmentPointers[i] && Nodetest != head && Nodetest != tail && ad1 > 2) { //xoa o giua tuyen
                            if ((space == 3 && pNode->CODE == Nodetest->CODE && pNode->TIMEA <= Nodetest->TIMEA && Nodetest->TIMEA <= pNode->TIMEB) || (space == 2 && pNode->CODE == Nodetest->CODE && pNode->TIMEA == Nodetest->TIMEA) || (space == 1 && pNode->CODE == Nodetest->CODE)) {
                                Nodetest->prev->next = Nodetest->next;
                                Nodetest->next->prev = Nodetest->prev;
                                this->count--;
                                ad1--;
                                dem++;

                            }
                        }
                    }
                    Nodetest = Nodetest->next;
                }


            }
        }
        delete pNode;
        return dem;
    }

}
int FragmentLinkedList::CS(const string& CODE, const string& CASE, int& TIME1) {
    Node* pNode = new Node;
    pNode->CODE = CODE;
    pNode->CASE = CASE;
    int ad = 0, dem = 0, i = 0, bus = 0;
    if (count > 0) {
        Node* Nodetest = fragmentPointers[0];
        for (i = 0; i < sizef; i++) {
            if (pNode->CODE == fragmentPointers[i]->CODE) {
                Nodetest = fragmentPointers[i];
                bus++;
                break;
            }
        }
        if (bus > 0) {
            if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
            else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;


            for (int j = 0; j < ad; j++) {
                if (pNode->CASE == "1" || pNode->CASE == "0") {
                    if (Nodetest->TIMEA <= TIME1 && Nodetest->TIMEB >= TIME1 && pNode->CASE == Nodetest->CASE) dem += 1;
                }
                else {
                    if (Nodetest->TIMEA <= TIME1 && Nodetest->TIMEB >= TIME1) dem += 1;
                }
                Nodetest = Nodetest->next;
            }
        }

    }
    delete pNode;
    return dem;
}
int FragmentLinkedList::CE(const string& CODE, const string& CASE, int& TIME1) {
    Node* pNode = new Node;
    pNode->CODE = CODE;
    pNode->CASE = CASE;
    int  ad = 0, dem = 0, i = 0, bus = 0;
    if (count > 0) {
        Node* Nodetest = fragmentPointers[0];
        for (i = 0; i < sizef; i++) {
            if (pNode->CODE == fragmentPointers[i]->CODE) {
                Nodetest = fragmentPointers[i];
                bus++;
                break;
            }
        }
        if (bus > 0) {
            if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
            else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;


            for (int j = 0; j < ad; j++) {
                if (pNode->CASE == "1" || pNode->CASE == "0") {
                    if (Nodetest->TIMEB < TIME1 && pNode->CASE == Nodetest->CASE) dem += 1;
                }
                else {
                    if (Nodetest->TIMEB < TIME1) dem += 1;
                }
                Nodetest = Nodetest->next;
            }
        }

    }
    delete pNode;
    return dem;
}
string FragmentLinkedList::GS(const string& CODE, const string& CASE, int& TIME1) {
    Node* pNode = new Node;
    pNode->CODE = CODE;
    pNode->CASE = CASE;
    string LP = "-1";
    int  ad = 0, i = 0, sub = 999999999, bus = 0;

    if (count > 0) {
        Node* Nodetest = fragmentPointers[0];
        for (i = 0; i < sizef; i++) {
            if (pNode->CODE == fragmentPointers[i]->CODE) {
                Nodetest = fragmentPointers[i];
                if (TIME1 >= fragmentPointers[i]->TIMEA) {
                    if (pNode->CASE == "1" || pNode->CASE == "0") {
                        if (pNode->CASE == Nodetest->CASE) {
                            LP = fragmentPointers[i]->data;
                            sub = (TIME1 - fragmentPointers[i]->TIMEA) > 0 ? (TIME1 - fragmentPointers[i]->TIMEA) : (fragmentPointers[i]->TIMEA - TIME1);
                        }
                    }
                    else {
                        LP = fragmentPointers[i]->data;
                        sub = (TIME1 - fragmentPointers[i]->TIMEA) > 0 ? (TIME1 - fragmentPointers[i]->TIMEA) : (fragmentPointers[i]->TIMEA - TIME1);
                    }
                }
                bus++;
                break;
            }
        }
        

        if (bus > 0) {
            if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
            else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;


            for (int j = 0; j < ad; j++) {
                if (TIME1 >= Nodetest->TIMEA) {
                    if (pNode->CASE == "1" || pNode->CASE == "0") {
                        if (pNode->CASE == Nodetest->CASE) {
                            int sub1 = (TIME1 - fragmentPointers[i]->TIMEA) > 0 ? (TIME1 - fragmentPointers[i]->TIMEA) : (fragmentPointers[i]->TIMEA - TIME1);
                            if (sub1 < sub) {
                                sub = sub1;
                                LP = Nodetest->data;
                            }
                        }
                    }
                    else {
                        int sub1 = (TIME1 - Nodetest->TIMEA) > 0 ? (TIME1 - Nodetest->TIMEA) : (Nodetest->TIMEA - TIME1);
                        if (sub1 < sub) {

                            sub = sub1;
                            LP = Nodetest->data;
                        }
                        if (sub1 == sub && Nodetest->CASE == "0") {

                            sub = sub1;
                            LP = Nodetest->data;
                        }
                    }

                }
                Nodetest = Nodetest->next;
            }
        }

    }
    delete pNode;
    return LP;
}

string FragmentLinkedList::GE(const string& CODE, const string& CASE, int& TIME1) {
    Node* pNode = new Node;
    pNode->CODE = CODE;
    pNode->CASE = CASE;
    string LP = "-1";
    int  ad = 0, i = 0, sub = 99999999, bus = 0;
    if (count > 0) {
        Node* Nodetest = fragmentPointers[0];
        Node* Nodesave = fragmentPointers[0];
        for (i = 0; i < sizef; i++) {
            if (pNode->CODE == fragmentPointers[i]->CODE) {
                Nodetest = fragmentPointers[i];
                Nodesave = fragmentPointers[i];
                if (TIME1 > fragmentPointers[i]->TIMEB) {
                    if (pNode->CASE == "1" || pNode->CASE == "0") {
                        if (pNode->CASE == Nodetest->CASE) {
                            LP = fragmentPointers[i]->data;
                            sub = (TIME1 - fragmentPointers[i]->TIMEB) > 0 ? (TIME1 - fragmentPointers[i]->TIMEB) : (fragmentPointers[i]->TIMEB - TIME1);
                        }
                    }
                    else {
                        LP = fragmentPointers[i]->data;
                        sub = (TIME1 - fragmentPointers[i]->TIMEB) > 0 ? (TIME1 - fragmentPointers[i]->TIMEB) : (fragmentPointers[i]->TIMEB - TIME1);
                    }
                }
                bus++;
                break;
            }
        }
        
        if (bus > 0) {
            if (i < sizef - 1) for (Node* a = fragmentPointers[i]; a != fragmentPointers[i + 1]; a = a->next) ad++;
            else for (Node* a = fragmentPointers[i]; a != NULL; a = a->next) ad++;
            for (int j = 0; j < ad; j++) {
                if (TIME1 > Nodetest->TIMEB) {
                    if (pNode->CASE == "1" || pNode->CASE == "0") {
                        if (pNode->CASE == Nodetest->CASE) {
                            int sub1 = (TIME1 - fragmentPointers[i]->TIMEB) > 0 ? (TIME1 - fragmentPointers[i]->TIMEB) : (fragmentPointers[i]->TIMEB - TIME1);
                            if (sub1 < sub) {
                                sub = sub1;
                                LP = Nodetest->data;
                                Nodesave = Nodetest;
                            }
                            if (sub1 == sub && Nodetest->TIMEA > Nodesave->TIMEA) {
                                sub = sub1;
                                LP = Nodetest->data;
                                Nodesave = Nodetest;
                            }
                        }
                    }
                    else {
                        int sub1 = (TIME1 - Nodetest->TIMEA) > 0 ? (TIME1 - Nodetest->TIMEA) : (Nodetest->TIMEA - TIME1);
                        if (sub1 < sub) {

                            sub = sub1;
                            LP = Nodetest->data;
                            Nodesave = Nodetest;
                        }
                        if (sub1 == sub && Nodetest->TIMEA > Nodesave->TIMEA) {

                            sub = sub1;
                            LP = Nodetest->data;
                            Nodesave = Nodetest;
                        }
                        if (sub1 == sub && Nodetest->TIMEA == Nodesave->TIMEA && Nodetest->CASE == "0") {
                            sub = sub1;
                            LP = Nodetest->data;
                            Nodesave = Nodetest;
                        }
                    }
                }
                Nodetest = Nodetest->next;
            }
        }


    }
    delete pNode;
    return LP;
}

FragmentLinkedList:: ~FragmentLinkedList() {
    this->sizef = 0;
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
int main(){
    BusSystem *bs=new BusSystem();
    cout<<bs->query("SQ 2")<<endl;
    cout<<bs->query("INS 50 ABC 123 456")<<endl;
    cout<<bs->query("CE 50 457");
}