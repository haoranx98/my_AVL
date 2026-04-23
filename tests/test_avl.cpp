#include <iostream>
#include <vector>
#include <cassert>
#include "my_avl_tree.hpp"

// 简易测试框架
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name)                                              \
    do {                                                        \
        std::cout << "  [RUN ] " << name << std::flush;         \
    } while (0)

#define PASS(name)                                              \
    do {                                                        \
        std::cout << " -> [PASS]" << std::endl;                 \
        tests_passed++;                                         \
    } while (0)

#define FAIL(name, msg)                                         \
    do {                                                        \
        std::cout << " -> [FAIL] " << msg << std::endl;         \
        tests_failed++;                                         \
    } while (0)

#define ASSERT_EQ(a, b, name)                                   \
    do {                                                        \
        if ((a) != (b)) {                                       \
            FAIL(name, "Expected " << (a) << " == " << (b));    \
            return;                                             \
        }                                                       \
    } while (0)

#define ASSERT_TRUE(cond, name, msg)                            \
    do {                                                        \
        if (!(cond)) {                                          \
            FAIL(name, msg);                                    \
            return;                                             \
        }                                                       \
    } while (0)

// ===================== 测试用例 =====================

void test_empty_tree() {
    TEST("empty tree");
    avl_tree<int> tree;
    ASSERT_TRUE(tree.empty(), "empty tree", "new tree should be empty");
    ASSERT_EQ(tree.size(), 0, "empty tree size");
    PASS("empty tree");
}

void test_insert_single() {
    TEST("insert single element");
    avl_tree<int> tree;
    tree.insert(42);
    ASSERT_TRUE(!tree.empty(), "insert single", "tree should not be empty after insert");
    ASSERT_EQ(tree.size(), 1, "insert single size");
    ASSERT_TRUE(tree.contain(42), "insert single", "tree should contain inserted element");
    PASS("insert single element");
}

void test_insert_multiple() {
    TEST("insert multiple elements");
    avl_tree<int> tree;
    std::vector<int> vals = {10, 20, 30, 5, 15, 2, 25};
    for (int v : vals) {
        tree.insert(v);
    }
    ASSERT_EQ(tree.size(), static_cast<int>(vals.size()), "insert multiple size");
    for (int v : vals) {
        ASSERT_TRUE(tree.contain(v), "insert multiple", "tree should contain " + std::to_string(v));
    }
    PASS("insert multiple elements");
}

void test_insert_duplicate() {
    TEST("insert duplicate element");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(10);
    ASSERT_EQ(tree.size(), 1, "insert duplicate size");
    PASS("insert duplicate element");
}

void test_contain_existing() {
    TEST("contain - existing element");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    ASSERT_TRUE(tree.contain(10), "contain existing", "should contain 10");
    ASSERT_TRUE(tree.contain(20), "contain existing", "should contain 20");
    PASS("contain - existing element");
}

void test_contain_nonexisting() {
    TEST("contain - non-existing element");
    avl_tree<int> tree;
    tree.insert(10);
    ASSERT_TRUE(!tree.contain(99), "contain non-existing", "should not contain 99");
    PASS("contain - non-existing element");
}

void test_remove_leaf() {
    TEST("remove leaf node");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(5);
    ASSERT_EQ(tree.size(), 2, "remove leaf size");
    ASSERT_TRUE(!tree.contain(5), "remove leaf", "should not contain removed element");
    ASSERT_TRUE(tree.contain(10), "remove leaf", "should still contain 10");
    ASSERT_TRUE(tree.contain(15), "remove leaf", "should still contain 15");
    PASS("remove leaf node");
}

void test_remove_root() {
    TEST("remove root node");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(10);
    ASSERT_EQ(tree.size(), 2, "remove root size");
    ASSERT_TRUE(!tree.contain(10), "remove root", "should not contain removed root");
    PASS("remove root node");
}

void test_remove_nonexisting() {
    TEST("remove non-existing element");
    avl_tree<int> tree;
    tree.insert(10);
    tree.remove(99);
    ASSERT_EQ(tree.size(), 1, "remove non-existing size");
    PASS("remove non-existing element");
}

void test_remove_all() {
    TEST("remove all elements");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(10);
    tree.remove(5);
    tree.remove(15);
    ASSERT_TRUE(tree.empty(), "remove all", "tree should be empty after removing all");
    ASSERT_EQ(tree.size(), 0, "remove all size");
    PASS("remove all elements");
}

void test_copy_constructor() {
    TEST("copy constructor");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);

    avl_tree<int> copy(tree);
    ASSERT_EQ(copy.size(), tree.size(), "copy constructor size");
    ASSERT_TRUE(copy.contain(10), "copy constructor", "copy should contain 10");
    ASSERT_TRUE(copy.contain(20), "copy constructor", "copy should contain 20");
    ASSERT_TRUE(copy.contain(5), "copy constructor", "copy should contain 5");

    // 修改原树不影响拷贝
    tree.remove(10);
    ASSERT_TRUE(copy.contain(10), "copy constructor", "copy should still contain 10 after original modified");
    PASS("copy constructor");
}

void test_move_constructor() {
    TEST("move constructor");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> moved(std::move(tree));
    ASSERT_EQ(moved.size(), 2, "move constructor size");
    ASSERT_TRUE(moved.contain(10), "move constructor", "moved tree should contain 10");
    ASSERT_TRUE(moved.contain(20), "move constructor", "moved tree should contain 20");
    ASSERT_TRUE(tree.empty(), "move constructor", "original tree should be empty after move");
    PASS("move constructor");
}

