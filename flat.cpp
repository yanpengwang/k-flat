/*************************************************************************
	> File Name: flat.cpp
	> Author: wang yanpeng
	> Mail: wangyanpeng@gmail.com 
	> Created Time: 2016年12月27日 星期二 11时46分58秒
 ************************************************************************/

#include<iostream>
#include<set>
using namespace std;

class Range 
{
public:
    Range()
    {
        parent = NULL; number = begin = end = rank = 0;
    }

    void setval(Range * prt, int nb, int bgn, int ed)
    {
        parent = prt;
        number = nb;
        begin = bgn;
        end = ed;
        if (parent == NULL) 
            rank = 1;
        else
            rank = parent->rank + 1;
    }

    Range * parent; // point to my precedent node within k-flat sequence
    int    number; // the input number of current node
    int    begin;  // starting point of my range, may change
    int    end;    // starting point of my range, may change
    int    rank;   // my rank in current k-flat sequence
};

// Define a comparator, to be used by set<Range*, RangeComp> in class Ranges.
class RangeComp
{
public:
    bool operator()(const Range * r1, const Range * r2) const
    {
        // compare the begin value
        return r1->begin < r2->begin;    
    }

};

class Ranges
{
public:
    // set ranges will store 
    set<Range *, RangeComp> range_set;
    typedef set<Range *, RangeComp>::iterator iterator;
    // toFind is used to find the new node within range_set
    static Range toFind;
    // Find the range which contains given number or return null.
    Range * find(int number)
    {
        toFind.begin = number; // note: set begin with number to search
        iterator I = range_set.upper_bound(&toFind);

        if (I != range_set.begin()) I--;

        // because range_set's size >=1, so "I" couldn't equal to range_set.end()
        Range * result = *I;
        if(result->begin <= number && result->end > number)
            return result;
        return NULL;
    }

    // Add range to range_set
    void add(Range *new_range)
    {
        iterator I = range_set.upper_bound(new_range);
        if (I != range_set.begin()) I--;

        /* traverse all the nodes untill *I's begin >=range->end */
        while(I != range_set.end() && (*I)->begin < new_range->end)
        {
            // find the first overlapping range
            if ((*I)->end <= new_range->begin) 
            {
                I++;
                continue;
            }

            /*
             * if new_range's rank is higher, to get the longest sequence, 
             * replace (*I)'s range with new_range's
             */
            if(new_range->rank > (*I)->rank)
            {
                if(new_range->begin > (*I)->begin)
                    /*
                     *          new_range:    |------|
                     *       (*I)'s range: |-----| 
                     */
                    (*I)->end = new_range->begin;
                else if(new_range->end < (*I)->end)
                    /*
                     *          new_range:    |------|
                     *       (*I)'s range:      |-----| 
                     */
                    (*I)->begin = new_range->end;
                else
                    /*
                     *          new_range:    |------|
                     *       (*I)'s range:      |---| 
                     */
                {
                    range_set.erase(I++);
                    continue;
                }
            } else {
            /*
             * the new range doesn't has higher rank, according to 
             * requirement, (*I)'s range will be used for later 
             * coming ranges
             */
                if(new_range->begin < (*I)->begin || new_range->end <= (*I)->end)
                    new_range->end = (*I)->begin;
                else
                    new_range->begin = (*I)->end;
            }
            I++;
        } //end of while
        range_set.insert(new_range);
    } //end of add()
};

Range Ranges::toFind;

int main() 
{
	int n, k, number;
    cin >> n >> k;
	Ranges ranges;
	Range *max = NULL;
	Range slab[100000]; // slab to speedup memory allocations
    Range *range, *parent;
    if (n >= 1) 
    {
        cin >> number;
		range = &slab[0];
		range->setval(NULL, number, number - k, number + k + 1);
		if (max == NULL || (max->rank < range->rank)) {
			max = range;
		}
		ranges.add(range);
    }
	for (int i=1; i<n; i++) {
        cin >> number;
		parent = ranges.find(number);
		range = &slab[i];
		range->setval(parent, number, number - k, number + k + 1);
		if (max == NULL || (max->rank < range->rank)) {
			max = range;
		}
		ranges.add(range);
	}
	int result[max->rank];
	int rank = max->rank;
	int i = rank - 1;
	while (max != NULL) {
		result[i] = max->number;
		max = max->parent;
		i--;
	}
    i = 0;
	while (i < rank) {
		cout << result[i] <<endl;
		i++;
	}
    return 0;
}
