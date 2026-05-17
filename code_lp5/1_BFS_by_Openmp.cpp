#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

struct Node {
    int data;
    vector<Node*> children;

    Node(int val) {
        data = val;
    }
};

class ParallelBFS {
private:
    Node* root;

public:
    ParallelBFS() {
        root = NULL;
    }

    // Insert node level-wise
    void insert(int data) {
        Node* newNode = new Node(data);

        if (root == NULL) {
            root = newNode;
            return;
        }

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            // Binary Tree (max 2 children)
            if (temp->children.size() < 2) {
                temp->children.push_back(newNode);
                return;
            }

            for (auto child : temp->children) {
                q.push(child);
            }
        }
    }

    Node* getRoot() {
        return root;
    }

    // Sequential BFS
    void bfsSequential(Node* root) {
        if (root == NULL)
            return;

        queue<Node*> q;
        q.push(root);

        cout << "\nSequential BFS: ";

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            cout << curr->data << " ";

            for (auto child : curr->children) {
                q.push(child);
            }
        }

        cout << endl;
    }

    // Parallel BFS using OpenMP
    void bfsParallel(Node* root) {
        if (root == NULL)
            return;

        vector<Node*> currentLevel;
        currentLevel.push_back(root);

        cout << "\nParallel BFS: ";

        while (!currentLevel.empty()) {

            vector<Node*> nextLevel;

            // Parallel processing of current level
            #pragma omp parallel
            {
                vector<Node*> localNext;

                #pragma omp for
                for (int i = 0; i < currentLevel.size(); i++) {

                    // Print safely
                    #pragma omp critical
                    {
                        cout << currentLevel[i]->data << " ";
                    }

                    // Store children
                    for (auto child : currentLevel[i]->children) {
                        localNext.push_back(child);
                    }
                }

                // Merge local vectors safely
                #pragma omp critical
                {
                    nextLevel.insert(nextLevel.end(),
                                     localNext.begin(),
                                     localNext.end());
                }
            }

            currentLevel = nextLevel;
        }

        cout << endl;
    }

    // Display Tree
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

    ParallelBFS tree;

    int n, value;

    cout << "Enter number of nodes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "Enter node value: ";
        cin >> value;

        tree.insert(value);
    }

    Node* root = tree.getRoot();

    cout << "\nTree Structure:\n";
    tree.printTree(root);

    tree.bfsSequential(root);

    tree.bfsParallel(root);

    return 0;
}

/**Enter number of nodes: 7
Enter node value: 1
Enter node value: 2
Enter node value: 3
Enter node value: 4
Enter node value: 5
Enter node value: 6
Enter node value: 7

Tree Structure:
1
  2
    4
    5
  3
    6
    7

Sequential BFS: 1 2 3 4 5 6 7

Parallel BFS: 1 2 3 4 5 6 7*/
