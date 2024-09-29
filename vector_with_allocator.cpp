#include <iostream>
#include <memory>
#include <cassert>

template <typename T>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() = default;

    template <typename U>
    MyAllocator(const MyAllocator<U>&) {}

    T* allocate(std::size_t n) {
        if (auto p = std::malloc(n * sizeof(T))) {
            return static_cast<T*>(p);
        }
        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) {
        std::free(p);
    }
};

template <typename T, typename Alloc = MyAllocator<T>>
class MyVector {
public:
    using allocator_type = Alloc;

    MyVector(size_t initial_capacity = 1) 
        : capacity_(initial_capacity), size_(0) {
        data_ = allocator_.allocate(capacity_);
    }

    ~MyVector() {
        clear();
        allocator_.deallocate(data_, capacity_);
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    T& operator[](size_t index) {
        assert(index < size_);
        return data_[index];
    }

    const T& operator[](size_t index) const {
        assert(index < size_);
        return data_[index];
    }

    size_t size() const {
        return size_;
    }

private:
    void resize(size_t new_capacity) {
        T* new_data = allocator_.allocate(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(data_[i]);
            data_[i].~T();
        }
        allocator_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    Alloc allocator_;
    T* data_;
    size_t size_;
    size_t capacity_;
};

int main() {
    MyVector<int> vec;

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}