#pragma once

namespace tachyon::memory {

template <typename T>
class UniquePtr {
private: 
    T* ptr_{nullptr};  
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
    
    ~UniquePtr() {
        delete ptr_;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    T* get() const {
        return ptr_;
    }
    T& operator*() const { 
        return *ptr_; 
    }
    T* operator->() const {
        return ptr_;
    }
};

}