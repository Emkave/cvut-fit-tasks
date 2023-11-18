#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

#endif


template <typename Product>
struct Bestsellers {
    Bestsellers()=default;
    struct Node {
        std::vector<Product> shipment_list = {};

        size_t key = 0;

        size_t height = 1;
        size_t total_subtree_sum = 0;
        size_t total_subtree_size = 0;

        Node * left = nullptr;
        Node * right = nullptr;
        Node * parent = nullptr;

        explicit Node (size_t key) : key(key) {}
    };

    Node * AVL_TREE_ROOT = nullptr;

    std::unordered_map<Product, Node *> product_map = {};

    ///█████████████████████████████████████████████████████████████████████████████████████████████ ///
    ///████████████████████████<--LITTLE FUNCTIONS START-->█████████████████████████████████████████///
    inline Node * allocate_new_product_node(size_t key) noexcept {
        return new Node(key);                                          // Create new
    }

    inline long long int get_balance_coefficient(Node * node) const  { // Return balance coefficient
        return (node == nullptr) ? 0 : (((node->right == nullptr) ? 0 : node->right->height) - ((node->left == nullptr) ? 0 : node->left->height));
    }

    inline size_t get_max(const size_t a, const size_t b) const  {
        return (a > b) ? a : b; // return max
    }

    inline size_t get_height(Node * node) const  {
        return (node == nullptr) ? 0 : node->height; // Return height
    }

    inline size_t set_height(Node * node) const  {
        return 1 + get_max(get_height(node->left), get_height(node->right));
    }

    ///==========================///

