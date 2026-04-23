#include <iostream>
#include "my_avl_tree.hpp"

int main() {
    std::cout << "AVL debug environment is ready.\n";

    avl_tree<int> my_avl;

    my_avl.insert(10);
    my_avl.insert(20);
    my_avl.insert(30);

    my_avl.inorder();
    my_avl.print_tree();

    std::cout << "插入5， 15， 2， 使其不平衡" << std::endl;

    my_avl.insert(5);
    my_avl.insert(15);
    my_avl.insert(2);

    my_avl.inorder();
    my_avl.print_tree();
    my_avl.insert(1);
    my_avl.print_tree();
    std::cout << "删除2， 使平衡" << std::endl;
    my_avl.remove(2);
    my_avl.print_tree();

    std::cout << "删除10， 使平衡" << std::endl;
    my_avl.remove(10);
    my_avl.print_tree();

    return 0;
}