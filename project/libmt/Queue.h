#pragma once

#include <mutex>
#include <queue>
#include <cstddef>
#include <memory>

namespace mt
{
  template <typename T>
  class Queue
  {
  public:
    Queue() : m_queue(), m_mutex(std::make_unique<std::mutex>())
    {
    }
    ~Queue()
    {
    }

    Queue(Queue const &) = delete;
    Queue(Queue &&) = delete;

    Queue &operator=(Queue const &) = delete;
    Queue &operator=(Queue &&) = delete;

    // Access
    T const &front() const
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      return m_queue.front();
    }

    T const &back() const
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      return m_queue.back();
    }

    // Capacity
    bool empty() const
    {
      std::lock_guard<std::mutex> lock{*m_mutex};
      return m_queue.empty();
    }

    std::size_t size() const
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      return m_queue.size();
    }

    // Modifier
    void push(T const &value)
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      m_queue.push(value);
    }

    void push(T &&value)
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      m_queue.push(value);
    }

    void pop()
    {
      std::lock_guard<std::mutex> lock(*m_mutex);
      m_queue.pop();
    }

  private:
    std::queue<T>                       m_queue;
    mutable std::unique_ptr<std::mutex> m_mutex;
  };
}