    inline Node * get_minimum_value(Node * node) const  {
        Node * current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    inline Node * get_predecessor_replacement(Node * node)  { // Find min product node
        if (node->right != nullptr) {
            return get_minimum_value(node->right);
        } else {
            Node * parent = node->parent;
            while (parent != nullptr && node == parent->right) {
                node = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }
    ///==========================///

    inline void release_tree_node(Node * node) {
        if (node == nullptr) {
            return;
        }
        if (node->left != nullptr)
            release_tree_node(node->left);
        if (node->right != nullptr)
            release_tree_node(node->right);
        delete node;
    }

    ~Bestsellers() {
        release_tree_node(this->AVL_TREE_ROOT);
    }

    inline void update_total_subtree_sum(Node * node) {
        node->total_subtree_sum = node->shipment_list.size() * node->key;
        if (node->left != nullptr)
            node->total_subtree_sum += node->left->total_subtree_sum;
        if (node->right != nullptr)
            node->total_subtree_sum += node->right->total_subtree_sum;
    }

    inline void update_total_subtree_size(Node * node) {
        node->total_subtree_size = node->shipment_list.size();
        if (node->left != nullptr)
            node->total_subtree_size += node->left->total_subtree_size;
        if (node->right != nullptr)
            node->total_subtree_size += node->right->total_subtree_size;
    }

    inline void update_tree_data(Node * node) {
        if (node != nullptr) {
            update_total_subtree_sum(node);
            update_total_subtree_size(node);
            update_tree_data(node->parent);
        }
    }

    inline void update_tree_data_via_key(Node * node, size_t ending_node_key)  {
        if (node->key < ending_node_key)
            update_tree_data_via_key(node->right, ending_node_key);
        if (node->key > ending_node_key)
            update_tree_data_via_key(node->left, ending_node_key);

        node->total_subtree_sum = node->key * node->shipment_list.size();
        node->total_subtree_size = node->shipment_list.size();
        if (node->left != nullptr) {
            node->total_subtree_sum += node->left->total_subtree_sum;
            node->total_subtree_size += node->left->total_subtree_size;
        }
        if (node->right != nullptr) {
            node->total_subtree_sum += node->right->total_subtree_sum;
            node->total_subtree_size += node->right->total_subtree_size;
        }
    }

    inline size_t get_maximum_node_rank(Node * node) const {
        size_t to_return = node->shipment_list.size();
        if (node->right != nullptr)
            to_return += node->right->total_subtree_size;
        return to_return;
    }

    inline size_t get_minimum_node_rank(Node * node) const {
        size_t to_return = 1;
        if (node->right != nullptr)
            to_return += node->right->total_subtree_size;
        return to_return;
    }

    ///████████████████████████<--LITTLE FUNCTIONS END-->████████████████████████████████████ ///
    ///██████████████████████████████████████████████████████████████████████████████████████ ///
    ///████████████████████████<--MAIN FUNCTIONS START-->████████████████████████████████████///

    inline size_t get_product_index_from_map(Node * node, const Product & product) const {
        for (size_t i=0; i<node->shipment_list.size(); i++)
            if (node->shipment_list[i] == product)
                return i;
        return 0;
    }

    inline Product & get_product_via_rank(Node * node, size_t rank) const {
        if (rank < get_minimum_node_rank(node))
            return get_product_via_rank(node->right, rank);
        else if (rank > get_maximum_node_rank(node))
            return get_product_via_rank(node->left, rank - get_maximum_node_rank(node));
        else return node->shipment_list[rank - get_minimum_node_rank(node)];
    }

    inline Node * execute_right_rotate(Node * n) {
        Node * x = n->left;
        Node * y = n;

        x->parent = y->parent;

        if (x->parent != nullptr) {
            if (y->parent->left == y) {
                x->parent->left = x;
            } else {
                x->parent->right = x;
            }
        } y->left = x->right;//connect subtree B in slides from x to y

        if (y->left != nullptr) {   //check if we are rotating at the root of the tree, && change the root pointer as well
            y->left->parent = y;
        }

        x->right = y;       //make y child of x like in the slides
        y->parent = x;

        y->height = set_height(y);
        x->height = set_height(x);
        y->total_subtree_size = y->shipment_list.size() + ((y->left == nullptr) ? 0 : y->left->total_subtree_size) + ((y->right == nullptr) ? 0 : y->right->total_subtree_size);
        y->total_subtree_sum = y->key * y->shipment_list.size() + ((y->left == nullptr) ? 0 : y->left->total_subtree_sum) + ((y->right == nullptr) ? 0 : y->right->total_subtree_sum);
        x->total_subtree_size = x->shipment_list.size() + ((x->left == nullptr) ? 0 : x->left->total_subtree_size) + ((x->right == nullptr) ? 0 : x->right->total_subtree_size);
        x->total_subtree_sum = x->key * x->shipment_list.size() + ((x->left == nullptr) ? 0 : x->left->total_subtree_sum) + ((x->right == nullptr) ? 0 : x->right->total_subtree_sum);

        if (this->AVL_TREE_ROOT == n)
            this->AVL_TREE_ROOT = x;

        return x;   //return new subtree root
    }

    inline Node * execute_left_rotate(Node * n) {
        Node * x = n;
        Node * y = n->right;

        y->parent = x->parent;
        if (y->parent != nullptr) {
            if (x->parent->left == x) {
                y->parent->left = y;
            } else {
                y->parent->right = y;
            }
        } x->right = y->left;                  //connect subtree B in slides from y to x
        if (x->right != nullptr)
            x->right->parent = x;

        y->left = x;                //make x child of y like in the slides
        x->parent = y;

        x->height = set_height(x);
        y->height = set_height(y);
        x->total_subtree_size = x->shipment_list.size() + ((x->left == nullptr) ? 0 : x->left->total_subtree_size) + ((x->right == nullptr) ? 0 : x->right->total_subtree_size);
        x->total_subtree_sum = x->key * x->shipment_list.size() + ((x->left == nullptr) ? 0 : x->left->total_subtree_sum) + ((x->right == nullptr) ? 0 : x->right->total_subtree_sum);
        y->total_subtree_size = y->shipment_list.size() + ((y->left == nullptr) ? 0 : y->left->total_subtree_size) + ((y->right == nullptr) ? 0 : y->right->total_subtree_size);
        y->total_subtree_sum = y->key * y->shipment_list.size() + ((y->left == nullptr) ? 0 : y->left->total_subtree_sum) + ((y->right == nullptr) ? 0 : y->right->total_subtree_sum);

        if (n == this->AVL_TREE_ROOT)
            this->AVL_TREE_ROOT = y;

        return y;                   //return new subtree root
    }
    ///==========================///

    // Completed
    inline Node * execute_node_balance_check(Node * node)  {
        if (get_balance_coefficient(node) > 1) {
            if (get_balance_coefficient(node->right) != -1) {
                return execute_left_rotate(node);
            } else {
                node->right = execute_right_rotate(node->right);
                return execute_left_rotate(node);
            }
        } else if (get_balance_coefficient(node) < -1) {
            if (get_balance_coefficient(node->left) != 1) {
                return execute_right_rotate(node);
            } else {
                node->left = execute_left_rotate(node->left);
                return execute_right_rotate(node);
            }
        }
        return nullptr;
    }

    // Completed
    inline void execute_tree_balance_check(Node * node) {
        if (node == nullptr) {
            return;
        } else {
            node->height = set_height(node);
            if (get_balance_coefficient(node) < -1 || get_balance_coefficient(node) > 1)
                node = execute_node_balance_check(node);

            if (node == nullptr) return;

            execute_tree_balance_check(node->parent);
        }
    }

    inline void execute_tree_balancer(Node * node, Node * prev = nullptr)  {
        if (node == nullptr) return;
        if (node->left != nullptr) execute_tree_balancer(node->left, node);
        if (node->right != nullptr) execute_tree_balancer(node->right, node);

        node->height = set_height(node);                                             // Update height
        long long int balance = get_balance_coefficient(node);                         // Get balance coefficient

        if (balance > 1 && get_balance_coefficient(node->right) >= 0) {
            Node * oldNode = node;
            node = execute_left_rotate(node);
            //check if previously visited node (also the parent) is not a nullptr
            if (prev != nullptr) {
                //check if old node was left || right child of parent, && rewire parent left || right children to new node
                if (prev->left == oldNode) {
                    prev->left = node;
                } else {
                    prev->right = node;
                }
            }
        } else if (balance > 1 && get_balance_coefficient(node->right) < 0) {
            Node * oldNode = node;
            node->right = execute_right_rotate(node->right);
            node = execute_left_rotate(node);

            //check if previously visited node (also the parent) is not a nullptr
            if (prev != nullptr) {
                //check if old node was left || right child of parent, && rewire parent left || right children to new node
                if (prev->left == oldNode) {
                    prev->left = node;
                } else {
                    prev->right = node;
                }
            }
        } else if (balance < -1 && get_balance_coefficient(node->left) <= 0) {
            Node * oldNode = node;
            node = execute_right_rotate(node);

            //check if previously visited node (also the parent) is not a nullptr
            if (prev != nullptr) {
                //check if old node was left || right child of parent, && rewire parent left || right children to new node
                if (prev->left == oldNode) {
                    prev->left = node;
                } else {
                    prev->right = node;
                }
            }
        } else if (balance < -1 && get_balance_coefficient(node->left) > 0) {
            Node * oldNode = node;
            node->left = execute_left_rotate(node->left);
            node = execute_right_rotate(node);

            //check if previously visited node (also the parent) is not a nullptr
            if (prev) {
                //check if old node was left || right child of parent, && rewire parent left || right children to new node
                if (prev->left == oldNode) {
                    prev->left = node;
                } else {
                    prev->right = node;
                }
            }
        }

        node->total_subtree_sum = node->key * node->shipment_list.size();
        if (node->left != nullptr)
            node->total_subtree_sum += node->left->total_subtree_sum;
        if (node->right != nullptr)
            node->total_subtree_sum += node->right->total_subtree_sum;
    }
    ///==========================///

    inline void delete_product_from_tree(Node * node)  {
        if (node->left != nullptr && node->right == nullptr) { //only left child
            node->left->parent = node->parent;

            if (node->parent != nullptr) {
                if (node->parent->left == node) {
                    node->parent->left = node->left;
                } else {
                    node->parent->right = node->left;
                }
            } else {
                this->AVL_TREE_ROOT = node->left;
            }

            if (node->left != nullptr) {
                update_total_subtree_sum(node->left);
                update_total_subtree_size(node->left);
                update_tree_data(node->left->parent);
            }

            execute_tree_balance_check(node->left);
            delete node;

        } else if (node->left == nullptr && node->right != nullptr) { //only right child
            node->right->parent = node->parent;

            if (node->parent != nullptr) {
                if (node->parent->left == node) {
                    node->parent->left = node->right;
                } else {
                    node->parent->right = node->right;
                }
            } else {
                this->AVL_TREE_ROOT = node->right;
            }

            if (node->right != nullptr) {
                update_total_subtree_sum(node->right);
                update_total_subtree_size(node->right);
                update_tree_data(node->right->parent);
            }

            execute_tree_balance_check(node->right);
            delete node;

        } else if (node->left != nullptr && node->right != nullptr) { //both children
            Node * replacement = get_predecessor_replacement(node);
            size_t temp_key = replacement->key;
            node->shipment_list = replacement->shipment_list;
            node->key = temp_key;

            for (Product & i : node->shipment_list) {
                product_map[i] = node;
            }

            delete_product_from_tree(replacement);
        } else if (node->left == nullptr && node->right == nullptr) { //no children
            if (this->AVL_TREE_ROOT == node) {
                this->AVL_TREE_ROOT = nullptr;
                delete node;
                return;
            } else {
                if (node->parent->left == node) {
                    node->parent->left = nullptr;
                } else {
                    node->parent->right = nullptr;
                }

                if (node->parent != nullptr) {
                    update_total_subtree_sum(node->parent);
                    update_total_subtree_size(node->parent);
                    update_tree_data(node->parent->parent);
                }
                execute_tree_balance_check(node->parent);
                delete node;
            }
        }
    }

    inline void find_and_delete_node(Node * node, Node * prev, size_t key) {
        if (key == node->key) {
            delete_product_from_tree(node, prev);
        } else if (key < node->key) {
            find_and_delete_node(node->left, node, key);
        } else {
            find_and_delete_node(node->right, node, key);
        }
    }

    inline size_t return_largest_product_rank_number(Node * node, size_t key, size_t rank) const {
        if (node == nullptr) {
            return 0;
        }
        if (key < node->key)
            return return_largest_product_rank_number(node->left, key, rank);
        else if (key > node->key) {
            if (node->left != nullptr)
                rank -= node->left->total_subtree_size;
            return return_largest_product_rank_number(node->right, key, rank - node->shipment_list.size());
        } else {
            if (node->left != nullptr)
                rank -= node->left->total_subtree_size;
            return rank;
        }
    }

    inline Node * return_largest_product_rank_node(Node * node, size_t key, size_t rank) const {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->key)
            return return_largest_product_rank_node(node->left, key, rank);
        else if (key > node->key) {
            if (node->left != nullptr)
                rank -= node->left->total_subtree_size;
            return return_largest_product_rank_node(node->right, key, rank - node->shipment_list.size());
        } else {
            if (node->left != nullptr)
                rank -= node->left->total_subtree_size;
            return node;
        }
    }

    inline size_t return_lower_ground_product_key(Node * node, size_t key, size_t value) const {
        if (node->key > key)
            return return_lower_ground_product_key(node->left, key, value - (node->total_subtree_sum - node->left->total_subtree_sum));
        else if (node->key < key)
            return return_lower_ground_product_key(node->right, key, value);
        else if (node->right != nullptr)
            return value - node->right->total_subtree_sum;
        else return value;
    }

    inline size_t return_upper_ground_product_key(Node * node, size_t key, size_t value) const {
        if (node->key > key)
            return return_upper_ground_product_key(node->left, key, value);
        else if (node->key < key)
            return return_upper_ground_product_key(node->right, key, value - (node->total_subtree_sum - node->right->total_subtree_sum));
        else if (node->left != nullptr)
            return value - node->left->total_subtree_sum;
        else return value;
    }

    inline Node * get_product_data_node(Node * node, size_t key, size_t rank) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key)
            return get_product_data_node(node->left, key, rank);
        else if (key > node->key) {
            if (node->left)
                rank -= node->left->total_subtree_size;
            return get_product_data_node(node->right, key, rank - node->shipment_list.size());
        } else {
            if (node->left != nullptr)
                rank -= node->left->total_subtree_size;
            return node;
        }
    }

