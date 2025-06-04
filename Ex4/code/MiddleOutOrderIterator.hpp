// ofekbarshalom@gmail.com

#ifndef MIDDLEOUT_ORDER_ITERATOR_HPP
#define MIDDLEOUT_ORDER_ITERATOR_HPP

#include <vector>
#include <cstddef>

using namespace std;

namespace container {

    template<typename T>
    class MyContainer;

    template<typename T>
    class MiddleOutOrderIterator {

        private:
            const MyContainer<T>& container;
            vector<size_t> middleOutIndices;
            size_t pos;

            void build_order() {
                size_t n = container.size();
                if (n == 0) return;

                size_t mid = n / 2;
                middleOutIndices.push_back(mid);

                size_t left = (mid > 0) ? mid - 1 : 0;
                size_t right = mid + 1;

                bool toggle = true;
                while (middleOutIndices.size() < n) {
                    if (toggle && left < mid) {
                        middleOutIndices.push_back(left--);
                    } else if (!toggle && right < n) {
                        middleOutIndices.push_back(right++);
                    }
                    toggle = !toggle;
                }
            }

        public:
            MiddleOutOrderIterator(const MyContainer<T>& cont, size_t start_pos = 0): container(cont), pos(start_pos) {
                build_order();
            }

            const T& operator*() const {
                if (pos >= container.size()) {
                    throw out_of_range("Iterator out of bounds");
                }
                return container.elements[middleOutIndices[pos]];
            }

            MiddleOutOrderIterator& operator++() {
                if (pos >= container.size()) {
                    throw out_of_range("Cannot increment iterator past end");
                }
                ++pos;
                return *this;
            }

            bool operator==(const MiddleOutOrderIterator& other) const {
                return pos == other.pos && &container == &other.container;
            }

            bool operator!=(const MiddleOutOrderIterator& other) const {
                return !(*this == other);
            }
    };

} // namespace container

#endif
