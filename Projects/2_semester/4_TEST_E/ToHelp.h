#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <string>
#include <math.h>



struct treeNode {
    std::string val;
    int prior, size;
    treeNode *left, *right;
    treeNode() {
        val = "";
        size = 1;
        left = right = nullptr;
        prior = rand();

    }
    treeNode(std::string s) {
        val = s;
        size = 1;
        prior = rand();
        left = right = nullptr;
    }
};

int get_size(treeNode* tree) {
    if (tree == nullptr) {
        return 0;
    }
    else {
        return tree->size;
    }
}

void upd(treeNode* &tree) {
    if (tree != nullptr) {
        tree->size = get_size(tree->right) + 1 + get_size(tree->left);
    }
}

void split(treeNode *tree, treeNode* &left, treeNode* &right, int k) {
    if (tree == nullptr) {
        left = right = nullptr;
    }
    else
    if (get_size(tree->left) < k) {
        left = tree;
        split(left->right, left->right, right, k - get_size(tree->left) - 1);
        upd(left);
        upd(right);
    }
    else {
        right = tree;
        split(right->left, left, right->left, k);
        upd(left);
        upd(right);
    }
}

void merge(treeNode* &tree, treeNode* left, treeNode* right) {
    if (left == nullptr) {
        tree = right;
        upd(tree);
    }
    else
    if (right == nullptr) {
        tree = left;
        upd(tree);
    }
    else {
        if (left->prior > right->prior) {
            tree = left;
            merge(tree->right, tree->right, right);
            upd(tree);
        }
        else {
            tree = right;
            merge(tree->left, left, tree->left);
            upd(tree);
        }
    }
}

std::string get(treeNode* tree, int k) {
    int size = get_size(tree->left);
    if (size == k) {
        return tree->val;
    }
    if (size >= k) {
        return get(tree->left, k);
    }
    return get(tree->right, k - size - 1);
}

void insert(treeNode* &tree, int k, std::string value) {
    treeNode *left = nullptr, *right = nullptr;
    split(tree, left, right, k);
    tree = new treeNode(value);
    merge(tree, tree, right);
    merge(tree, left, tree);
}

void delete_(treeNode* &tree, int k) {
    treeNode *left = nullptr, *middle = nullptr, *right = nullptr;
    split(tree, left, middle, k);
    split(middle, middle, right, 1);
    merge(tree, left, right);
}

int main() {
    int k, position;
    char sign;
    std::cin >> k;
    treeNode* a = nullptr;


    std::string s;

    for (int i = 0; i < k; ++i) {
        std::cin >> sign >> position;
        if (sign == '?') {
            std::cout << get(a, position) << std::endl;
        }
        else
        if (sign == '-') {
            delete_(a, position);
        }
        else {
            std::cin >> s;
            insert(a, position, s);
        }
    }

    system("pause");
}
