#ifndef LEVENSHTEIN_DISTANCE_H
#define LEVENSHTEIN_DISTANCE_H

#include <vector>
#include <algorithm>

//how many edits to turn a into b
//http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B
template <typename T>
unsigned int levenshtein_distance(const T & a, const T & b)
{
    const size_t len1 { a.size() };
    const size_t len2 { b.size() };
    std::vector<int> col(len2+1);
    std::vector<int> prev_col(len2+1);
 
    for(size_t i=0; i<prev_col.size(); i++) {
        prev_col[i] = i;
    }

    for(size_t i=0; i<len1; i++) {
        col[0] = i+1;

        for (size_t j=0; j<len2; j++) {
            col[j+1] = std::min(
                std::min(prev_col[1 + j] + 1, col[j] + 1),
                prev_col[j] + (a[i] == b[j] ? 0 : 1)
            );
        }
        
        col.swap(prev_col);
    }

    return prev_col[len2];
}

#endif