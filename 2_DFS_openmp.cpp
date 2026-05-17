#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

struct Node {
    int data;
    vector<Node*> children;

    Node(int val) {
        data = val;
    }
};

class ParallelDFS {
private:
    Node* root;

public:
    ParallelDFS() {
        root = NULL;
    }

    // Insert nodes level-wise
    void insert(int data) {

        Node* newNode = new Node(data);

        if (root == NULL) {
            root = newNode;
            return;
        }

        vector<Node*> q;
        q.push_back(root);

        while (!q.empty()) {

            Node* temp = q[0];
            q.erase(q.begin());

            // Binary Tree (max 2 children)
            if (temp->children.size() < 2) {
                temp->children.push_back(newNode);
                return;
            }

            for (auto child : temp->children) {
                q.push_back(child);
            }
        }
    }

    Node* getRoot() {
        return root;
    }

    // Sequential DFS
    void dfsSequential(Node* root) {

        if (root == NULL)
            return;

        stack<Node*> st;
        st.push(root);

        cout << "\nSequential DFS: ";

        while (!st.empty()) {

            Node* curr = st.top();
            st.pop();

            cout << curr->data << " ";

            // Push children in reverse order
            for (int i = curr->children.size() - 1; i >= 0; i--) {
                st.push(curr->children[i]);
            }
        }

        cout << endl;
    }

    // Parallel DFS using OpenMP
    void dfsParallel(Node* root) {

        if (root == NULL)
            return;

        cout << "\nParallel DFS: ";

        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFS(root);
            }
        }

        cout << endl;
    }

    void parallelDFS(Node* node) {

        if (node == NULL)
            return;

        // Print safely
        #pragma omp critical
        {
            cout << node->data << " ";
        }

        // Create parallel tasks for children
        for (auto child : node->children) {

            #pragma omp task
            {
                parallelDFS(child);
            }
        }

        #pragma omp taskwait
    }

    // Print Tree
    void printTree(Node* root, int level = 0) {

        if (root == NULL)
            return;

        for (int i = 0; i < level; i++)
            cout << "  ";

        cout << root->data << endl;

        for (auto child : root->children) {
            printTree(child, level + 1);
        }
    }
};

int main() {

    ParallelDFS tree;

    int n, value;

    cout << "===== Parallel DFS using OpenMP =====" << endl;

    cout << "\nEnter number of nodes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {

        cout << "Enter node value: ";
        cin >> value;

        tree.insert(value);
    }

    Node* root = tree.getRoot();

    cout << "\nTree Structure:\n";
    tree.printTree(root);

    tree.dfsSequential(root);

    tree.dfsParallel(root);

    return 0;
}

/*Time Complexity

DFS Complexity:

O(V+E)

Where:

V = vertices
E = edges*/

