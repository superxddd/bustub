//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

/**
 * 创建一个新的 LRUReplacer。
 * @param num_pages LRUReplacer 需要管理的最大页数
 */
LRUReplacer::LRUReplacer(size_t num_pages): lru_list_(), f_map_(num_pages) {
    
}

/**
 * 析构 LRUReplacer。
 */
LRUReplacer::~LRUReplacer() = default;

auto LRUReplacer::Victim(frame_id_t *frame_id) -> bool {
    if(lru_list_.empty()) {
        return false;
    }
    mutex_.lock();
    *frame_id = lru_list_.back();
    lru_list_.pop_back();

    f_map_.erase(*frame_id);
    mutex_.unlock();
     return true; 
    }

void LRUReplacer::Pin(frame_id_t frame_id) {
    if(f_map_.find(frame_id) == f_map_.end()) {
        return;
    }
    mutex_.lock();
    lru_list_.erase(f_map_[frame_id]);
    f_map_.erase(frame_id);
    mutex_.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    if(f_map_.find(frame_id) != f_map_.end()) {
        return;
    }
    mutex_.lock();
    lru_list_.push_front(frame_id);
    f_map_[frame_id] = lru_list_.begin();
    mutex_.unlock();
}


auto LRUReplacer::Size() -> size_t { return lru_list_.size(); }

}  // namespace bustub
