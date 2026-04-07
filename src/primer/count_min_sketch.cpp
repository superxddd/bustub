//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// count_min_sketch.cpp
//
// Identification: src/primer/count_min_sketch.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/count_min_sketch.h"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>

namespace bustub {

/**
 * count-min sketch 的构造函数。
 *
 * @param width sketch 矩阵的宽度。
 * @param depth sketch 矩阵的深度。
 * @throws std::invalid_argument 当 width 或 depth 为 0 时抛出。
 */
template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(uint32_t width, uint32_t depth)
    : width_(width), depth_(depth), sketch_(width * depth) {
  /** @TODO(student) Implement this function! */
  if (width == 0 || depth == 0) throw(std::invalid_argument("wrong"));

  for (auto &x:sketch_)
  {
    x.store(0,std::memory_order_relaxed);
  }
  
  /** @spring2026 请不要修改以下内容 */
  // 初始化带种子的哈希函数
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(CountMinSketch &&other) noexcept
    : width_(other.width_), depth_(other.depth_), sketch_(std::move(other.sketch_)) {
  /** @TODO(student) Implement this function! */

  hash_functions_.clear();
  hash_functions_.reserve(depth_);

  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::operator=(CountMinSketch &&other) noexcept -> CountMinSketch & {
  /** @TODO(student) Implement this function! */
  if (this == &other) return *this;
  this->sketch_ = std::move(other.sketch_);
  this->width_ = other.width_;
  this->depth_ = other.depth_;

  hash_functions_.clear();
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }

  return *this;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Insert(const KeyType &item) {
  /** @TODO(student) Implement this function! */
  for (size_t i = 0; i < depth_; i++) {
    size_t hashcount = (hash_functions_[i](item));
    (this->sketch_[i * width_ + hashcount]).fetch_add(1,std::memory_order_relaxed);
    
  }
}

template <typename KeyType>
void CountMinSketch<KeyType>::Merge(const CountMinSketch<KeyType> &other) {
  if (width_ != other.width_ || depth_ != other.depth_) {
    throw std::invalid_argument("Incompatible CountMinSketch dimensions for merge.");
  }
  /** @TODO(student) Implement this function! */
  for (size_t i = 0; i < this->sketch_.size(); i++) {
    const auto value = other.sketch_[i].load(std::memory_order_relaxed);
    this->sketch_[i].fetch_add(value,std::memory_order_relaxed);
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::Count(const KeyType &item) const -> uint32_t {
  uint32_t res = std::numeric_limits<uint32_t>::max();
  for (size_t i = 0; i < depth_; i++) {
    size_t hashcount = (hash_functions_[i](item));

    const auto value = (this->sketch_[i * width_ + hashcount]).load(std::memory_order_relaxed);
    res = std::min(res,value);
  }
  return res;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Clear() {
  /** @TODO(student) Implement this function! */
  for(auto &x:sketch_){
    x.store(0,std::memory_order_relaxed);
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::TopK(uint16_t k, const std::vector<KeyType> &candidates)
    -> std::vector<std::pair<KeyType, uint32_t>> {
  /** @TODO(student) Implement this function! */
  std::vector<std::pair<KeyType, uint32_t>> resvec;
  for (size_t i = 0; i < candidates.size(); i++) {
    resvec.push_back({candidates[i], this->Count(candidates[i])});
  }
  std::stable_sort(resvec.begin(), resvec.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
  if (resvec.size() > k) resvec.resize(k);
  return resvec;
}

// 为测试中使用到的所有类型进行显式实例化
template class CountMinSketch<std::string>;
template class CountMinSketch<int64_t>;  // 用于 int64_t 测试
template class CountMinSketch<int>;      // 这里同时覆盖 int 和 int32_t
}  // namespace bustub
