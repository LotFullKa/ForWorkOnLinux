#include <iostream>
#include <utility>
#include <memory>

using namespace std;

class CartesianTree {
public:

    CartesianTree() = default;

    void InsertAt( int position, const std::string& value ) {
        auto new_node = make_shared<Node>(value);

        auto split_node = split(root, position);

        auto right = merge(new_node, split_node.second);

        root = merge(split_node.first, right);
    }

    void DeleteAt( int position ) {

        auto split_node = split(root, position);
        auto new_node = split(split_node.second, 1);

        root = merge(split_node.first, new_node.second);
    }

    string GetAt( int position ) {
        auto node = root;
        int left_size = get_size(root->left_child);

        while (left_size != position) {

            if (position < left_size) {
                node = node->left_child;
            } else {
                position -= left_size + 1;
                node = node->right_child;
            }

            left_size = get_size(node->left_child);
        }

        return node->value;
    }

private:
    struct Node {

        explicit Node(string value) :
            value(std::move(value)),
            priority(rand()){}

        int size = 1;
        int priority = 0;

        string value;

        shared_ptr<Node> left_child = nullptr,
                         right_child = nullptr;
};

    shared_ptr<Node> root;

    int get_size(const shared_ptr<Node> &node) {
        return node ? node -> size : 0;
    }

    void update(const shared_ptr<Node> &node) {
        if (node) {
            node->size = 1 + get_size(node->left_child) + get_size(node->right_child);
        }
    }

    pair <shared_ptr<Node>, shared_ptr<Node>> split (const shared_ptr<Node> &node, int position) {
        if (!node) {
            return make_pair(nullptr, nullptr);
        }

        int left_size = get_size(node->left_child);
        if (left_size >= position) {
            auto split_left = split(node->left_child, position);

            node->left_child = split_left.second;
            update(node);

            return make_pair(split_left.first, node);
        } else {
            auto split_right = split(node->right_child, position - left_size - 1);

            node->right_child = split_right.first;
            update(node);

            return make_pair(node, split_right.second);
        }
    }

    shared_ptr<Node> merge (const shared_ptr<Node> &left, const shared_ptr<Node> &right) {
        if (!left)
            return right;
        if (!right)
            return left;

        if (left->priority > right->priority) {
            left->right_child = merge(left->right_child, right);
            update(left);

            return left;
        } else {
            right->left_child = merge(left, right->left_child);
            update(right);

            return right;
        }
    }
};

int main() {
    CartesianTree array;

    int k;
    cin >> k;

    for (int i = 0; i < k; ++i) {
        char oper;
        cin >> oper;

        int position = 0;
        cin >> position;

        if (oper == '+') {
            string val;
            cin >> val;

            array.InsertAt(position, val);

        } else if ( oper == '-') {
            int end_pos;
            cin >> end_pos;

            for(int pos = position; pos <= end_pos; ++pos) {
                array.DeleteAt(position);
            }
        } else if (oper == '?') {
            cout << array.GetAt(position) << endl;
        }
    }
    return 0;
}
