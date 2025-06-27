#include <iostream>
#include <memory>
#include <vector>
#include <concepts>
#pragma once

template<typename T>
concept Dereferenceable = requires(T t) {
    { *t } -> std::convertible_to<std::iter_reference_t<T>>;
};

template<typename T, typename Allocator = std::allocator<T>>
class Vector {
    Allocator alloc_;
    T* data_ = nullptr;

    size_t real_size_ = 0;
    size_t capacity_ = 0;
public:
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;

    template <bool IsMutable = true>
    class Iterator {
        friend Iterator operator+ (difference_type index, const Iterator& that);
        T* massive_ = nullptr;
    public:
        size_type index_ = 0;   
        using value_type = std::conditional_t<IsMutable, T, const T>;
        using reference = std::conditional_t<IsMutable, T&, const T&>;
        using pointer = std::conditional_t<IsMutable, T*, const T*>;
        using difference_type = ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        Iterator(T* mass = nullptr, size_type index = 0) noexcept : massive_(mass), index_(index) {}

        Iterator& operator=(Iterator& other) noexcept {
            if (this != &other) {
                massive_ = other.massive_;
                index_ = other.index_;
            }
            return *this;
        }

        Iterator& operator++() noexcept {
            if (massive_) {
                index_++;
            }
            return *this;
        }

        Iterator operator++(int) noexcept {
            Iterator copy = *this;
            ++(*this);
            return copy;
        }

        Iterator& operator--() noexcept {
            if (massive_) {
                index_--;
            }
            return *this;
        }

        Iterator operator--(int) noexcept {
            Iterator copy = *this;
            --(*this);
            return copy;
        }

        bool operator==(const Iterator& other) const noexcept {
            return (massive_ == other.massive_) && (index_ == other.index_);
        }

        bool operator!=(const Iterator& other) const noexcept {
            return !(*this == other);
        }

        Iterator& operator=(const Iterator& other) noexcept {
            if (this != &other) {
                index_ = other.index_;
                massive_ = other.massive_;
            }
            return *this;
        }

        reference operator*() const noexcept {
            return massive_[index_];
        }

        pointer operator->() const noexcept {
            return &massive_[index_];
        }

        Iterator& operator+= (difference_type index) {
            index_ += index;
            return *this;
        } 

        Iterator& operator-= (difference_type index) {
            index_ -= index;
            return *this;
        } 

        Iterator operator+ (difference_type index) const {
            Iterator copy(*this);
            copy.index_ += index;
            return copy;
        } 

        friend Iterator operator+ (difference_type index, const Iterator& iter) {
            return iter + index;
        } 

        Iterator operator- (difference_type index) const {
            Iterator copy(*this);
            copy.index_ -= index;
            return copy;
        } 

        difference_type operator- (const Iterator& iter) {
            return (index_ -= iter.index_);
        }

        reference operator[](difference_type i) const {
            return massive_[index_ + i];
        }

        bool operator<(const Iterator& iter) const {
            return index_ < iter.index_;
        }

        bool operator>=(const Iterator& iter) {
            return !(*this < iter);
        }

        bool operator>(const Iterator& iter) {
            return iter < (*this);
        }

        bool operator<=(const Iterator& iter) const {
            return index_ <= iter.index_;
        }
    };

    using iterator = Iterator<true>;
    using const_iterator = Iterator<false>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Vector() {};

    constexpr explicit Vector(const Allocator& alloc) noexcept : alloc_(alloc) {};

    constexpr Vector(size_t count, const T& value = T{}, const Allocator& alloc = Allocator()) : alloc_(alloc) {
        for (size_t i = 0; i < count; i++) {
            push_back(value);
        }
    }

    void swap(Vector& other) noexcept {
        std::swap(alloc_, other.alloc_);
        std::swap(data_, other.data_);
        std::swap(real_size_, other.real_size_);
        std::swap(capacity_, other.capacity_);
    }

    Vector(const Vector& vec) : alloc_(vec.alloc_) {
        for (int i = 0; i < vec.size(); i++) {
            push_back(vec.data_[i]);
        }
    }

    Vector(Vector&& vec) noexcept : alloc_(std::move(vec.alloc_)), data_(vec.data_),
     real_size_(vec.real_size_), capacity_(vec.capacity_) {
        vec.data_ = nullptr;
        vec.real_size_ = 0;
        vec.capacity_ = 0;
    }

