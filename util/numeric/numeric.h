#include <iterator>
#include <concepts>


template <std::input_iterator It>
int mex(It begin, It end);

template <std::input_iterator It>
int mex(It begin, It end, size_t max_value);
