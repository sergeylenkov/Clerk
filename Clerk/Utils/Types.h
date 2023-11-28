#pragma once

template<class T>
using shared_vector = std::vector<std::shared_ptr<T>>;