    Vector(const Vector& vec, const Allocator& alloc) : alloc_(alloc) {
        for (int i = 0; i < vec.size(); i++) {
            push_back(vec.data_[i]);
        }
    }

    Vector(Vector&& vec, const Allocator& alloc) : alloc_(alloc), data_(vec.data_),
        real_size_(vec.real_size_), capacity_(vec.capacity_) {
        vec.data_ = nullptr;
        vec.real_size_ = 0;
        vec.capacity_ = 0;
    }

    Vector(std::initializer_list<T> ilist, const Allocator& alloc = Allocator()) : alloc_(alloc) {
        (*this) = ilist;
    }

    Vector& operator=(const Vector& vec) noexcept {
		Vector copy(vec);
        swap(copy);
        return *this;
    }
    Vector& operator=(Vector&& vec) noexcept {
		Vector copy(std::move(vec));
        swap(copy);
        return *this;
    }

    Vector& operator=(std::initializer_list<T> ilist) {
        for (const auto& i : ilist) {
            push_back(i);
        }
        return *this;
    }

    void allocate() {
        size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2; 
        T* new_massive = std::allocator_traits<Allocator>::allocate(alloc_, new_capacity);
        for (size_t i = 0; i < real_size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, new_massive + i, data_[i]);
        }
        for (size_t i = 0; i < real_size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        data_ = new_massive;
        capacity_ = new_capacity;
    }

