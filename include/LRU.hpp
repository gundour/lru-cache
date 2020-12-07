#ifndef LRU_SINGLEHEADER_INCLUDED
#define LRU_SINGLEHEADER_INCLUDED

#include <unordered_map>

namespace LRU
{
    template <typename T1, typename T2>
    struct Node
    {
        bool is_empty;
        T1 key;
        T2 value;
        Node *prev;
        Node *next;

        Node() : is_empty(true) {}
        Node(T1 key, T2 value) : is_empty(false),
                                 key(key),
                                 value(value)
        {
        }
    };

    template <typename T1, typename T2>
    class Cache
    {
    public:
        Cache(const uint &size)
        {
            this->size = size;
            head = new Node<T1, T2>{};
            tail = new Node<T1, T2>{};

            head->prev = nullptr;
            tail->next = nullptr;
            head->next = tail;
            tail->prev = head;

            cache_store.reserve(size);
        }

        ~Cache()
        {
            for (auto &&record : cache_store)
            {
                delete record.second;
            }

            cache_store.clear();

            delete head;
            delete tail;
        }

        Node<T1, T2> Get(const T1 &key)
        {
            auto cache_store_itr = cache_store.find(key);
            if (cache_store_itr != cache_store.end())
            {
                // element found, move it to front and return it
                Node<T1, T2> *node = cache_store_itr->second;
                p_UnRef(node);
                p_ChRef(node);

                return *node;
            }
            else
            {
                // element not found
                Node<T1, T2> empty_node{};
                return empty_node;
            }
        }

        void Put(const T1 &key, const T2 &value)
        {
            if (size == 0)
            {
                return;
            }

            auto cache_store_itr = cache_store.find(key);
            if (cache_store_itr != cache_store.end())
            {
                // element found, update value and move to front
                Node<T1, T2> *node = cache_store_itr->second;
                p_UnRef(node);
                node->value = value;
                p_ChRef(node);
            }
            else
            {
                // element not found, add new element
                if (cache_store.size() == size)
                {
                    // cache full, remove last element
                    p_Drop(tail->prev);
                }

                Node<T1, T2> *node = new Node<T1, T2>(key, value);
                p_Add(node);
            }
        }

        void Drop(const T1 &key)
        {
            auto cache_store_itr = cache_store.find(key);
            if (cache_store_itr != cache_store.end())
            {
                Node<T1, T2> *node = cache_store_itr->second;
                p_Drop(node);
            }
        }

        size_t Size()
        {
            return cache_store.size();
        }

    private:
        uint size;
        std::unordered_map<T1, Node<T1, T2> *> cache_store;
        Node<T1, T2> *head;
        Node<T1, T2> *tail;

        void p_ChRef(Node<T1, T2> *node)
        {
            Node<T1, T2> *next = head->next;
            head->next = node;
            node->prev = head;
            node->next = next;
            next->prev = node;
        }

        void p_Add(Node<T1, T2> *node)
        {
            p_ChRef(node);
            cache_store.insert({node->key, node});
        }

        void p_UnRef(Node<T1, T2> *node)
        {
            Node<T1, T2> *prev = node->prev;
            Node<T1, T2> *next = node->next;
            prev->next = next;
            next->prev = prev;
        }

        void p_Drop(Node<T1, T2> *node)
        {

            p_UnRef(node);
            cache_store.erase(node->key);
            delete node;
        }
    };
} // namespace LRU

#endif // LRU_SINGLEHEADER_INCLUDED
