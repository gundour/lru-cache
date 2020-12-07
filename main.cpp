#include "LRU.hpp"
#include <iostream>

int main()
{
    uint size = 5;
    LRU::Cache<int, int> lru_cache(size);

    lru_cache.Put(1, 10);
    lru_cache.Put(2, 20);
    lru_cache.Put(3, 20);
    lru_cache.Put(4, 20);
    lru_cache.Put(5, 20);
    lru_cache.Put(6, 20);
    lru_cache.Put(7, 20);

    std::cout << lru_cache.Get(1).is_empty << std::endl;
    std::cout << lru_cache.Get(2).is_empty << std::endl;
    std::cout << lru_cache.Get(3).is_empty << std::endl;
    std::cout << lru_cache.Get(4).is_empty << std::endl;
    std::cout << lru_cache.Get(5).is_empty << std::endl;
    std::cout << lru_cache.Get(6).is_empty << std::endl;
    std::cout << lru_cache.Get(7).is_empty << std::endl;

    lru_cache.Drop(6);

    std::cout << lru_cache.Size() << std::endl;

    return 0;
}
