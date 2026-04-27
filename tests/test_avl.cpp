#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "my_avl_tree.hpp"

TEST(EmptyTree, IsEmptyOnConstruction) {
    avl_tree<int> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(Insert, SingleElement) {
    avl_tree<int> tree;
    tree.insert(42);
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.contain(42));
}

TEST(Insert, MultipleElements) {
    avl_tree<int> tree;
    std::vector<int> vals = {10, 20, 30, 5, 15, 2, 25};
    for (int v : vals) {
        tree.insert(v);
    }
    EXPECT_EQ(tree.size(), static_cast<int>(vals.size()));
    for (int v : vals) {
        EXPECT_TRUE(tree.contain(v));
    }
}

TEST(Insert, DuplicateIgnored) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(10);
    EXPECT_EQ(tree.size(), 1);
}

TEST(Contain, ExistingElement) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    EXPECT_TRUE(tree.contain(10));
    EXPECT_TRUE(tree.contain(20));
}

TEST(Contain, NonExistingElement) {
    avl_tree<int> tree;
    tree.insert(10);
    EXPECT_FALSE(tree.contain(99));
}

TEST(Remove, LeafNode) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(5);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_FALSE(tree.contain(5));
    EXPECT_TRUE(tree.contain(10));
    EXPECT_TRUE(tree.contain(15));
}

TEST(Remove, RootNode) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(10);
    EXPECT_EQ(tree.size(), 2);
    EXPECT_FALSE(tree.contain(10));
}

TEST(Remove, NonExistingNoop) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.remove(99);
    EXPECT_EQ(tree.size(), 1);
}

TEST(Remove, AllElements) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.remove(10);
    tree.remove(5);
    tree.remove(15);
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(Copy, ConstructorDeepCopy) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);

    avl_tree<int> copy(tree);
    EXPECT_EQ(copy.size(), tree.size());
    EXPECT_TRUE(copy.contain(10));
    EXPECT_TRUE(copy.contain(20));
    EXPECT_TRUE(copy.contain(5));

    tree.remove(10);
    EXPECT_TRUE(copy.contain(10));
}

TEST(Move, ConstructorTransfersOwnership) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> moved(std::move(tree));
    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(moved.contain(10));
    EXPECT_TRUE(moved.contain(20));
    EXPECT_TRUE(tree.empty());
}

TEST(Copy, AssignmentDeepCopy) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> assigned;
    assigned.insert(99);
    assigned = tree;

    EXPECT_EQ(assigned.size(), tree.size());
    EXPECT_TRUE(assigned.contain(10));
    EXPECT_FALSE(assigned.contain(99));
}

TEST(Move, AssignmentTransfersOwnership) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);

    avl_tree<int> assigned;
    assigned.insert(99);
    assigned = std::move(tree);

    EXPECT_EQ(assigned.size(), 2);
    EXPECT_TRUE(assigned.contain(10));
    EXPECT_FALSE(assigned.contain(99));
}

TEST(Assignment, SelfAssignmentSafe) {
    avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree = tree;  // NOLINT
    EXPECT_EQ(tree.size(), 2);
    EXPECT_TRUE(tree.contain(10));
}

TEST(AVLBalance, SequentialInsertsTriggerRotations) {
    avl_tree<int> tree;
    for (int i = 1; i <= 7; ++i) {
        tree.insert(i);
    }
    EXPECT_EQ(tree.size(), 7);
    for (int i = 1; i <= 7; ++i) {
        EXPECT_TRUE(tree.contain(i));
    }
}

TEST(AVLBalance, DeletesMaintainBalance) {
    avl_tree<int> tree;
    for (int i = 1; i <= 7; ++i) {
        tree.insert(i);
    }
    tree.remove(4);
    tree.remove(2);
    tree.remove(6);
    EXPECT_EQ(tree.size(), 4);
    EXPECT_FALSE(tree.contain(4));
    EXPECT_TRUE(tree.contain(1));
    EXPECT_TRUE(tree.contain(3));
    EXPECT_TRUE(tree.contain(5));
    EXPECT_TRUE(tree.contain(7));
}

TEST(Template, CustomComparatorDescending) {
    struct Descending {
        bool operator()(int a, int b) const { return a > b; }
    };
    avl_tree<int, Descending> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contain(10));
    EXPECT_TRUE(tree.contain(5));
    EXPECT_TRUE(tree.contain(20));
}

TEST(Template, StringType) {
    avl_tree<std::string> tree;
    tree.insert("hello");
    tree.insert("world");
    tree.insert("avl");
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contain("hello"));
    EXPECT_TRUE(tree.contain("world"));
    tree.remove("avl");
    EXPECT_FALSE(tree.contain("avl"));
}

TEST(Stress, LargeScaleInsertAndRemove) {
    avl_tree<int> tree;
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        tree.insert(i);
    }
    EXPECT_EQ(tree.size(), N);
    for (int i = 0; i < N; ++i) {
        EXPECT_TRUE(tree.contain(i));
    }
    for (int i = 0; i < N; i += 2) {
        tree.remove(i);
    }
    EXPECT_EQ(tree.size(), N / 2);
    for (int i = 0; i < N; i += 2) {
        EXPECT_FALSE(tree.contain(i));
    }
    for (int i = 1; i < N; i += 2) {
        EXPECT_TRUE(tree.contain(i));
    }
}