    ///==========================///

    inline void insert_to_avl(Node * node, Node * prev, const Product & product, size_t key) {
        if (node == nullptr) {                        // If node is empty
            Node * new_node = allocate_new_product_node(key);
            new_node->shipment_list.emplace_back(product);
            this->product_map[product] = new_node;
            new_node->parent = prev;

            if (prev != nullptr) {
                if (key < prev->key)
                    prev->left = new_node;
                else
                    prev->right = new_node;
            }

            if (this->AVL_TREE_ROOT == nullptr) {
                this->AVL_TREE_ROOT = new_node;
            }

            execute_tree_balance_check(new_node);
            update_tree_data(new_node);
            return;
        }

        if (key < node->key) {                                               // Check if left is not null
            insert_to_avl(node->left, node, product, key);         // Re-course to left
        } else if (key > node->key) {                                        // Check if right is not null
            insert_to_avl(node->right, node, product, key);        // Re-course to right
        } else {                                       // Check if key is same
            node->shipment_list.emplace_back(product);
            this->product_map[product] = node;
            if (node != nullptr) {
                update_total_subtree_sum(node);
                update_total_subtree_size(node);
                update_tree_data(node->parent);
            }
        }
    }
    ///████████████████████████<--MAIN FUNCTIONS END-->█████████████████████████████████████████ ///
    ///█████████████████████████████████████████████████████████████████████████████████████████ ///
    ///████████████████████████<--PROGTEST FUNCTIONS START-->███████████████████████████████████///
    // The total number of tracked products
    size_t products() const {
        return this->product_map.size();
    }

