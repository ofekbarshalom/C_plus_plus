// ofekbarshalom@gmail.com

#ifndef ORDER_ITERATOR_HPP
#define ORDER_ITERATOR_HPP

#include <vector>
#include <cstddef>

using namespace std;

namespace container {

    template<typename T>
    class MyContainer;

    template<typename T>
    class OrderIterator {

        private:
            const MyContainer<T>& container;
            size_t pos;

        public:
            OrderIterator(const MyContainer<T>& cont, size_t start_pos = 0): container(cont), pos(start_pos) {}

            const T& operator*() const {
                if (pos >= container.size()) {
                    throw out_of_range("Iterator out of bounds");
                }
                return container.elements[pos];
            }

            OrderIterator& operator++() {
                if (pos >= container.size()) {
                    throw out_of_range("Cannot increment iterator past end");
                }
                ++pos;
                return *this;
            }

            bool operator==(const OrderIterator& other) const {
                return pos == other.pos && &container == &other.container;
            }

            bool operator!=(const OrderIterator& other) const {
                return !(*this == other);
            }
    };

} // namespace container

#endif
