#ifndef DATA_STRUCTURES_TREE_HPP
#define DATA_STRUCTURES_TREE_HPP

template <class DataType, size_t BranchNumber>
struct Tree {
    Tree() = default;
    Tree(DataType data = {}) { this->data = data; }
    ~Tree() = default;

    DataType data {};
    std::array<std::unique_ptr<Tree>, BranchNumber> branches {};
};

#endif // DATA_STRUCTURES_TREE_HPP