    void sell(const Product & p, size_t amount) {
        if (this->AVL_TREE_ROOT == nullptr) {
            this->AVL_TREE_ROOT = allocate_new_product_node(amount);
            this->AVL_TREE_ROOT->total_subtree_sum = amount;
            this->AVL_TREE_ROOT->shipment_list.emplace_back(p);
            this->AVL_TREE_ROOT->total_subtree_size = 1;
            this->product_map[p] = this->AVL_TREE_ROOT;
        } else {
            typename std::unordered_map<Product, Node *>::const_iterator product_map_iter = this->product_map.find(p);
            if (product_map_iter != this->product_map.end()) {
                if (amount != 0) {
                    Node * old_node = product_map_iter->second;
                    size_t old_key = old_node->key;

                    //typename std::vector<Product>::iterator shipment_list_iter = std::find(product_map_iter->second->shipment_list.begin(), product_map_iter->second->shipment_list.end(), p);

                    product_map_iter->second->shipment_list.erase(std::find(product_map_iter->second->shipment_list.begin(), product_map_iter->second->shipment_list.end(), p));
                    if (product_map_iter->second->shipment_list.empty()) {
                        delete_product_from_tree(product_map_iter->second);
                    } else {
                        if (product_map_iter->second != nullptr) {
                            update_total_subtree_sum(product_map_iter->second);
                            update_total_subtree_size(product_map_iter->second);
                            update_tree_data(product_map_iter->second->parent);
                        }
                    }
                    insert_to_avl(this->AVL_TREE_ROOT, nullptr, p, old_key + amount);
                }
            } else {
                insert_to_avl(this->AVL_TREE_ROOT, nullptr, p, amount);
            }
        }
    }

