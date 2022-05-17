#include <stdio.h>
#include <utility>
#include <memory>

//Weak and Shared Ptr implementation


namespace smart_ptr {

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr;

class ControlBlockBase {
  public:
    ControlBlockBase() : shared_count_(1), weak_count_(0) {}
    ControlBlockBase(size_t shared_count, size_t weak_count) : shared_count_(shared_count), weak_count_(weak_count) {}
    virtual ~ControlBlockBase() {};

    void incShared() {
        ++shared_count_;
    }
    size_t decShared() {
        return --shared_count_;
    }
    void incWeak() { 
        ++weak_count_;
    }
    size_t decWeak() { 
        return --weak_count_;
    }

    size_t sharedCount() const {
        return shared_count_;
    }
    size_t weakCount() const {
        return weak_count_;
    }

    virtual void deleteObject() = 0;

    bool is_deleted() {
        return is_deleted_;
    }

  private:
    size_t shared_count_;
    size_t weak_count_;
  
  protected:
    bool is_deleted_;
};

template <typename T>
class ControlBlockOwner : public ControlBlockBase {
  public:
    template <typename... Args>
    ControlBlockOwner(Args&&... args) : object_(std::forward<Args>(args)...) {}
    virtual ~ControlBlockOwner() override {};

    virtual void deleteObject() override {
        object_.~T();
        is_deleted_ = true;
    }

    T* get() {
        return &object_;
    }

  private:
    T object_;
};

template <typename T>
class ControlBlockUser : public ControlBlockBase {
  public:
    ControlBlockUser(T* ptr) : object_(ptr) {}
    virtual ~ControlBlockUser() override {
    }
    
    virtual void deleteObject() override {
        delete object_;
        is_deleted_ = true;
    }

  private:
    T* object_;
};

template <typename T>
class SharedPtr {
  public:
    //constructing
    SharedPtr() : ptr_(nullptr), block_(nullptr) {};
    SharedPtr(T* ptr) : ptr_(ptr) {
        block_ = new ControlBlockUser<T>(ptr);
    }
    SharedPtr(const SharedPtr<T>& shared_ptr) : ptr_(shared_ptr.ptr_), block_(shared_ptr.block_) {
        block_->incShared();
    }
    SharedPtr(SharedPtr<T>&& shared_ptr) : ptr_(std::move(shared_ptr.ptr_)), block_(std::move(shared_ptr.block_)){    
    }
    ~SharedPtr() {
        if (block_ != nullptr) {
            if (block_->decShared() == 0) {
                block_->deleteObject();
                if (block_->weakCount() == 0) {
                    delete block_;
                }
            }
        }
    }

    void swap(SharedPtr<T>& shared) {
        std::swap(shared.ptr_, ptr_);
        std::swap(shared.block_, block_);
    }
    void reset() {
        SharedPtr<T>().swap(*this);
    }
    void replace(T* ptr) {
        SharedPtr<T>(ptr).swap(*this);
    }

    SharedPtr& operator=(const SharedPtr<T>& ptr) {
        SharedPtr<T>(ptr).swap(*this);
    }
    SharedPtr& operator=(SharedPtr<T>&& ptr) {
        SharedPtr<T>(std::move(ptr)).swap(*this);
    }

    size_t count() const {
        return block_->sharedCount();
    }
    
    operator bool() {
        return ptr_ != nullptr;
    }

    T* get() {
        return ptr_;
    }
    T& operator*() {
        return *ptr_;
    }
    T* operator->() {
        return ptr_;
    }

    friend class WeakPtr<T>;
    
    template <typename Type, typename... Args>
    friend SharedPtr<Type> MakeShared(Args&&... args );
  private:
    T* ptr_;
    ControlBlockBase* block_;

    SharedPtr(ControlBlockOwner<T>* owner) : block_(owner), ptr_(owner->get()) {}
};

template <typename T>
class WeakPtr {
  public:
    WeakPtr() : ptr_(nullptr), block_(nullptr) {}
    WeakPtr(const WeakPtr<T>& weak) : ptr_(weak.ptr_), block_(weak.block_) {
        block_->incWeak();
    }
    WeakPtr(const SharedPtr<T>& shared) : ptr_(shared.ptr_), block_(shared.block_) {
        block_->incWeak();
    }
    WeakPtr(WeakPtr<T>&& weak) : ptr_(std::move(weak.ptr_)), block_(std::move(weak.block_)) {
    }
    ~WeakPtr() {
        if (block_ != nullptr) {
            if (block_->decWeak() == 0 && block_->sharedCount() == 0) {
                delete block_;
            }
        }
    }

    WeakPtr& operator=(const WeakPtr<T>& ptr) {
        WeakPtr<T>(ptr).swap(*this);
    }
    WeakPtr& operator=(WeakPtr<T>&& ptr) {
        WeakPtr<T>(std::move(ptr)).swap(*this);
    }

    size_t count() {
        return block_->sharedCount();
    }

    bool expired() {
        return block_->is_deleted();
    }
    
    void swap(WeakPtr<T>& weak) {
        std::swap(weak.ptr_, ptr_);
        std::swap(weak.block_, block_);
    }
    void reset() {
        WeakPtr<T>().swap(*this);
    }
    SharedPtr<T> lock() {
        if(this->expired()) {
            return SharedPtr<T>();
        }
        return SharedPtr<T>(*this);
    }

    friend class SharedPtr<T>;
  private:
    T* ptr_;
    ControlBlockBase* block_;
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new ControlBlockOwner<T>(std::forward<Args>(args)...));
};

} //namespace smart_ptr