#include <iostream>
#include <string>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <algorithm>

using namespace tbb;
using namespace std;


static const size_t N = 23;

class SubStringFilnder {
  const string str;
  size_t *max_array;
  size_t *pos_array;
public :
  void operator() (const blocked_range<size_t> &r) const {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      size_t max_size = 0, max_pos = 0;
      for (size_t j = 0; j < str.size() ; ++j)
        if (j != i) {
          size_t limit = str.size() - max(i, j);
          for (size_t k=0; i < limit; ++k) {
            if (str[i+k] != str[j+k]) break;
            if (k > max_size) {
              max_size = k;
              max_pos = j;
            }
          }
        }
      max_array[i] = max_size;
      pos_array[i] = max_pos;
    }
  }

  SubStringFilnder(string &s, size_t *m, size_t *p) :
    str(s), max_array(m), pos_array(p) {}
};

int main(int argc, char **argv)
{
  string str[N] = { string("a"), string("b") };

  /* create a fibonacci series of string starting from a and b */
  for (size_t i = 2; i < N ; ++i) 
    str[i] = str[i-1] + str[i-2];

  string to_scan = str[N-1];
  size_t num_elem = to_scan.size();

  size_t *max = new size_t[num_elem];
  size_t *pos = new size_t[num_elem];

  parallel_for(blocked_range<size_t>(0, num_elem),
	       SubStringFilnder(to_scan, max, pos));

  for (size_t i= 0; i < num_elem; ++i)
    cout << " " << max[i] << "(" << pos[i] << ")" << endl;

  delete[] pos;
  delete[] max;

  return 0;
}
