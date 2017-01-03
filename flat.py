#!/usr/bin/env python
# coding=utf-8
#
# written by wangyanpeng@gmail.com, 2017.1.3
#

import bisect

def find_le(a, x):
    'Find rightmost value less than or equal to x, return index to caller'
    i = bisect.bisect_right(a, x)
    if i:
        return i-1
    else:
        return 0

class Node(object):
    """
    The class to store each input number's information 
    """
    def __init__(self, number, parentseq):
        self.number = number
        self.begin = number - k
        self.end = number + k
        self.parentseq = parentseq
        if parentseq < 0:
            self.rank = 1
        else:
            self.rank = allnodes[parentseq].rank + 1
    
class Ranges(object):
    """
    The class used to maintain the dictionary of active ranges 
    begin_dic is a dictionary with pairs of "begin:sequence"
    """
    def __init__(self):
        self.begin_dic = {}
        self.sorted_dic_key = []

    # find the sequence number of parent node
    def find(self, number):
        idx = find_le(self.sorted_dic_key, number)
        result = self.sorted_dic_key[idx]
        seq = self.begin_dic[result]
        if number >= result and number <= allnodes[seq].end:
            return seq
        else:
            return -1

    # add begin value into begin_dic
    def add(self, sequence):
        # i_xxx is for be inserted node, c_xxx is for current existing node
        i_begin = allnodes[sequence].begin
        i_end = allnodes[sequence].end
        i_rank = allnodes[sequence].rank

        loc = find_le(self.sorted_dic_key, i_begin)
        
        while (loc < len(self.sorted_dic_key) and \
            self.sorted_dic_key[loc] <= i_end):

            c_begin = self.sorted_dic_key[loc]
            c_end = allnodes[self.begin_dic[c_begin]].end
            c_rank = allnodes[self.begin_dic[c_begin]].rank

            if i_begin > c_end :
                loc += 1
                continue

            if i_rank > c_rank:
                if i_begin > c_begin:
                    allnodes[self.begin_dic[c_begin]].end = i_begin - 1
                elif i_end < c_end:
                    allnodes[self.begin_dic[c_begin]].begin = i_end + 1
                    # update the begin_dic with new begin
                    self.begin_dic[i_end + 1] = self.begin_dic.pop(c_begin)
                else:
                    self.begin_dic.pop(c_begin)
            else:
                if i_begin < c_begin or i_end < c_end:
                    allnodes[sequence].end = c_begin - 1
                else:
                    allnodes[sequence].begin = c_end + 1
            # refresh insert node's begin and end as they would change
            i_begin = allnodes[sequence].begin
            i_end = allnodes[sequence].end
            loc += 1
        # add new begin into begin_dic
        self.begin_dic[allnodes[sequence].begin] = sequence
        # update the sorted key dictionary
        self.sorted_dic_key = sorted(self.begin_dic.keys())

###############################################################################

# global dictionary to store all nodes's information
allnodes = {}
ranges = Ranges()
input_num = []

while True:
    try:
        str_in = raw_input()
        if str_in == '':
            break
        input_num = input_num + [int(i) for i in str_in.split()]
    except (EOFError):
            break
n = input_num[0]
k = input_num[1]
input_data = input_num[2:]
max_seq = 0

# process the first node
if n > 1:
    allnodes[0] = Node(input_data[0], -1)
    ranges.begin_dic[input_data[0] - k] = 0
    ranges.sorted_dic_key = sorted(ranges.begin_dic.keys())

# process remain nodes
for i in range(1, n):
    parent = ranges.find(input_data[i])
    allnodes[i] = Node(input_data[i], parent)
    if allnodes[i].rank > allnodes[max_seq].rank:
        max_seq = i
    ranges.add(i)

cur_seq = max_seq
result_list = []
while cur_seq >= 0:
    result_list.insert(0, allnodes[cur_seq].number)
    cur_seq = allnodes[cur_seq].parentseq

print result_list