    template <typename... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args) {
        if (real_size_ == capacity_) {
            allocate();
        }
        
        const size_type insert_pos = pos - cbegin();
        
        std::allocator_traits<Allocator>::construct(alloc_, data_ + real_size_, std::forward<Args>(args)...);
        real_size_++;
        
        for (size_type i = real_size_ - 1; i > insert_pos; --i) {
            std::swap(data_[i], data_[i - 1]);
        }
        
        return begin() + insert_pos + 1;
    }

    template <typename... Args>
    constexpr iterator emplace(iterator pos, Args&&... args) {
        if (real_size_ == capacity_) {
            allocate();
        }
        
        size_type insert_pos = pos - begin();
        
        std::allocator_traits<Allocator>::construct(alloc_, data_ + real_size_, std::forward<Args>(args)...);
        real_size_++;
        
        for (size_type i = real_size_ - 1; i > insert_pos; --i) {
            std::swap(data_[i], data_[i - 1]);
        }
        
        return begin() + insert_pos + 1;
    }

    constexpr iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    constexpr iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    constexpr iterator insert(iterator pos, const T& value) {
        return emplace(pos, value);
    }

    constexpr iterator insert(iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    constexpr iterator insert(const_iterator pos, size_type count, const T& value) {
        for (size_t i = 0; i < count - 1; i++) {
            insert(pos, value);
        }
        return insert(pos, value);
    }

    constexpr iterator insert(iterator pos, size_type count, const T& value) {
        for (size_t i = 0; i < count - 1; i++) {
            insert(pos, value);
        }
        return insert(pos, value);
    }

    template<Dereferenceable InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
        while (first != last) {
            pos = insert(pos, *first);
            first++;
        }
        return pos;
    }

    template<Dereferenceable InputIt>
    constexpr iterator insert(iterator pos, InputIt first, InputIt last) {
        while (first != last) {
            pos = insert(pos, *first);
            first++;
        }
        return pos;
    }

    constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
        for (const auto& i : ilist) {
            pos = insert(pos, i);
        }
        return pos;
    }

    constexpr iterator insert(iterator pos, std::initializer_list<T> ilist) {
        for (const auto& i : ilist) {
            pos = insert(pos, i);
        }
        return pos;
    }

    template <typename... Args>
    constexpr void emplace_back(Args&&... args) {
        if (real_size_ == capacity_) {
            allocate();
        }
        std::allocator_traits<Allocator>::construct(alloc_, data_ + real_size_, std::forward<Args>(args)...);
        real_size_++;
    }

    constexpr void push_back(const T& value) {
        emplace_back(value);
    }

    constexpr void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template <std::ranges::input_range Range>
    constexpr void append_range(Range&& range) {
        if constexpr (std::ranges::sized_range<Range>) {
            reserve(real_size_ + std::ranges::size(range));
        }
        
        for (auto&& i : range) {
            emplace_back(std::forward<decltype(i)>(i));
        }
    }

    template <std::ranges::input_range Range>
    constexpr void assign_range(Range&& range) {
        clear();
        append_range(std::forward<Range>(range));
    }

    template <std::ranges::input_range Range>
    constexpr iterator insert_range(const_iterator pos, Range&& range) {
        const size_type insert_pos = pos - cbegin();
        
        if constexpr (std::ranges::sized_range<Range>) {
            reserve(real_size_ + std::ranges::size(range));
        }
        
        Vector<T> tail;
        tail.reserve(real_size_ - insert_pos);
        
        for (auto it = begin() + insert_pos; it != end(); ++it) {
            tail.emplace_back(std::move(*it));
        }
        
        real_size_ = insert_pos;
        
        for (auto&& i : range) {
            emplace_back(std::forward<decltype(i)>(i));
        }
        
        for (auto&& i : tail) {
            emplace_back(std::move(i));
        }
        
        return begin() + insert_pos;
    }

    template <std::ranges::input_range Range>
    constexpr iterator insert_range(iterator pos, Range&& range) {
        size_type insert_pos = pos - begin();
        
        if constexpr (std::ranges::sized_range<Range>) {
            reserve(real_size_ + std::ranges::size(range));
        }
        
        Vector<T> tail;
        tail.reserve(real_size_ - insert_pos);
        
        for (auto it = begin() + insert_pos; it != end(); ++it) {
            tail.emplace_back(std::move(*it));
        }
        
        real_size_ = insert_pos;
        
        for (auto&& i : range) {
            emplace_back(std::forward<decltype(i)>(i));
        }
        
        for (auto&& i : tail) {
            emplace_back(std::move(i));
        }
        
        return begin() + insert_pos;
    }

    void pop_back() noexcept {
        if (real_size_ == 0) {
            return;
        }
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + real_size_);
        if (real_size_ == 0) {
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
            return;
        }
        real_size_--;
    }

    reference operator[] (size_t index) {
        return data_[index];
    }

    const_reference operator[] (size_t index) const {
        return data_[index];
    }

    reference at(size_t index) {
        return data_[index];
    }

    const_reference at(size_t index) const {
        return data_[index];
    }

    reference front() {
        return data_[0];
    }

    const_reference front() const {
        return data_[0];
    }

    reference back() {
        return data_[real_size_ - 1];
    }

    const_reference back() const {
        return data_[real_size_ - 1];
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }

    size_t size() const {
        return real_size_;
    }

    size_t max_size() const {
        return capacity_;
    }

    bool empty() const noexcept {
        return (real_size_ == 0);
    }

    void clear() noexcept {
        while (!empty()) {
            pop_back();
        }
    }

    void assign(size_t count, const T& value) {
        clear();
        for (size_t i = 0; i < count; i++) {
            push_back(value);
        }
    }

    constexpr Allocator get_allocator() const {
        return alloc_;
    }

    constexpr void resize(size_type size, const T& value = T{}) {
        if (size > capacity_) {
            reserve(size);
        }
        for (size_t i = real_size_; i < size; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
        }
        for (size_t i = size; i < real_size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        real_size_ = size;
    }

    constexpr void reserve(size_t size) {
        T* new_massive = std::allocator_traits<Allocator>::allocate(alloc_, size);
        for (size_t i = 0; i < real_size_; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, new_massive + i, data_[i]);
        }
        for (size_t i = 0; i < real_size_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
        data_ = new_massive;
        capacity_ = size;
    }

    size_t capacity() const {
        return capacity_;
    }

    constexpr void assign(std::initializer_list<T> ilist) {
        clear();
        for (const auto& i : ilist) {
            push_back(i);
        }
    }

    iterator begin() {
        return iterator{data_, 0};
    }

    const_iterator begin() const {
        return const_iterator{data_, 0};
    }

    const_iterator cbegin() const {
        return const_iterator{data_, 0};
    }

    reverse_iterator rbegin() {
        return std::reverse_iterator<iterator>(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    iterator end() {
        return iterator{data_, real_size_};
    }

    const_iterator end() const {
        return const_iterator{data_, real_size_};
    }

    const_iterator cend() const {
        return const_iterator{data_, real_size_};
    }

    reverse_iterator rend() {
        return std::reverse_iterator<iterator>(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << " ";
    }
    return os;
}
