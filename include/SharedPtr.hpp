//
// Created by mrbgn on 12.12.2020.
//

#ifndef TEMPLATE_SHAREDPTR_HPP
#define TEMPLATE_SHAREDPTR_HPP
#include <atomic>
#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {
 public:

  SharedPtr() {
    _ptr = nullptr;
    _control = nullptr;
  }

  explicit SharedPtr(T* ptr)
  {
    _ptr = ptr;
    if (_ptr == nullptr)
    {
      _control = nullptr;
    } else {
      _control = new std::atomic_uint;
      *_control = 1;
    }
  }

  SharedPtr(const SharedPtr& copy)
  {
    _ptr = copy._ptr;
    _control = copy._control;
    if (_ptr) {
      *_control = *_control + 1;
    } else {
      _control = nullptr;
    }
  }

  SharedPtr(SharedPtr&& move)
  {
    _ptr = move._ptr;
    _control = move._control;
    move._ptr = nullptr;
    move._control = nullptr;
  }

  ~SharedPtr()
  {
    if (_ptr){
      if (*_control == 1)
      {
        delete _ptr;
        delete _control;
      } else {
        *_control = *_control - 1;
      }
    }
    _ptr = nullptr;
    _control = nullptr;
  }

  auto operator=(const SharedPtr& eq_copy) -> SharedPtr&
  {
    if (this != &eq_copy) {
      //
      _ptr = eq_copy._ptr;
      _control = eq_copy._control;
      if (_ptr) {
        *_control = *_control + 1;
      } else {
        _control = nullptr;
      }
    }
    return *this;
  }

  auto operator=(const SharedPtr&& eq_move) -> SharedPtr&
  {
    if (this != &eq_move){
      _ptr = eq_move._ptr;
      _control = eq_move._control;
    }
    return *this;
  }

  // проверяет, указывает ли указатель на объект
  operator bool() const
  {
    if (_ptr) return true;
    else
      return false;
  }

  auto operator*() const -> T&
  {
    if (_ptr) return *_ptr;
    else
      return nullptr;
  }
  auto operator->() const -> T*
  {
    return _ptr;
  }

  auto get() -> T*
  {
    return _ptr;
  }

  void reset()
  {
    if (*_control == 1)
    {
      delete _ptr;
      delete _control;
    } else {
      *_control = *_control - 1;
    }
    _ptr = nullptr;
    _control = nullptr;
  }

  void reset(T* ptr)
  {
    if (*_control == 1)
    {
      delete _ptr;
      delete _control;
    } else {
      *_control = *_control - 1;}
    _ptr = ptr;
    if (ptr == nullptr)
    {
      _control = nullptr;
    } else {
      _control = new std::atomic_uint;
      *_control = 1;
    }
  }

  void swap(SharedPtr& r)
  {
    T* tmp_ptr(std::move(r._ptr));
    r._ptr = std::move(_ptr);
    _ptr = std::move(tmp_ptr);
    std::atomic_uint* tmp_control = r._control;
    r._control = _control;
    _control = tmp_control;
  }

  // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
  auto use_count() const -> size_t
  {
    if (_control) {
      return *_control;
    }
    else return 0;
  }

 private:
  T* _ptr;
  std::atomic_uint* _control;
};

#endif  // TEMPLATE_SHAREDPTR_HPP
