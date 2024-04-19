#ifndef __PROGTEST__
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
#endif /* __PROGTEST__ */

namespace memory {
    void *memPool = nullptr; // Pointer to start of memory pool
    int memSize = 0;         // Size of memory pool
    int alloc_counter = 0;  // Stores amount of allocations subtract amount of deallocations.
}

// Struct to represent a block of memory in the pool
struct Node {
    int size = 0;           // Size of the node (excluding Node struct)
    bool allocated = false; // Indicates if the node is allocated
    Node * left = nullptr;   // Pointer to the left child node
    Node * right = nullptr;  // Pointer to the right child node
};

Node *findNode(Node *node, int requiredSize) {
    if (node == nullptr || node->allocated || node->size < requiredSize)
        return nullptr;

    if (node->size == requiredSize)
        return node;

    Node *leftNode = findNode(node->left, requiredSize);
    if (leftNode != nullptr)
        return leftNode;

    return findNode(node->right, requiredSize);
}

Node *getParent(Node *node) {
    uintptr_t nodeAddress = reinterpret_cast<uintptr_t>(node);
    uintptr_t poolAddress = reinterpret_cast<uintptr_t>(memory::memPool);
    uintptr_t offset = nodeAddress - poolAddress;
    uintptr_t parentOffset = offset / (2 * node->size);
    return reinterpret_cast<Node *>(poolAddress + parentOffset);
}

Node *getBuddy(Node *node) {
    Node *parent = getParent(node);
    if (parent->left == node)
        return parent->right;
    else
        return parent->left;
}

void splitNode(Node *node) {
    int newSize = node->size / 2;

    node->left = node + 1;
    node->left->size = newSize - sizeof(Node);
    node->left->allocated = false;
    node->left->left = nullptr;
    node->left->right = nullptr;

    node->right = node + newSize;
    node->right->size = newSize - sizeof(Node);
    node->right->allocated = false;
    node->right->left = nullptr;
    node->right->right = nullptr;
}

void mergeNodes(Node *node1, Node *node2) {
    Node *parent = getParent(node1);
    if (parent->left == node1)
        parent->left = nullptr;
    else
        parent->right = nullptr;

    // Calculate the size of the merged node
    int newSize = node1->size * 2 + sizeof(Node);

    // Update the merged node
    node1->size = newSize - sizeof(Node);
    node1->left = nullptr;
    node1->right = nullptr;

    // Update the parent node
    parent->size = newSize - sizeof(Node);
    parent->left = node1;
    parent->right = nullptr;
}

// Function to initialize the memory pool
Node *root = nullptr; // Pointer to the root node

void HeapInit(void *startAddr, int size) {
    // Set the memory pool start address and size in the memory namespace
    memory::memPool = startAddr;
    memory::memSize = size;

    // Create the root node that covers the entire memory pool
    root = reinterpret_cast<Node *>(memory::memPool);
    root->size = memory::memSize - sizeof(Node); // Set size to cover the entire pool, minus the size of the Node struct itself
    root->allocated = false;
    root->left = nullptr;
    root->right = nullptr;

    memory::alloc_counter = 0;
}

// Function to allocate memory from the pool
void *HeapAlloc(int size) {
    // Calculate the required size including the Node struct
    int requiredSize = size + sizeof(Node);

    // Find a suitable node in the tree
    Node *node = findNode(root, requiredSize);
    if (node == nullptr)
        return nullptr; // No suitable node found

    // Split larger nodes until we reach the desired size
    while (node->size >= 2 * requiredSize) {
        splitNode(node);
        node = node->left;
    }

    // Mark the node as allocated
    node->allocated = true;

    // Update the allocation counter
    memory::alloc_counter++;

    // Return a pointer to the allocated memory block
    return reinterpret_cast<void *>(node + 1);
}

bool HeapFree(void *blk) {
    Node *node = reinterpret_cast<Node *>(blk) - 1;

    // Check if the node is already freed
    if (!node->allocated)
        return false;

    // Mark the node as freed
    node->allocated = false;

    // Merge with buddy nodes if they are also free
    while (node->size < memory::memSize) {
        Node *buddy = getBuddy(node);
        if (buddy->allocated)
            break; // Buddy is allocated, stop merging

        mergeNodes(node, buddy);
        node = node->left; // Move up to the parent node
    }

    memory::alloc_counter--;

    return true;
}



void HeapDone(int *pendingBlk) {
    *pendingBlk = memory::alloc_counter;
}

#ifndef __PROGTEST__
int main(void) {
    uint8_t *p0, *p1, *p2, *p3, *p4;
    int pendingBlk;
    static uint8_t memPool[3 * 1048576];

    HeapInit(memPool, 2097152);
    assert((p0 = (uint8_t *)HeapAlloc(512000)) != NULL);
    memset(p0, 0, 512000);
    assert((p1 = (uint8_t *)HeapAlloc(511000)) != NULL);
    memset(p1, 0, 511000);
    assert((p2 = (uint8_t *)HeapAlloc(26000)) != NULL);
    memset(p2, 0, 26000);
    HeapDone(&pendingBlk);
    assert(pendingBlk == 3);

    HeapInit(memPool, 2097152);
    assert((p0 = (uint8_t *)HeapAlloc(1000000)) != NULL);
    memset(p0, 0, 1000000);
    assert((p1 = (uint8_t *)HeapAlloc(250000)) != NULL);
    memset(p1, 0, 250000);
    assert((p2 = (uint8_t *)HeapAlloc(250000)) != NULL);
    memset(p2, 0, 250000);
    assert((p3 = (uint8_t *)HeapAlloc(250000)) != NULL);
    memset(p3, 0, 250000);
    assert((p4 = (uint8_t *)HeapAlloc(50000)) != NULL);
    memset(p4, 0, 50000);
    assert(HeapFree(p2));
    assert(HeapFree(p4));
    assert(HeapFree(p3));
    assert(HeapFree(p1));
    assert((p1 = (uint8_t *)HeapAlloc(500000)) != NULL);
    memset(p1, 0, 500000);
    assert(HeapFree(p0));
    assert(HeapFree(p1));
    HeapDone(&pendingBlk);
    assert(pendingBlk == 0);

    HeapInit(memPool, 2359296);
    assert((p0 = (uint8_t *)HeapAlloc(1000000)) != NULL);
    memset(p0, 0, 1000000);
    assert((p1 = (uint8_t *)HeapAlloc(500000)) != NULL);
    memset(p1, 0, 500000);
    assert((p2 = (uint8_t *)HeapAlloc(500000)) != NULL);
    memset(p2, 0, 500000);
    assert((p3 = (uint8_t *)HeapAlloc(500000)) == NULL);
    assert(HeapFree(p2));
    assert((p2 = (uint8_t *)HeapAlloc(300000)) != NULL);
    memset(p2, 0, 300000);
    assert(HeapFree(p0));
    assert(HeapFree(p1));
    HeapDone(&pendingBlk);
    assert(pendingBlk == 1);

    HeapInit(memPool, 2359296);
    assert((p0 = (uint8_t *)HeapAlloc(1000000)) != NULL);
    memset(p0, 0, 1000000);
    assert(!HeapFree(p0 + 1000));
    HeapDone(&pendingBlk);
    assert(pendingBlk == 1);

    return 0;
}
#endif /* __PROGTEST__ */
