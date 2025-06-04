// ofekbarshalom@gmail.com

#ifndef REVERSE_ORDER_ITERATOR_HPP
#define REVERSE_ORDER_ITERATOR_HPP

#include <vector>
#include <cstddef>

using namespace std;

namespace container {

    template<typename T>
    class MyContainer;

    template<typename T>
    class ReverseOrderIterator {

        private:
            const MyContainer<T>& container;
            size_t pos;

        public:
            ReverseOrderIterator(const MyContainer<T>& cont, size_t start_pos): container(cont), pos(start_pos) {}

            const T& operator*() const {
                if (pos >= container.size()) {
                    throw out_of_range("Iterator out of bounds");
                }
                return container.elements[pos];
            }

            ReverseOrderIterator& operator++() {
                if (pos == 0) {
                    pos = container.size();  // Mark as end
                } else {
                    --pos;
                }
                return *this;
            }

            bool operator==(const ReverseOrderIterator& other) const {
                return pos == other.pos && &container == &other.container;
            }

            bool operator!=(const ReverseOrderIterator& other) const {
                return !(*this == other);
            }
    };

} // namespace container

#endif
