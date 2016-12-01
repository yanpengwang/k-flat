// another implementation by others
#include <cstdio>
#include <assert.h>
#include <set>
#include <sys/time.h>  
using namespace std;
struct Range {
	Range *parent;
	int number;
	int begin;
	int end; // one past end
	int rank;
	void set(Range *parent, int number, int begin, int end) {
		this->parent = parent;
		this->number = number;
		this->begin = begin;
		this->end = end;
		
		if (parent == NULL) {
			rank = 1;
		} else {
			rank = parent->rank + 1;
		}
	}
};
struct comparator {
	bool operator()(const Range* i1, Range* i2) const {
		return i1->begin < i2->begin;
	}
};
/*
 * Define len(i) as the length of the longest k-flat subsequence ending at any number in (i-k, i+k)
 *
 * There is Theorem I:
 * For any i there is a vicinity (i-x, i+y), x+y>=2*k such that for any j in (i-x, i+y) there is len(j) >= len(i)
 *
 * We describe len(i) as a set or ranges.
 */
struct Ranges {
	set<Range*, comparator> ranges;
	typedef set<Range*, comparator>::iterator iterator;
	static Range toFind;
	/*
	 * Find the range which contains given number or return null.
	 */
	Range* find(int number) {
		toFind.begin = number;
		iterator I = ranges.upper_bound(&toFind);
	
				
		if (I != ranges.begin()) {
			I--;
		}
		if (I == ranges.end()) {
//			printf("%d", number);
			return NULL;
		}
		Range *result = *I;
		if (result->begin <= number && number < result->end) {
			return result;
		}
		return NULL;
	}
	/*
	 * Add new range to the set
	 */
	void add(Range *range) {
		iterator I = ranges.upper_bound(range);
		if (I != ranges.begin()) {
			I--;
		}
		while (I != ranges.end() && (*I)->begin < range->end) {
		
			if ((*I)->end <= range->begin) {
		
						++I;
				continue;
			}
			if ((*I)->rank < range->rank) {
				// (*I) is partially on the left, partially on the right, or completely inside
				if ((*I)->begin < range->begin) {
					(*I)->end = range->begin;
				} else if (range->end < (*I)->end) {
					(*I)->begin = range->end;
				} else {
					ranges.erase(I++);
					continue;
				}
			} else {
				if (range->begin < (*I)->begin || range->end <= (*I)->end) {
					range->end = (*I)->begin;
				} else {
					range->begin = (*I)->end;
				}
			}
			++I;
		}
		// sequence cannot be shortened to 0-length
		//assert(range->begin < range->end);
		ranges.insert(range);
	}
	void print() {
		iterator I  = ranges.begin();
		while (I != ranges.end()) {
			fprintf(stderr, "(%d, %d)^%d_%d ", (*I)->begin, (*I)->end - 1, (*I)->number, (*I)->rank);
			I++;
		}
		fprintf(stderr, "\n");
	}
};
Range Ranges::toFind;
int main(int argc, char *argv[]) {
//hrtime_t  time_start,time_now,time_used;
//time_start =  gethrtime();
	int n, k;
	scanf("%d %d", &n, &k);
	Ranges ranges;
	Range *max = NULL;
	Range slab[100000]; // slab to speedup memory allocations, gives 0.3s on max test
	for (int i=0; i<n; i++) {
		int number;
		scanf("%d", &number);
//
//fprintf(stderr, "Number: %d\n", number);
		Range *parent = ranges.find(number);
		Range *range = &slab[i];
		range->set(parent, number, number - k, number + k + 1);
		if (max == NULL || (max->rank < range->rank)) {
			max = range;
		}
		ranges.add(range);
//
//ranges.print();
	}
	int result[max->rank];
	int rank = max->rank;
	int i = rank - 1;
	while (max != NULL) {
		result[i] = max->number;
		max = max->parent;
		i--;
	}
	i++;
	// assert(i==0);
		typedef set<Range*, comparator>::iterator iterator;
	iterator I = ranges.ranges.begin();
	while (I != ranges.ranges.end())
	{
		//printf("number=%d ,rank=%d ,begin=%d ,end=%d \n", (*I)->number, (*I)->rank, (*I)->begin, (*I)->end);
		I++;
	}
	while (i < rank) {
		printf("%d ", result[i]);
		i++;
	}
		//	time_now =  gethrtime();
		//time_used = time_now-time_start; 
	
		//printf("\nTime used is:%lld ms\n",time_used/1000000);
    printf("\n");
	return 0;
}