     size_t rank(const Product & p) const {
        typename std::unordered_map<Product, Node *>::const_iterator product_map_iter = this->product_map.find(p);
        if (product_map_iter == this->product_map.end()) throw std::out_of_range("");

        size_t largest_product_rank_number = return_largest_product_rank_number(this->AVL_TREE_ROOT, product_map_iter->second->key, this->AVL_TREE_ROOT->total_subtree_size);
        return largest_product_rank_number - (product_map_iter->second->shipment_list.size() - get_product_index_from_map(product_map_iter->second, p)) + 1;
    }

     const Product & product(size_t rank) const {
        if (this->product_map.size() < rank || 1 > rank)
            throw std::out_of_range("");
        return get_product_via_rank(this->AVL_TREE_ROOT, rank);
    }

    // How many copies of product with given rank were sold
     size_t sold(size_t rank) const {
        if (this->product_map.size() < rank || 1 > rank)
            throw std::out_of_range("");
        return this->product_map.find(get_product_via_rank(this->AVL_TREE_ROOT, rank))->second->key;
    }

    // The same but sum over interval of products (including from && to)
    // It must hold: sold(x) == sold(x, x)
     size_t sold(size_t from, size_t to) const {
        if (from > this->product_map.size()) throw std::out_of_range("");
        else if (from < 1) throw std::out_of_range("");
        else if (to < 1) throw std::out_of_range("");
        else if (to > this->product_map.size()) throw std::out_of_range("");

        Product & lower_ground_product_product = get_product_via_rank(this->AVL_TREE_ROOT, from);
        Node * lower_ground_product_node = this->product_map.find(lower_ground_product_product)->second;

        Product & upper_ground_product_product = get_product_via_rank(this->AVL_TREE_ROOT, to);
        Node * upper_ground_product_node = this->product_map.find(upper_ground_product_product)->second;

        size_t to_return = return_lower_ground_product_key(this->AVL_TREE_ROOT, lower_ground_product_node->key, this->AVL_TREE_ROOT->total_subtree_sum);
        to_return = return_upper_ground_product_key(this->AVL_TREE_ROOT, upper_ground_product_node->key, to_return);

        size_t lower_ground_product_index = get_product_index_from_map(lower_ground_product_node, lower_ground_product_product);
        size_t upper_ground_product_index = get_product_index_from_map(upper_ground_product_node, upper_ground_product_product);

        to_return -= lower_ground_product_node->key * lower_ground_product_index;
        to_return -= upper_ground_product_node->key * (upper_ground_product_node->shipment_list.size() - upper_ground_product_index - 1);

        return to_return;
    }

