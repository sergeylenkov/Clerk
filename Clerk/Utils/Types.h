#pragma once

#include <vector>
#include <map>
#include <memory>

template<class T>
using shared_vector = std::vector<std::shared_ptr<T>>;

using exhange_rate_map = std::map<std::pair<int, int>, float>;