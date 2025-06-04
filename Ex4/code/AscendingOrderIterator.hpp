// ofekbarshalom@gmail.com

#ifndef ASCENDING_ORDER_ITERATOR_HPP
#define ASCENDING_ORDER_ITERATOR_HPP

#include <vector>
#include <algorithm>
#include <cstddef>

using namespace std;

namespace container {

    template<typename T>
    class MyContainer;

    template<typename T>
    class AscendingOrderIterator {

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
                        return container.elements[a] < container.elements[b];
                    });
            }
        
        public:
            AscendingOrderIterator(const MyContainer<T>& cont, size_t start_pos = 0): container(cont), pos(start_pos) {
                build_indices();
            }

            const T& operator*() const {
                if (pos >= container.size()) {
                    throw out_of_range("Iterator out of bounds");
                }
                return container.elements[sortedIndices[pos]];
            }

            AscendingOrderIterator& operator++() {
                if (pos >= container.size()) {
                    throw out_of_range("Cannot increment iterator past end");
                }
                ++pos;
                return *this;
            }

            bool operator==(const AscendingOrderIterator& other) const {
                return pos == other.pos && &container == &other.container;
            }

            bool operator!=(const AscendingOrderIterator& other) const {
                return !(*this == other);
            }
    };

} // namespace container

#endif