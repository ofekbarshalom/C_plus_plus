// ofekbarshalom@gmail.com

#ifndef MY_CONTAINER_HPP
#define MY_CONTAINER_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "AscendingOrderIterator.hpp"
#include "DescendingOrderIterator.hpp"
#include "SideCrossOrderIterator.hpp"
#include "ReverseOrderIterator.hpp"
#include "OrderIterator.hpp"
#include "MiddleOutOrderIterator.hpp"

using namespace std;

namespace container {

    template <typename T = int>
    class MyContainer {

        private:
            vector<T> elements;

        public:
            MyContainer() = default;
            ~MyContainer() = default;

            void add(const T& value) {
                elements.push_back(value);
            }

            void remove(const T& value) {
                auto it = std::remove(elements.begin(), elements.end(), value);
                if (it == elements.end()) {
                    throw runtime_error("Value not found in container");
                }
                elements.erase(it, elements.end());
            }

            size_t size() const {
                return elements.size();
            }

            friend ostream& operator<<(ostream& os, const MyContainer& container) {
                os << "[ ";
                for (const auto& val : container.elements) {
                    os << val << " ";
                }
                os << "]";
                return os;
            }

            friend class AscendingOrderIterator<T>;
            friend class DescendingOrderIterator<T>;
            friend class SideCrossOrderIterator<T>;
            friend class ReverseOrderIterator<T>;
            friend class OrderIterator<T>;
            friend class MiddleOutOrderIterator<T>;

            auto begin_ascending_order() const { 
                return AscendingOrderIterator<T>(*this, 0); 
            }

            auto end_ascending_order() const { 
                return AscendingOrderIterator<T>(*this, elements.size()); 
            }

            auto begin_descending_order() const {
                return DescendingOrderIterator<T>(*this, 0);
            }

            auto end_descending_order() const {
                return DescendingOrderIterator<T>(*this, elements.size());
            }

            auto begin_sidecross_order() const {
                return SideCrossOrderIterator<T>(*this, 0);
            }

            auto end_sidecross_order() const {
                return SideCrossOrderIterator<T>(*this, elements.size());
            }

            auto begin_reverse_order() const {
                return ReverseOrderIterator<T>(*this, elements.size() - 1);
            }

            auto end_reverse_order() const {
                return ReverseOrderIterator<T>(*this, elements.size());
            }

            auto begin_order() const {
                return OrderIterator<T>(*this, 0);
            }

            auto end_order() const {
                return OrderIterator<T>(*this, elements.size());
            }

            auto begin_middleout_order() const {
                return MiddleOutOrderIterator<T>(*this, 0);
            }

            auto end_middleout_order() const {
                return MiddleOutOrderIterator<T>(*this, elements.size());
            }
    };

} // namespace container

#endif // MY_CONTAINER_HPP
