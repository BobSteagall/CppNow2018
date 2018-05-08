//==================================================================================================
//  File:   scd_tests.cpp
//
//  Copyright (c) 2018 Bob Steagall, KEWB Computing
//==================================================================================================
//
#include "container_tests.h"

//--------------------------------------------------------------------------------------------------
//  Class:
//      scd_raw_heap
//
//  Summary:
//      This class template implements a simple monotonic heap that draws bytes from a fixed-
//      size buffer of bytes.  This class provides the addressing model, storage model, and
//      allocation strategy in one place.
//--------------------------------------------------------------------------------------------------
//
template<size_t N>
struct scd_raw_heap
{
    size_t      m_hwm;          //- High water mark
    uint8_t     m_buf[N];       //- The fixed-size segment

    using void_pointer = syn_ptr<void, offset_addressing_model>;

    scd_raw_heap();

    void_pointer    allocate(size_t n);
    void            deallocate(void_pointer p);
    static  size_t  round_up(size_t x, size_t r);
};

template<size_t N> inline
scd_raw_heap<N>::scd_raw_heap()
:   m_hwm(0)
{
    memset(m_buf, 0, sizeof(m_buf));
}

template<size_t N> inline typename scd_raw_heap<N>::void_pointer
scd_raw_heap<N>::allocate(size_t n)
{
    void_pointer    p(m_buf + m_hwm);
    m_hwm += round_up(n, 8);

    return p;
}

template<size_t N> inline void
scd_raw_heap<N>::deallocate(void_pointer p)
{}

template<size_t N> inline size_t
scd_raw_heap<N>::round_up(size_t x, size_t r)
{
    return (x % r) ? (x + r - (x % r)) : x;
}

template struct scd_raw_heap<4096u>;

//--------------------------------------------------------------------------------------------------
//  Class:
//      scd_allocator
//
//  Summary:
//      This class template implements a standard-conforming allocator that allocates from an
//      associated scd_raw_heap.  In keeping with the idea that an allocator is a handle to a
//      memory resource, it stores a pointer to the heap object.
//--------------------------------------------------------------------------------------------------
//
template<class T, size_t N>
class scd_allocator
{
  public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    using heap_type             = scd_raw_heap<N>;
    using difference_type       = ptrdiff_t;
    using size_type             = size_t;
    using void_pointer          = syn_ptr<void, offset_addressing_model>;
    using const_void_pointer    = syn_ptr<void const, offset_addressing_model>;
    using pointer               = syn_ptr<T, offset_addressing_model>;
    using const_pointer         = syn_ptr<T const, offset_addressing_model>;
    using reference             = T&;
    using const_reference       = T const&;
    using value_type            = T;
    using element_type          = T;

    template<class U>
    struct rebind
    {
        using other = scd_allocator<U, N>;
    };

  public:
    ~scd_allocator() = default;

    scd_allocator() = default;
    scd_allocator(const scd_allocator& src) noexcept = default;
    scd_allocator(heap_type* pheap);
    template<class U>
    scd_allocator(const scd_allocator<U, N>& src) noexcept;

    scd_allocator&  operator =(const scd_allocator& vRhs) noexcept = default;

    T*          address(reference t) const noexcept;
    T const*    address(const_reference t) const noexcept;
    size_type   max_size() const noexcept;

    pointer     allocate(size_type n);
    pointer     allocate(size_type n, const_void_pointer p);
    void        deallocate(pointer p, size_type n);

    template<class U, class... Args>
    void        construct(U* p, Args&&... args);

    template<class U>
    void        destroy(U* p);

  private:
    template<class OT, size_t ON> friend class scd_allocator;
    using heap_pointer = syn_ptr<heap_type, offset_addressing_model>;
    
    heap_pointer    mp_heap;
};

template<class T, size_t N> inline
scd_allocator<T, N>::scd_allocator(heap_type* pheap)
:   mp_heap(pheap)
{}

template<class T, size_t N>
template<class U> inline
scd_allocator<T, N>::scd_allocator(scd_allocator<U,N> const& src) noexcept
:   mp_heap(src.mp_heap)
{}

template<class T, size_t N> inline T*
scd_allocator<T, N>::address(reference t) const noexcept
{
    return &t;
}

template<class T, size_t N> inline T const*
scd_allocator<T, N>::address(const_reference t) const noexcept
{
    return &t;
}

template<class T, size_t N> inline typename scd_allocator<T, N>::size_type
scd_allocator<T, N>::max_size() const noexcept
{
    return N / sizeof(T);
}

template<class T, size_t N> inline
typename scd_allocator<T, N>::pointer
scd_allocator<T, N>::allocate(size_type n)
{
    return static_cast<pointer>(mp_heap->allocate(n * sizeof(T)));
}

