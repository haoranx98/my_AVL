#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

template<typename T, typename Compare = std::less<T>>
class avl_tree{

    private:
        struct Node{
            T data;
            Node* left;
            Node* right;
            int height;
        };

        Node* root;
        // 新增比较器对象
        Compare comp;

        int tree_size;

        void print_tree(Node* node, const std::string& prefix, bool is_left) {
            if (node == nullptr) {
                return;
            }

            std::cout << prefix;

            std::cout << (is_left ? "├── " : "└── ");
            std::cout << node->data << " (h=" << node->height << ")" << '\n';

            std::string next_prefix = prefix + (is_left ? "│   " : "    ");

            if (node->left != nullptr || node->right != nullptr) {
                print_tree(node->left, next_prefix, true);
                print_tree(node->right, next_prefix, false);
            }
        }

        Node* copy_tree(Node* node){
            if(node == nullptr){
                return nullptr;
            }

            Node* new_root = new Node;
            new_root -> data = node -> data;
            new_root -> height = node -> height;

            new_root -> left = copy_tree(node -> left);
            new_root -> right = copy_tree(node -> right);

            return new_root;
        }

    public:
        avl_tree(): root(nullptr), comp(Compare{}), tree_size(0) {}
        avl_tree(const avl_tree& other): root(nullptr), comp(other.comp), tree_size(other.tree_size){
            root = copy_tree(other.root);
        }

        avl_tree(avl_tree&& other) noexcept
            :root(other.root), comp(std::move(other.comp)), tree_size(other.tree_size){

                other.root = nullptr;
                other.tree_size = 0;

        }

        ~avl_tree(){
            destroy_tree(root);
        }

        avl_tree& operator=(const avl_tree& other){
            if(this != &other){
                destroy_tree(root);
                root = copy_tree(other.root);
                comp = other.comp;
                tree_size = other.tree_size;
            }

            return *this;
        }

        avl_tree& operator=(avl_tree&& other) noexcept {
            if(this != &other){
                destroy_tree(root);

                root = other.root;
                comp = std::move(other.comp);
                tree_size = other.tree_size;

                other.root = nullptr;
                other.tree_size = 0;
            }

            return *this;
        }



        void destroy_tree(Node* node){
            if(node != nullptr){
                destroy_tree(node -> left);
                destroy_tree(node -> right);
                delete node;
            }
        }

        void insert(T data){
            Node* node = new Node;
            
           if(!contain(data)){
                node->data = data;
                node->left = nullptr;
                node->right = nullptr;
                node->height = 1;

                root = insert(root, node);
                tree_size ++;
            }

        }

        Node* insert(Node* node, Node* new_node){ 
            if(node == nullptr){
                return new_node;
            }

            // if(new_node->data < node->data){
            // if(comp(new_node->data, node->data)){
            //     node->left = insert(node->left, new_node);
            // }else{
            //     node->right = insert(node->right, new_node);
            // }

            if(comp(new_node -> data, node -> data)){
                node -> left = insert(node -> left, new_node);
            }else if(comp(node -> data, new_node -> data)){
                node -> right = insert(node -> right, new_node);
            }else{
                return node;
            }

            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            return balance(node);
        }

        int get_height(Node* node){
            if(node == nullptr){
                return 0;
            }
            return node->height;
        }

        Node* balance(Node* node){ 
            int balance_factor = get_height(node->left) - get_height(node->right);
            if(balance_factor > 1){ 
                if(get_height(node->left->left) >= get_height(node->left->right)){
                    return rotate_right(node);
                }else{
                    node->left = rotate_left(node->left);
                    return rotate_right(node);
                }
            }else if(balance_factor < -1){ 
                if(get_height(node->right->right) >= get_height(node->right->left)){
                    return rotate_left(node);
                }else{
                    node->right = rotate_right(node->right);
                    return rotate_left(node);
                }
            }else{
                return node;
            }
        }

        // LL型，左孩子的左孩子导致不平衡，需要向右旋转
        Node* rotate_right(Node* node){
            Node* new_node = node->left;
            Node* temp_node = new_node->right;
            new_node->right = node;
            node->left = temp_node;

            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
            new_node -> height = std::max(get_height(new_node->left), get_height(new_node->right)) + 1;

            return new_node;
        }

        // RR 型，右孩子的右孩子导致不平衡，需要向左旋转
        Node* rotate_left(Node* node){
            Node* new_node = node->right;
            Node* temp_node = new_node->left;

            new_node->left = node;
            node->right = temp_node;

            node -> height = std::max(get_height(node->left), get_height(node->right)) + 1;
            new_node -> height = std::max(get_height(new_node->left), get_height(new_node->right)) + 1;

            return new_node;
        }

        Node* minValueNode(Node* node){
            Node* current = node;
            while (current->left != nullptr)
            {
                current = current->left;
            }

            return current;
        }

        int get_balance(Node *node)
        {
            return node ? get_height(node->left) - get_height(node->right) : 0;
        }

        void remove(T data){
            
            if(contain(data)){
                root = removeNode(root, data);
                tree_size --;
            }
        }
        Node* removeNode(Node* root, T key){

            // 删除节点
            if(root == nullptr){
                return root;
            }

            if(key < root -> data){
                root -> left = removeNode(root -> left, key);
            }else if(key > root -> data){
                root -> right = removeNode(root -> right, key);
            }else{
                if((root -> left == nullptr) || (root -> right == nullptr)){
                    Node* temp = root -> left? root -> left: root->right;

                    if(temp == nullptr){
                        temp = root;
                        root = nullptr;
                    }else{
                        *root = *temp;
                    }

                    delete temp;
                }else{
                    Node* temp = minValueNode(root -> right);

                    root -> data = temp -> data;

                    root -> right = removeNode(root -> right, temp -> data);
                }
            }

            if(root == nullptr){
                return root;
            }

            // 更新高度
            root -> height = std::max(get_height(root->left), get_height(root->right)) + 1;

            // 计算平衡因子
            int balance = get_balance(root);

            // 使不平衡的树平衡

            // LL, 需要右旋
            if(balance > 1 && get_balance(root -> left) >= 0){
                return rotate_right(root);
            }

            // RR, 需要左旋

            if(balance < -1 && get_balance(root -> right) <= 0){
                return rotate_left(root);
            }


            // LR, 需要先左旋再右旋
            if(balance > 1 && get_balance(root -> left) < 0){
                root -> left = rotate_left(root -> left);
                return rotate_right(root);
            }

            // RL, 需要先右旋，再左旋
            if (balance < -1 && get_balance(root -> right) < 0)
            {
                root -> right = rotate_right(root -> right);
                return rotate_left(root);
            }
        
            return root;

        }

        void inorder(){
            inorder(this -> root);
            std::cout << std::endl;
        }

        void inorder(Node* node){
            if(node == nullptr){
                return;
            }

            inorder(node->left);
            std::cout << node->data << '\t';
            inorder(node->right); 
        }

        Node* get_root(){
            return this -> root;
        }

        void print_tree() {
            print_tree(root, "", true);
        }

        bool contain(T data) const {
            
            return contain(root, data);
        }

        bool contain(Node* node, T data) const {

            bool result = false;
            
            if(node == nullptr){
                return result;
            }

            if(comp(node -> data, data)){
                result = contain(node -> right, data);
            }else if(comp(data, node -> data)){
                result =contain(node -> left, data);
            }else{
                result = true;
            }

            return result;
        }

        bool empty() const {
            bool result = false;

            if(root == nullptr){
                result = true;
            }

            return result;
        }

        int size() const {

            
            return tree_size;

        }


};