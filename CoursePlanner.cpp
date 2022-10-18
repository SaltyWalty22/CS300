//============================================================================
// Name        : CoursePlanner.cpp
// Author      : Trenton Walty
// Version     : 1.0
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================



// define a structure to hold bid information

struct Bid {
    string bidId; // unique identifier
    string title;
    string titleShort;
    string prereqOne;
    string prereqTwo;
        
};
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
        Node() {
        bid = aBid;
    }
};
class CoursePlanner {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node); 
    void preOrder(Node* node);  
    Node* removeNode(Node* node, string bidId);

public:
    CoursePlanner();
    virtual ~CoursePlanner();
    void InOrder();
    void PostOrder();   
    void PreOrder();    
    void Insert(Bid bid);
    void Remove(string bidId);
    void displayClass(Bid bid);
    void displayPrereqs(Bid bid);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
CoursePlanner::CoursePlanner() {
    //initialize housekeeping variables
    root = nullptr;
    //root is equal to nullptr
}
/**
 * Destructor
 */
CoursePlanner::~CoursePlanner() {
    // recurse from root deleting every node
}
/**
 * Traverse the tree in order
 */
void CoursePlanner::InOrder() {
    // call inOrder fuction and pass root
    inOrder(root);
}
/**
 * Traverse the tree in post-order
 */
void CoursePlanner::PostOrder() {
    // postOrder root
    postOrder(root);
}
/**
 * Traverse the tree in pre-order
 */
void CoursePlanner::PreOrder() {
    // preOrder root
    preOrder(root);
}
/**
 * Insert a bid
 */
void CoursePlanner::Insert(Bid bid) {
    
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node;
        root->bid = bid;
        root->left = nullptr;
        root->right = nullptr;
    }
    // else
    else {
        // add Node root and bid
        addNode(root, bid);
    }
}
/**
 * Remove a bid
 */
void CoursePlanner::Remove(string bidId) {
    // remove node root bidID
    removeNode(root, bidId);
}
Bid CoursePlanner::Search(string bidId) {
    // set current node equal to root
    Node* currentNode = root;

    int curKey = atoi(currentNode->bid.bidId.c_str());
    int key = atoi(bidId.c_str());
    while (currentNode != nullptr) {
        // keep looping downwards until bottom reached or matching bidId found (Zybooks 6.4.1 BST search used as example)
        if (curKey == key) {
            // if match found, return current bid
            Bid bid;
            return bid;
        }
        else if (key < curKey) {
            // if bid is smaller than current node then traverse left
            currentNode = currentNode->left;
        }
        else {
            // else larger so traverse right
            currentNode = currentNode->right;
        }
    }
    Bid bid;
    return bid;
}
/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void CoursePlanner::addNode(Node* node, Bid bid) {
    int curKey = atoi(node->bid.bidId.c_str());
    int key = atoi(bid.bidId.c_str());

    // if node is larger then add to left
    if (curKey < key) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node;
            node->left->bid = bid;
            node->left->left = nullptr;
            node->left->right = nullptr;
        }
        else {
            // else recurse down the left node
            addNode(node->left, bid);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node;
            node->right->bid = bid;
            node->right->left = nullptr;
            node->right->right = nullptr;
        }
        //else
        else {
            // recurse down the left node (Note says down left but I believe this is suppose to go down the right node)
            addNode(node->right, bid);
        }
    }

}
void CoursePlanner::inOrder(Node* node) {
    //if node is not equal to null ptr (zyBooks 6.7.1)
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);
        //output class and classShort
        Bid bid;
        displayClass(bid);
        inOrder(node->right);
    }
    //InOder right
}
void CoursePlanner::postOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //postOrder left
        postOrder(node->left);
        //postOrder right
        postOrder(node->right);
        //output class and classShort
        Bid bid;
        displayClass(bid);
    }
}
void CoursePlanner::preOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //output bidID, title, amount, fund
        Bid bid;
        displayClass(bid);
        //postOrder left
        preOrder(node->left);
        //postOrder right 
        preOrder(node->right);
    }
}
/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayClass(Bid bid) {
    cout << bid.titleShort << ", " << bid.title << endl;
    return;
}
void displayPrereqs(Bid bid) {
    if (bid.prereqOne == "none") {
        cout << "Prerequisites: None" << endl;
    }
    if ( bid.prereqOne != "none" &&  bid.prereqTwo == "none") {
        cout << "Prerequisites: " << bid.prereqOne << endl;
    }
    if (bid.prereqTwo != "none")
    cout << "Prerequisites: " << bid.prereqOne << ", " << bid.prereqTwo << endl;
}
/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, CoursePlanner* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.titleShort = file[i][2];
            bid.prereqOne = file[i][3];
            bid.prereqTwo = file[i][4];

            //cout << bid.title << ", " << titleShort << 

            // push this bid to the end
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}
/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "1";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Courses.csv";
        bidKey = "1";
    }

    // Define a binary search tree to hold all bids
    CoursePlanner* bst;
    bst = new CoursePlanner();
    Bid bid;

    int choice = 0;
    string selectedclass = "";
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            loadBids(csvPath, bst);
            cout << "Courses loaded" << endl;
            
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            cin >> selectedclass;
            bid = bst->Search(selectedclass);
            if (!bid.titleShort.empty()) {
                displayClass(bid);
                displayPrereqs(bid);
            }
            else {
                cout << "Course: " << selectedclass << " not found." << endl;
            }
        default:
            cout << choice << " is not a valid option." << endl;

            break;

        
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;
}