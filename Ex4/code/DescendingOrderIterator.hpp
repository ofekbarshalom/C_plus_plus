// ofekbarshalom@gmail.com

#ifndef DESCENDING_ORDER_ITERATOR_HPP
#define DESCENDING_ORDER_ITERATOR_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

using namespace std;

namespace container {

    template<typename T>
    class MyContainer;

    template<typename T>
    class DescendingOrderIterator {

        private:
            const MyContainer<T>& container;
            vector<size_t> sortedIndices;
            size_t pos;

            void build_indices() {
                sortedIndices.resize(container.size());
                for (size_t i = 0; i < container.size(); ++i) {
                    sortedIndices[i] = i;
                }
                sort(sortedIndices.begin(), sortedIndices.end(),
                    [this](size_t a, size_t b) {
                        return container.elements[a] > container.elements[b];
                    });
            }

        public:
            DescendingOrderIterator(const MyContainer<T>& cont, size_t start_pos = 0): container(cont), pos(start_pos) {
                build_indices();
            }

            const T& operator*() const {
                if (pos >= container.size()) {
                    throw out_of_range("Iterator out of bounds");
                }
                return container.elements[sortedIndices[pos]];
            }

            DescendingOrderIterator& operator++() {
                if (pos >= container.size()) {
                    throw out_of_range("Cannot increment iterator past end");
                }
                ++pos;
                return *this;
            }

            bool operator==(const DescendingOrderIterator& other) const {
                return pos == other.pos && &container == &other.container;
            }

            bool operator!=(const DescendingOrderIterator& other) const {
                return !(*this == other);
            }
    };

} // namespace container

#endif
