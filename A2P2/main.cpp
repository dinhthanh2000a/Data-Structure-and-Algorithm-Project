#include<bits/stdc++.h> 
using namespace std; 
class Interval {
public:
    int start;
    int end;
    
    Interval(int start = 0, int end = 0) {
        this->start = start;
        this->end = end;
    }
};
int minPark(vector<Interval> lInterval) {
    vector<int> start;
    vector<int> end;
    int size = lInterval.size();
    for (int unsigned i = 0; i < lInterval.size(); i++) {
        start.push_back(lInterval[i].start);
        end.push_back( lInterval[i].end);
    }
    sort(start.begin(),start.end());
    sort(end.begin(),end.end());
    int temp = 0, count = 0, i = 0, j = 0;
    
    while (i < size && j < size) {
        if (start[i] < end[j]) {
            count++;
            temp = max(temp, count);
            i++;
        }
        else {
            count--;
            j++;
        }
    }

    return temp;
}
int main(){
    vector<Interval> intervals;
    intervals.push_back(Interval(1, 5));
    intervals.push_back(Interval(2, 5));
    intervals.push_back(Interval(3, 5));

    cout << minPark(intervals);
}