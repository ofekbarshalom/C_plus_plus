// ofekbarshalom@gmail.com

#include <iostream>
#include "MyContainer.hpp"

using namespace std;
using namespace container;

int main() {
    // Integer container
    MyContainer<int> container;
    container.add(7);
    container.add(15);
    container.add(6);
    container.add(1);
    container.add(2);
    container.add(20);

    cout << "\n[ Integer container ]\n";
    cout << "Size: " << container.size() << endl;
    cout << "Elements: " << container << "\n";

    container.remove(20);

    cout << "\n[ Integer container after removing 20 ]\n";
    cout << "Size: " << container.size() << endl;
    cout << "Elements: " << container << "\n";

    cout << "\nAscending: ";
    for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) cout << *it << " ";
    cout << "\nDescending: ";
    for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it) cout << *it << " ";
    cout << "\nSide-Cross: ";
    for (auto it = container.begin_sidecross_order(); it != container.end_sidecross_order(); ++it) cout << *it << " ";
    cout << "\nReverse: ";
    for (auto it = container.begin_reverse_order(); it != container.end_reverse_order(); ++it) cout << *it << " ";
    cout << "\nOrder: ";
    for (auto it = container.begin_order(); it != container.end_order(); ++it) cout << *it << " ";
    cout << "\nMiddle-Out: ";
    for (auto it = container.begin_middleout_order(); it != container.end_middleout_order(); ++it) cout << *it << " ";
    cout << "\n\n";

    // String container
    MyContainer<string> s;
    s.add("banana");
    s.add("apple");
    s.add("pear");

    cout << "[ String container ]\n";
    cout << "Elements: " << s << "\nAscending: ";
    for (auto it = s.begin_ascending_order(); it != s.end_ascending_order(); ++it) cout << *it << " ";
    cout << "\n\n";

    // Double container
    MyContainer<double> d;
    d.add(5.1);
    d.add(2.2);
    d.add(9.3);

    cout << "[ Double container ]\n";
    cout << "Elements: " << d << "\nDescending: ";
    for (auto it = d.begin_descending_order(); it != d.end_descending_order(); ++it) cout << *it << " ";
    cout << "\n\n";

    // Char container
    MyContainer<char> c;
    c.add('z');
    c.add('a');
    c.add('m');

    cout << "[ Char container ]\n";
    cout << "Elements: " << c << "\nMiddle-Out: ";
    for (auto it = c.begin_middleout_order(); it != c.end_middleout_order(); ++it) cout << *it << " ";
    cout << "\n";

    return 0;
}
