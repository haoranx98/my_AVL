#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>

template<typename T>
class Node{


    public:
        T data;
        std::unique_ptr<Node<T>> left;
        std::unique_ptr<Node<T>> right;
        int height;


};

template<typename T, typename Compare = std::less<T>>
class avl_tree{

    private:


        // Node* root;
        std::unique_ptr<Node<T>> root;
        // 新增比较器对象
        Compare comp;

        int tree_size;

        // void print_tree(Node* node, const std::string& prefix, bool is_left);
        void print_tree(const std::unique_ptr<Node<T>>& node, const std::string& prefix, bool is_left);
        std::unique_ptr<Node<T>> copy_tree(std::unique_ptr<Node<T>> node);

    public:
        avl_tree();
        avl_tree(const avl_tree& other);
        avl_tree(avl_tree&& other) noexcept;
        ~avl_tree();

        avl_tree& operator=(const avl_tree& other);
        avl_tree& operator=(avl_tree&& other) noexcept;

        // void destroy_tree(Node* node);
        // void destroy_tree(std::unique_ptr<Node<T>> node);
        void insert(T data);
        // Node* insert(Node* node, Node* new_node);
        std::unique_ptr<Node<T>> insert(std::unique_ptr<Node<T>> node, std::unique_ptr<Node<T>> new_node);
        // int get_height(Node* node);
        int get_height(std::unique_ptr<Node<T>>& node) const;
        
        // Node* balance(Node* node);
        std::unique_ptr<Node<T>> balance(std::unique_ptr<Node<T>> node);

        // LL型，左孩子的左孩子导致不平衡，需要向右旋转
        // Node* rotate_right(Node* node);
        std::unique_ptr<Node<T>> rotate_right(std::unique_ptr<Node<T>> node);
        // RR 型，右孩子的右孩子导致不平衡，需要向左旋转
        // Node* rotate_left(Node* node);
        std::unique_ptr<Node<T>> rotate_left(std::unique_ptr<Node<T>> node);

        // Node* minValueNode(Node* node);
        std::unique_ptr<Node<T>> minValueNode(std::unique_ptr<Node<T>> node) const;
        // int get_balance(Node* node);
        int get_balance(std::unique_ptr<Node<T>>& node) const;

        void remove(T data);
        // Node* removeNode(Node* root, T key);
        std::unique_ptr<Node<T>> removeNode(std::unique_ptr<Node<T>> root, T key);

        void inorder();
        void inorder(std::unique_ptr<Node<T>>& node) const;

        // Node* get_root();
        std::unique_ptr<Node<T>> get_root() const;
        void print_tree();
        bool contain(T data) const;
        // bool contain(Node* node, T data) const;
        // bool contain(const std::unique_ptr<Node<T>>& node, T data) const;
        bool contain(const std::unique_ptr<Node<T>> &node, T data) const;
        bool empty() const;
        int size() const;

};

#include "my_avl_tree.tpp"