template<class T, size_t N> inline
typename scd_allocator<T, N>::pointer
scd_allocator<T, N>::allocate(size_type n, const_void_pointer)
{
    return static_cast<pointer>(mp_heap->allocate(n * sizeof(T)));
}

template<class T, size_t N> inline
void
scd_allocator<T, N>::deallocate(pointer p, size_type)
{
    mp_heap->deallocate(p);
}

template<class T, size_t N>
template<class U, class... Args> inline void
scd_allocator<T, N>::construct(U* p, Args&&... args)
{
    ::new ((void*) p) U(std::forward<Args>(args)...);
}

template<class T, size_t N>
template<class U> inline void
scd_allocator<T, N>::destroy(U* p)
{
    p->~U();
}


template class scd_allocator<std::string, 8192>;

//--------------------------------------------------------------------------------------------------
//  Class:
//      scd_message
//
//  Summary:
//      This class template implements a self-contained heap and message.
//--------------------------------------------------------------------------------------------------
//
template<size_t N>
class scd_message
{
    using heap_type        = scd_raw_heap<N>;

    using syn_string_alloc = scd_allocator<char, N>;
    using syn_string       = simple_string<syn_string_alloc>;

    using syn_list_alloc   = scd_allocator<syn_string, N>;
    using syn_list         = std::list<syn_string, syn_list_alloc>;

    using syn_less         = std::less<syn_string>;
    using syn_pair         = std::pair<syn_string const, syn_list>;
    using syn_map_alloc    = scd_allocator<syn_pair, N>;
    using syn_map          = std::map<syn_string, syn_list, syn_less, syn_map_alloc>;

  public:
    scd_message();
    scd_message(scd_message const&);

    scd_message&    operator =(scd_message const&);

    void            add_data(int key_start, int val_start, int count);
    void            print_values() const;

  private:
    heap_type   m_heap;
    syn_map     m_map;
};

template<size_t N>
scd_message<N>::scd_message()
:   m_heap()
,   m_map(syn_map_alloc(&m_heap))
{}

template<size_t N>
scd_message<N>::scd_message(scd_message const& src)
:   m_heap()
,   m_map(syn_map_alloc(&m_heap))
{
     memcpy(this, &src, sizeof(scd_message));
}

template<size_t N> scd_message<N>&
scd_message<N>::operator =(scd_message const& rhs)
{
    if (&rhs != this)
    {
        memcpy(this, &rhs, sizeof(scd_message));
    }
    return *this;
}

template<size_t N> void
scd_message<N>::add_data(int key_start, int val_start, int count)
{
    char        key_buf[128], val_buf[128];

    syn_string_alloc    str_alloc(&m_heap);
    syn_list_alloc      list_alloc(&m_heap);

    syn_string  key_str(str_alloc);
    syn_string  val_str(str_alloc);
    syn_list    val_list(list_alloc);

    for (int i = val_start;  i < (val_start + count);  ++i)
    {
        sprintf(val_buf, "this is value string #%d", i+100);
        val_str.assign(val_buf);
        val_list.push_back(std::move(val_str));
    }
    
    sprintf(key_buf, "this is key string #%d", key_start);
    key_str.assign(key_buf);

    m_map.emplace(std::move(key_str), std::move(val_list));
}

template<size_t N> void
scd_message<N>::print_values() const
{
    std::cout << "*****  TEST MSSG  ******" << std::endl;
    std::cout << "map address is: 0x" << std::hex << (uintptr_t) &m_map << std::endl;

    for (auto const& elem : m_map)
    {
        std::cout << elem.first << std::endl;
        for (auto const& val : elem.second)
        {
            std::cout << "    " <<  val << "   0x" << std::hex << (uintptr_t) &val << std::endl;
        }
    }
    std::cout << std::endl;
}

void test_scd()
{
    scd_message<8192>   msg;

    for (int i = 0;  i < 3;  ++i)
    {
        msg.add_data((i+1)*10, (i+1)*200, 4);
    }
    msg.print_values();

    char    bytes[sizeof(scd_message<8192>)];
    memcpy(&bytes[0], &msg, sizeof(scd_message<8192>));

    auto const*     pmsg = reinterpret_cast<scd_message<8192>*>(&bytes[0]);
    pmsg->print_values();

    std::vector<char>   vmsg(sizeof(scd_message<8192>));
    memcpy(vmsg.data(), &msg, sizeof(scd_message<8192>));

    auto const*     pmsg2 = reinterpret_cast<scd_message<8192>*>(vmsg.data());
    pmsg2->print_values();
}
