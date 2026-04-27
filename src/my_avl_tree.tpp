// my_avl_tree.tpp - 模板实现文件
// 此文件由 my_avl_tree.hpp 包含，不应单独编译

// ==================== private 方法 ====================

template <typename T, typename Compare>
void avl_tree<T, Compare>::print_tree(const std::unique_ptr<Node<T>> &node, const std::string &prefix, bool is_left)
{
    if (node == nullptr)
    {
        return;
    }

    std::cout << prefix;

    std::cout << (is_left ? "├── " : "└── ");
    std::cout << node->data << " (h=" << node->height << ")" << '\n';

    std::string next_prefix = prefix + (is_left ? "│   " : "    ");

    if (node->left != nullptr || node->right != nullptr)
    {
        print_tree(node->left, next_prefix, true);
        print_tree(node->right, next_prefix, false);
    }
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::copy_tree(std::unique_ptr<Node<T>> node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    // Node* new_root = new Node;
    std::unique_ptr<Node<T>> new_root = std::make_unique<Node<T>>();
    new_root->data = node->data;
    new_root->height = node->height;

    // new_root -> left = copy_tree(node -> left);
    // new_root -> right = copy_tree(node -> right);

    // 为什么这里用移动，转移所有权，unique_ptr具有唯一所有权，通过move可以使得函数的形参接管指针的所有权
    new_root->left = copy_tree(std::move(node->left));
    new_root->right = copy_tree(std::move(node->right));

    return new_root;
}

// ==================== 构造 / 析构 / 赋值 ====================

template <typename T, typename Compare>
avl_tree<T, Compare>::avl_tree() : root(nullptr), comp(Compare{}), tree_size(0) {}

template <typename T, typename Compare>
avl_tree<T, Compare>::avl_tree(const avl_tree &other) : root(nullptr), comp(other.comp), tree_size(other.tree_size)
{
    // root = copy_tree(other.root);
}

template <typename T, typename Compare>
avl_tree<T, Compare>::avl_tree(avl_tree &&other) noexcept
    : root(std::move(other.root)), comp(std::move(other.comp)), tree_size(other.tree_size)
{

    other.root = nullptr;
    other.tree_size = 0;
}

template <typename T, typename Compare>
avl_tree<T, Compare>::~avl_tree()
{
    // destroy_tree(root);
    root.reset();
}

// template<typename T, typename Compare>
// avl_tree<T, Compare>& avl_tree<T, Compare>::operator=(const avl_tree& other){
//     if(this != &other){
//         // destroy_tree(root);
//         root = copy_tree(other.root);
//         comp = other.comp;
//         tree_size = other.tree_size;
//     }

//     return *this;
// }

template <typename T, typename Compare>
avl_tree<T, Compare> &avl_tree<T, Compare>::operator=(const avl_tree &other)
{
    if (this != &other)
    {
        // 创建临时副本
        avl_tree temp(other); // 调用拷贝构造函数

        // 交换数据
        std::swap(root, temp.root);
        std::swap(comp, temp.comp);
        std::swap(tree_size, temp.tree_size);
    }

    return *this;
}

template <typename T, typename Compare>
avl_tree<T, Compare> &avl_tree<T, Compare>::operator=(avl_tree &&other) noexcept
{
    if (this != &other)
    {
        // destroy_tree(std::move(root));

        // root = other.root;
        root = std::move(other.root);
        comp = std::move(other.comp);
        tree_size = other.tree_size;

        other.root = nullptr;
        other.tree_size = 0;
    }

    return *this;
}

// ==================== 树操作 ====================

// template<typename T, typename Compare>
// void avl_tree<T, Compare>::destroy_tree(std::unique_ptr<Node<T>> node){
//     if(node != nullptr){
//         destroy_tree(std::move(node -> left));
//         destroy_tree(std::move(node -> right));
//         // delete node;
//         node.reset();
//     }
// }

template <typename T, typename Compare>
void avl_tree<T, Compare>::insert(T data)
{
    // Node* node = new Node;
    std::unique_ptr<Node<T>> node = std::make_unique<Node<T>>();

    if (!contain(data))
    {
        node->data = data;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;

        // move
        root = insert(std::move(root), std::move(node));
        tree_size++;
    }
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::insert(std::unique_ptr<Node<T>> node, std::unique_ptr<Node<T>> new_node)
{
    if (node == nullptr)
    {
        return new_node;
    }

    if (comp(new_node->data, node->data))
    {
        // node -> left = insert(node -> left, new_node);
        node->left = insert(std::move(node->left), std::move(new_node));
    }
    else if (comp(node->data, new_node->data))
    {
        // node -> right = insert(node -> right, new_node);
        node->right = insert(std::move(node->right), std::move(new_node));
    }
    else
    {
        return node;
    }

    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    // return balance(node);
    return balance(std::move(node));
}

template <typename T, typename Compare>
int avl_tree<T, Compare>::get_height(std::unique_ptr<Node<T>> &node) const
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::balance(std::unique_ptr<Node<T>> node)
{
    int balance_factor = get_height(node->left) - get_height(node->right);
    if (balance_factor > 1)
    {
        if (get_height(node->left->left) >= get_height(node->left->right))
        {
            return rotate_right(std::move(node));
        }
        else
        {
            node->left = rotate_left(std::move(node->left));
            return rotate_right(std::move(node));
        }
    }
    else if (balance_factor < -1)
    {
        if (get_height(node->right->right) >= get_height(node->right->left))
        {
            return rotate_left(std::move(node));
        }
        else
        {
            node->right = rotate_right(std::move(node->right));
            return rotate_left(std::move(node));
        }
    }
    else
    {
        return node;
    }
}

// LL型，左孩子的左孩子导致不平衡，需要向右旋转
template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::rotate_right(std::unique_ptr<Node<T>> node)
{

    // Node* new_node = node->left;
    std::unique_ptr<Node<T>> new_node = std::move(node->left);
    // Node* temp_node = new_node->right;
    std::unique_ptr<Node<T>> temp_node = std::move(new_node->right);
    // new_node->right = node;
    new_node->right = std::move(node);
    // node->left = temp_node;
    node->left = std::move(temp_node);

    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    new_node->height = std::max(get_height(new_node->left), get_height(new_node->right)) + 1;

    return new_node;
}

// RR 型，右孩子的右孩子导致不平衡，需要向左旋转
template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::rotate_left(std::unique_ptr<Node<T>> node)
{
    // Node* new_node = node->right;
    std::unique_ptr<Node<T>> new_node = std::move(node->right);
    // Node* temp_node = new_node->left;
    std::unique_ptr<Node<T>> temp_node = std::move(new_node->left);

    // new_node->left = node;
    new_node->left = std::move(node);
    // node->right = temp_node;
    node->right = std::move(temp_node);

    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
    new_node->height = std::max(get_height(new_node->left), get_height(new_node->right)) + 1;

    return new_node;
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::minValueNode(std::unique_ptr<Node<T>> node) const
{
    // Node* current = node;
    std::unique_ptr<Node<T>> current = std::move(node);
    while (current->left != nullptr)
    {
        // current = current->left;
        current = std::move(current->left);
    }

    return current;
}

template <typename T, typename Compare>
int avl_tree<T, Compare>::get_balance(std::unique_ptr<Node<T>> &node) const
{
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

template <typename T, typename Compare>
void avl_tree<T, Compare>::remove(T data)
{

    if (contain(data))
    {
        root = removeNode(std::move(root), data);
        tree_size--;
    }
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::removeNode(std::unique_ptr<Node<T>> root, T key)
{

    // 删除节点
    if (root == nullptr)
    {
        return root;
    }

    if (key < root->data)
    {
        root->left = removeNode(std::move(root->left), key);
    }
    else if (key > root->data)
    {
        root->right = removeNode(std::move(root->right), key);
    }
    else
    {
        // if((root -> left == nullptr) || (root -> right == nullptr)){
        //     // Node* temp = root -> left? root -> left: root->right;
        //     std::unique_ptr<Node<T>> temp = std::move(root -> left? root -> left: root->right);

        //     if(temp == nullptr){
        //         // temp = root;
        //         temp = std::move(root);
        //         root = nullptr;
        //     }else{
        //         *root = *temp;
        //     }

        //     // delete temp;
        //     temp.reset();
        // }else{
        //     // Node* temp = minValueNode(root -> right);
        //     std::unique_ptr<Node<T>> temp = minValueNode(std::move(root -> right));

        //     root -> data = temp -> data;

        //     // root -> right = removeNode(root -> right, temp -> data);
        //     root -> right = removeNode(std::move(root -> right), temp -> data);
        // }
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            std::unique_ptr<Node<T>> new_root = std::move(root->left ? root->left : root->right);
            root = std::move(new_root);
        }
        else
        {
            std::unique_ptr<Node<T>> temp = minValueNode(std::move(root->right));
            T successor_data = temp->data;

            root->data = successor_data;
            root->right = removeNode(std::move(root->right), successor_data);
        }
    }

    if (root == nullptr)
    {
        return root;
    }

    // 更新高度
    root->height = std::max(get_height(root->left), get_height(root->right)) + 1;

    // 计算平衡因子
    int balance = get_balance(root);

    // 使不平衡的树平衡

    // LL, 需要右旋
    if (balance > 1 && get_balance(root->left) >= 0)
    {
        return rotate_right(std::move(root));
    }

    // RR, 需要左旋

    if (balance < -1 && get_balance(root->right) <= 0)
    {
        return rotate_left(std::move(root));
    }

    // LR, 需要先左旋再右旋
    if (balance > 1 && get_balance(root->left) < 0)
    {
        root->left = rotate_left(std::move(root->left));
        return rotate_right(std::move(root));
    }

    // RL, 需要先右旋，再左旋
    if (balance < -1 && get_balance(root->right) < 0)
    {
        root->right = rotate_right(std::move(root->right));
        return rotate_left(std::move(root));
    }

    return root;
}

// ==================== 遍历 / 查询 ====================

template <typename T, typename Compare>
void avl_tree<T, Compare>::inorder()
{
    inorder(this->root);
    std::cout << std::endl;
}

template <typename T, typename Compare>
void avl_tree<T, Compare>::inorder(std::unique_ptr<Node<T>> &node) const
{
    if (node == nullptr)
    {
        return;
    }

    inorder(node->left);
    std::cout << node->data << '\t';
    inorder(node->right);
}

template <typename T, typename Compare>
std::unique_ptr<Node<T>> avl_tree<T, Compare>::get_root() const
{
    return this->root;
}

template <typename T, typename Compare>
void avl_tree<T, Compare>::print_tree()
{
    print_tree(root, "", true);
}

template <typename T, typename Compare>
bool avl_tree<T, Compare>::contain(T data) const
{
    return contain(root, data);
}

template <typename T, typename Compare>
bool avl_tree<T, Compare>::contain(const std::unique_ptr<Node<T>> &node, T data) const
{
    if (node == nullptr)
    {
        return false;
    }

    if (comp(node->data, data))
    {
        return contain(node->right, data);
    }
    else if (comp(data, node->data))
    {
        return contain(node->left, data);
    }
    else
    {
        return true;
    }
}

template <typename T, typename Compare>
bool avl_tree<T, Compare>::empty() const
{
    bool result = false;

    if (root == nullptr)
    {
        result = true;
    }

    return result;
}

template <typename T, typename Compare>
int avl_tree<T, Compare>::size() const
{

    return tree_size;
}
