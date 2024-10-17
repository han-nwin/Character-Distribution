




/*
    //TEST PART//
    std::cout << "======================================================="<< std::endl;
    // Test the AVL tree for integers
    AVLTree<int,int> intTree;
    std::cout << "Empty? "<< intTree.empty() << std::endl;

    // Insert integer keys with associated values
    intTree.insert(20, 1);
    intTree.insert(25, 2);
    intTree.insert(15, 3);
    intTree.insert(10, 4);
    intTree.insert(50, 5);
    intTree.insert(80, 6);
    intTree.insert(30, 7);
    intTree.insert(35, 8);
    

    // Insert some duplicate values to check count increments
    intTree.insert(20, 1); // Increment count for value 1 under key 20
    intTree.insert(50, 5); // Increment count for value 5 under key 50
    std::cout << "Size: " << intTree.size() << std::endl;   
    std::cout << "In-order traversal of integer AVL tree with values and counts:\n";
    intTree.display();
    std::cout << "SIZE: " << intTree.size() << std::endl;
    std::cout << "Empty? "<< intTree.empty() << std::endl;

    int valtofind = 20;
    std::cout << "Find key: " << valtofind << std::endl;
    intTree.find(20);

    int valtorm = 20;
    std::cout << "Remove key: " << valtorm << std::endl;
    intTree.remove(valtorm);
    intTree.display();

    // Test the AVL tree for strings
    AVLTree<std::string,std::string> stringTree1;
    std::cout << "Empty? "<< stringTree1.empty() << std::endl;

    // Insert string keys with associated string values
    stringTree1.insert("banana", "ba");
    stringTree1.insert("apple", "ap");
    stringTree1.insert("orange", "or");

    // Insert some duplicate values to check count increments
    stringTree1.insert("banana", "ba"); // Increment count for "ba" under "banana"
    stringTree1.insert("orange", "or"); // Increment count for "or" under "orange"
    std::cout << "Size: " << stringTree1.size() << std::endl;

    std::cout << "In-order traversal of string AVL tree with values and counts:\n";
    stringTree1.display();
    std::cout << "SIZE: " << stringTree1.size() << std::endl;
    std::cout << "Empty? "<< stringTree1.empty() << std::endl;

    std::string strvaltofind = "banana";
    std::cout << "Find key: " << strvaltofind << std::endl;
    stringTree1.find("banana");
    
    std::string strvaltorm = "banana";
    std::cout << "Remove key: " << strvaltorm << std::endl;
    stringTree1.remove(strvaltorm);
    stringTree1.display();
    
    return 0;
}

*/