    size_t first_same(size_t r) const {
         if (r < 1 || r > this->product_map.size()) throw std::out_of_range("");
         Product & product = get_product_via_rank(this->AVL_TREE_ROOT, r);
         size_t largest_product_rank_number = return_largest_product_rank_number(this->AVL_TREE_ROOT, this->product_map.find(product)->second->key, this->AVL_TREE_ROOT->total_subtree_size);
         Node * largest_product_rank_node = return_largest_product_rank_node(this->AVL_TREE_ROOT, this->product_map.find(product)->second->key, this->AVL_TREE_ROOT->total_subtree_size);
         return largest_product_rank_number - largest_product_rank_node->shipment_list.size() + 1;
    }
    size_t last_same(size_t r) const {
        if (r < 1 || r > this->product_map.size()) throw std::out_of_range("");
        Product & product = get_product_via_rank(this->AVL_TREE_ROOT, r);
        return return_largest_product_rank_number(this->AVL_TREE_ROOT, this->product_map.find(product)->second->key, this->AVL_TREE_ROOT->total_subtree_size);
     }


    ///=======================PROGTEST FUNCTIONS END========================================///
};


#ifndef __PROGTEST__

void test1() {
    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);
    assert(T.products() == 2);
    T.sell("ham", 2);
    T.sell("mushrooms", 12);

    assert(T.products() == 4);
    assert(T.rank("ham") == 3);
    assert(T.rank("coke") == 1);
    assert(T.sold(1, 3) == 46);
    assert(T.product(2) == "mushrooms");

    T.sell("ham", 11);
    assert(T.products() == 4);
    assert(T.product(2) == "ham");
    assert(T.sold(2) == 13);
    assert(T.sold(2, 2) == 13);
    assert(T.sold(1, 2) == 45);
    
}

void test2() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };

    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);

    CATCH(T.rank("ham"));
    CATCH(T.product(3));
    CATCH(T.sold(0));
    CATCH(T.sold(9));
    CATCH(T.sold(0, 1));
    CATCH(T.sold(3, 2));
    CATCH(T.sold(1, 9));

#undef CATCH
}

int main() {
    test1();
    test2();
}

#endif


