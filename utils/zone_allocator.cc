#include "zone_allocator.hh"

namespace Cougar::Utils {

constexpr std::size_t BLOCK_SIZE = 8000;

void *ZoneAllocator::doAllocate(std::size_t size, std::size_t alignment) {

  mAllocated += size;

  if (mBlocks.empty()) {
    return allocateInNewBlock(size, alignment);
  }

  Block &block = mBlocks.front();

  void *aligned = std::align(alignment, size, block.mCurrent, block.mSpace);
  block.mCurrent = (std::byte *)aligned + size;

  if (!aligned)
    return allocateInNewBlock(size, alignment);

  return aligned;
}

void *ZoneAllocator::allocateInNewBlock(std::size_t size,
                                        std::size_t alignment) {
  Block &block = mBlocks.emplace_front();
  block.mSpace = std::max(size + alignment,
                          BLOCK_SIZE); // add alignment to have some wiggle-room
                                       // for over-aligned types
  block.mData = std::malloc(block.mSpace);
  block.mCurrent = block.mData;

  mUsed += block.mSpace;

  void *aligned = std::align(alignment, size, block.mCurrent, block.mSpace);
  block.mCurrent = (std::byte *)aligned + size;
  return aligned;
}

ZoneAllocator::Block::~Block() { std::free(mData); }

} // namespace Cougar::Utils