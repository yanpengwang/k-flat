
import java.util.Iterator;
import java.util.Scanner;
import java.util.TreeSet;

class Range implements Comparable<Range>
{

    public void setval(Range prt, int nb, int bgn, int ed)
    {
        parent = prt;
        number = nb;
        begin = bgn;
        end = ed;
        rank = (parent == null)? 1: parent.rank + 1;
    }

    @Override
    public int compareTo(Range o){
        return this.begin - o.begin;
    }

    Range   parent;
    int number;
    int begin;
    int end;
    int rank;
};

class Ranges
{
    // set ranges will store
    public TreeSet<Range> range_set = new TreeSet<>();

    // toFind is used to find the new node within range_set
    private Range toFind = new Range();
    // Find the range which contains given number or return null.
    public Range find(int number)
    {

        toFind.begin = number; // note: set begin with number to search
        if(range_set.size() < 1) return null;
        Range result = (number < range_set.first().begin)? range_set.first(): range_set.floor(toFind);
        // because range_set's size >=1, so "I" couldn't equal to range_set.end()
        return (result.begin <= number && result.end > number)? result: null;
    }

    // Add range to range_set
    void add(Range new_range)
    {
        /* traverse all the nodes untill *I's begin >=range->end */
        Iterator<Range> iterator = range_set.iterator();
        while(iterator.hasNext())
        {
            Range x = iterator.next();
            if(x.begin >= new_range.end) break;
            // find the first overlapping range
            if (x.end <= new_range.begin)
            {
                continue;
            }

            /*
             * if new_range's rank is higher, to get the longest sequence,
             * replace current range with new_range's
             */
            if(new_range.rank > x.rank)
            {
                /*
                 *          new_range:    |------|
                 *       current range: |-----|
                 */
                if(new_range.begin > x.begin)
                    x.end = new_range.begin;
                /*
                 *          new_range:    |------|
                 *       current range:      |-----|
                 */
                else if(new_range.end < x.end)
                    x.begin = new_range.end;
                /*
                 *          new_range:    |------|
                 *      current range:      |---|
                 */
                else {
                    iterator.remove();
                    continue;
                }
            } else {
                /*
                 * the new range doesn't has higher rank, according to
                 * requirement, x's range will be used for later
                 * coming ranges
                 */
                if(new_range.begin < x.begin || new_range.end <= x.end)
                    new_range.end = x.begin;
                else
                    new_range.begin = x.end;
            }
        } //end of while
        range_set.add(new_range);
    } //end of add()
};

public class Flat{
    public static void main(String[] args){
        Scanner scan = new Scanner(System.in);

        int number;
        int n = scan.nextInt();
        int k = scan.nextInt();

        Ranges ranges = new Ranges();
        Range max = null;
        Range range, parent;
        if (n >= 1)
        {
            number = scan.nextInt();
            range = new Range();
            parent = null;
            range.setval(parent, number, number - k, number + k + 1);
            max = range;
            ranges.add(range);
        }
        for (int i=1; i<n; i++) {
            number = scan.nextInt();
            parent = ranges.find(number);
            range = new Range();
            range.setval(parent, number, number - k, number + k + 1);
            if  (max.rank < range.rank) {
                max = range;
            }
            ranges.add(range);
        }

        int[] result = new int[max.rank];
        int rank = max.rank;
        int i = rank - 1;
        while (max != null) {
            result[i] = max.number;
            max = max.parent;
            i--;
        }

        i = 0;
        while (i < rank) {
            System.out.println(result[i]);
            i++;
        }
    }
}

