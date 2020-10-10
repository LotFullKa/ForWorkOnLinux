/**
 * Задание 4. Задача №5
 * Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу со следующими методами:
 *  // Добавление строки в позицию position.\\
 *  // Все последующие строки сдвигаются на одну позицию вперед.\\
 *  void InsertAt( int position, const std::string& value );\\
 *  // Удаление строки из позиции position.\\
 *  // Все последующие строки сдвигаются на одну позицию назад.\\
 *  void DeleteAt( int position );\\
 *  // Получение строки из позиции position.\\
 *  std::string GetAt( int position );
 *  Все методы должны работать за O(log n) в среднем, где n – текущее количество строк в массиве.
 */

#include <iostream>
#include <memory>
#include <utility>

class StringArray {
public:
    void InsertAt(int32_t position, const std::string& value);

    void DeleteAt(int32_t position);

    std::string GetAt(int32_t position);

    class Node {
    public:
        explicit Node(std::string setValue);

        size_t childSize = 1;
        int64_t priority = 0;

        std::string value;

        std::shared_ptr<Node> leftChild  = nullptr,
                rightChild = nullptr;
    };

private:
    std::shared_ptr<Node> root = nullptr;

    std::pair<std::shared_ptr<Node>, std::shared_ptr<Node> > split
            (const std::shared_ptr<Node> &node, int32_t position);

    std::shared_ptr<Node> merge(const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right);

    size_t getSize(const std::shared_ptr<Node> &node);
    void update(const std::shared_ptr<Node> &node);
};

StringArray::Node::Node(std::string setValue) :
        value(std::move(setValue)),
        priority(rand()) {}

size_t StringArray::getSize(const std::shared_ptr<Node> &node) {
    return node ? node -> childSize : 0;
}

void StringArray::update(const std::shared_ptr<Node> &node) {
    if (node) {
        node -> childSize = 1 + getSize(node -> leftChild) + getSize(node -> rightChild);
    }
}

void StringArray::InsertAt(int32_t position, const std::string &value) {
    auto newNode = std::make_shared<Node>(value);

    auto splitNode = split(root, position);

    auto rightChild = merge(newNode, splitNode.second);
    root = merge(splitNode.first, rightChild);
}

void StringArray::DeleteAt(int32_t position) {
    auto splitNode = split(root, position);
    auto newNode = split(splitNode.second, 1);

    root = merge(splitNode.first, newNode.second);
}

std::string StringArray::GetAt(int32_t position) {
    auto currentNode = root;
    size_t leftIndex = getSize(root -> leftChild);

    while (leftIndex != position) {
        if (position < leftIndex) {
            currentNode = currentNode -> leftChild;
        } else {
            position -= leftIndex + 1;
            currentNode = currentNode -> rightChild;
        }
        leftIndex = getSize(currentNode -> leftChild);
    }

    return currentNode -> value;
}

std::pair<std::shared_ptr<StringArray::Node>, std::shared_ptr<StringArray::Node>> StringArray::split
        (const std::shared_ptr<Node> &node, int32_t position) {
    if (!node) {
        return std::make_pair(nullptr, nullptr);
    }

    size_t leftSize = getSize(node -> leftChild);
    if (leftSize >= position) {
        auto splitLeft = split(node -> leftChild, position);

        node -> leftChild = splitLeft.second;
        update(node);

        return std::make_pair(splitLeft.first, node);
    } else {
        auto splitRight = split(node -> rightChild, position - leftSize - 1);

        node -> rightChild = splitRight.first;
        update(node);

        return std::make_pair(node, splitRight.second);
    }
}

std::shared_ptr<StringArray::Node> StringArray::merge
        (const std::shared_ptr<Node> &left, const std::shared_ptr<Node> &right) {
    if (!left)  return right;
    if (!right) return left;

    if (left -> priority > right -> priority) {
        left -> rightChild = merge(left -> rightChild, right);
        update(left);

        return left;
    } else {
        right -> leftChild = merge(left, right -> leftChild);
        update(right);

        return right;
    }
}

namespace {
    void calculate(std::istream& in, std::ostream& out) {
        StringArray array;

        size_t k = 0;
        in >> k;

        for (size_t i = 0; i < k; ++i) {
            char buffer;
            in >> buffer;

            int32_t position = 0;
            in >> position;

            if (buffer == '+') {
                std::string value;
                in >> value;

                array.InsertAt(position, value);
            } else if (buffer == '-') {
                size_t endPosition = 0;
                in >> endPosition;

                for (size_t deletePosition = position; deletePosition <= endPosition; ++deletePosition) {
                    array.DeleteAt(deletePosition);
                }
            } else if (buffer == '?'){
                out << array.GetAt(position) << std::endl;
            }
        }
    }
}

int main() {
    srand(time(nullptr));
    calculate(std::cin, std::cout);

    return 0;
}