void test_copy_assignment() {
    TEST("copy assignment");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> assigned;
    assigned.insert(99);
    assigned = tree;

    ASSERT_EQ(assigned.size(), tree.size(), "copy assignment size");
    ASSERT_TRUE(assigned.contain(10), "copy assignment", "should contain 10");
    ASSERT_TRUE(!assigned.contain(99), "copy assignment", "should not contain old value 99");
    PASS("copy assignment");
}

void test_move_assignment() {
    TEST("move assignment");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> assigned;
    assigned.insert(99);
    assigned = std::move(tree);

    ASSERT_EQ(assigned.size(), 2, "move assignment size");
    ASSERT_TRUE(assigned.contain(10), "move assignment", "should contain 10");
    ASSERT_TRUE(!assigned.contain(99), "move assignment", "should not contain old value 99");
    PASS("move assignment");
}

void test_self_assignment() {
    TEST("self assignment");
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree = tree; // NOLINT：测试自赋值安全性
    ASSERT_EQ(tree.size(), 2, "self assignment size");
    ASSERT_TRUE(tree.contain(10), "self assignment", "should still contain 10");
    PASS("self assignment");
}

void test_avl_balance_after_inserts() {
    TEST("AVL balance after sequential inserts (RR rotation)");
    avl_tree<int> tree;
    // 顺序插入 1,2,3,4,5,6,7 应触发多次旋转
    for (int i = 1; i <= 7; ++i) {
        tree.insert(i);
    }
    ASSERT_EQ(tree.size(), 7, "AVL balance sequential size");
    for (int i = 1; i <= 7; ++i) {
        ASSERT_TRUE(tree.contain(i), "AVL balance sequential", "should contain " + std::to_string(i));
    }
    PASS("AVL balance after sequential inserts (RR rotation)");
}

void test_avl_balance_after_deletes() {
    TEST("AVL balance after deletes");
    avl_tree<int> tree;
    for (int i = 1; i <= 7; ++i) {
        tree.insert(i);
    }
    // 删除多个节点后验证剩余元素仍存在
    tree.remove(4);
    tree.remove(2);
    tree.remove(6);
    ASSERT_EQ(tree.size(), 4, "AVL balance after deletes size");
    ASSERT_TRUE(!tree.contain(4), "AVL balance delete", "should not contain 4");
    ASSERT_TRUE(tree.contain(1), "AVL balance delete", "should contain 1");
    ASSERT_TRUE(tree.contain(3), "AVL balance delete", "should contain 3");
    ASSERT_TRUE(tree.contain(5), "AVL balance delete", "should contain 5");
    ASSERT_TRUE(tree.contain(7), "AVL balance delete", "should contain 7");
    PASS("AVL balance after deletes");
}

void test_custom_comparator() {
    TEST("custom comparator (descending)");
    // 降序比较器
    struct Descending {
        bool operator()(int a, int b) const { return a > b; }
    };
    avl_tree<int, Descending> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    ASSERT_EQ(tree.size(), 3, "custom comparator size");
    ASSERT_TRUE(tree.contain(10), "custom comparator", "should contain 10");
    ASSERT_TRUE(tree.contain(5), "custom comparator", "should contain 5");
    ASSERT_TRUE(tree.contain(20), "custom comparator", "should contain 20");
    PASS("custom comparator (descending)");
}

void test_string_avl() {
    TEST("AVL tree with std::string");
    avl_tree<std::string> tree;
    tree.insert("hello");
    tree.insert("world");
    tree.insert("avl");
    ASSERT_EQ(tree.size(), 3, "string AVL size");
    ASSERT_TRUE(tree.contain("hello"), "string AVL", "should contain 'hello'");
    ASSERT_TRUE(tree.contain("world"), "string AVL", "should contain 'world'");
    tree.remove("avl");
    ASSERT_TRUE(!tree.contain("avl"), "string AVL", "should not contain removed 'avl'");
    PASS("AVL tree with std::string");
}

void test_large_insert_remove() {
    TEST("large scale insert and remove");
    avl_tree<int> tree;
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        tree.insert(i);
    }
    ASSERT_EQ(tree.size(), N, "large insert size");
    for (int i = 0; i < N; ++i) {
        ASSERT_TRUE(tree.contain(i), "large insert", "should contain " + std::to_string(i));
    }
    // 删除偶数
    for (int i = 0; i < N; i += 2) {
        tree.remove(i);
    }
    ASSERT_EQ(tree.size(), N / 2, "large remove size");
    for (int i = 0; i < N; i += 2) {
        ASSERT_TRUE(!tree.contain(i), "large remove", "should not contain removed " + std::to_string(i));
    }
    for (int i = 1; i < N; i += 2) {
        ASSERT_TRUE(tree.contain(i), "large remove", "should contain odd " + std::to_string(i));
    }
    PASS("large scale insert and remove");
}

// ===================== 主函数 =====================

int main() {
    std::cout << "==============================\n";
    std::cout << "  AVL Tree Unit Tests\n";
    std::cout << "==============================\n\n";

    test_empty_tree();
    test_insert_single();
    test_insert_multiple();
    test_insert_duplicate();
    test_contain_existing();
    test_contain_nonexisting();
    test_remove_leaf();
    test_remove_root();
    test_remove_nonexisting();
    test_remove_all();
    test_copy_constructor();
    test_move_constructor();
    test_copy_assignment();
    test_move_assignment();
    test_self_assignment();
    test_avl_balance_after_inserts();
    test_avl_balance_after_deletes();
    test_custom_comparator();
    test_string_avl();
    test_large_insert_remove();

    std::cout << "\n==============================\n";
    std::cout << "  Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n";
    std::cout << "==============================\n";

    return tests_failed > 0 ? 1 : 0;
}
