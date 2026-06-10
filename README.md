# University Management System

## Overview

A C++ based University Management System implementing Hash Tables, AVL Trees, Graphs, Queues, and Stacks for efficient campus management operations.

The application demonstrates the practical implementation of:

* Hash Tables
* AVL Trees
* Graphs
* Queues
* Stacks
* Linked Lists

This project was developed as a Data Structures course project.

---

## Developers

* **Areeba Fatima (24F-0619)**
* **Ayesha Yasin (24F-0565)**


---

## Project Features

### 1. User Management System (Hash Table)

The user management module stores and manages university users.

#### Features

* User Registration
* User Login Authentication
* User Search
* User Profile Update
* User Deletion
* View User Profiles
* Track Last Reserved Room

#### Data Structure Used

**Hash Table with Chaining**

#### Purpose

Provides efficient user storage and retrieval with near O(1) average search time.

---

### 2. Campus Navigation System (Graph)

The campus map is represented using a graph where:

* Buildings are vertices
* Roads/Walkways are edges

#### Features

* Add Buildings
* Remove Buildings
* Add Paths
* Remove Paths
* Display Campus Map
* Breadth First Search (BFS)
* Depth First Search (DFS)
* Shortest Path Calculation (Dijkstra's Algorithm)

#### Data Structure Used

**Graph using Adjacency Lists**

#### Algorithms Used

* BFS
* DFS
* Dijkstra's Algorithm

---

### 3. Room Booking and Scheduling System (AVL Tree)

Each building maintains its own AVL Tree of rooms.

#### Features

* Add Rooms
* Delete Rooms
* Search Rooms
* Search Rooms by Type
* Reserve Rooms
* Cancel Reservations
* AVL Traversals

  * Inorder
  * Preorder
  * Postorder
* AVL Statistics

  * Height
  * Number of Rooms
  * Leaf Nodes
  * Internal Nodes

#### Data Structure Used

**AVL Tree**

#### Purpose

Maintains balanced room records for efficient insertion, deletion, and searching.

---

### 4. Complaint Management System (Queue)

The complaint system processes maintenance issues in the order they are received.

#### Features

* Submit Complaints
* Process Complaints
* View Next Complaint
* Display Pending Complaints
* Complaint Statistics

#### Data Structure Used

**Queue (FIFO)**

#### Purpose

Ensures fair complaint handling based on arrival order.

---

### 5. Messaging System (Stack)

Allows communication between registered users.

#### Features

* Send Messages
* View Latest Message
* Pop Latest Message
* Display Conversations

#### Data Structure Used

* Stack
* Linked Lists

#### Purpose

Stores messages using Last-In-First-Out (LIFO) behavior for quick access to recent messages.

---

## Data Structures Used

| Module               | Data Structure |
| -------------------- | -------------- |
| User Directory       | Hash Table     |
| Campus Navigation    | Graph          |
| Room Scheduling      | AVL Tree       |
| Complaint Management | Queue          |
| Messaging System     | Stack          |
| Internal Storage     | Linked Lists   |

---

## Default Login Credentials

### Admin

* Username: Ayesha Masood
* Password: ayesha123

### Teacher

* Username: Areeba
* Password: areeba123

### Student

* Username: Ayesha
* Password: ayesha123

---

## Program Flow

1. User Login/Register
2. Access Main Menu
3. Choose Module:

   * User Management
   * Campus Navigation
   * Room Booking
   * Complaint System
   * Messaging System
   * Profile Management
4. Perform Required Operations
5. Logout or Exit

---

## Time Complexity Summary

| Operation         | Complexity   |
| ----------------- | ------------ |
| Hash Table Search | O(1) Average |
| AVL Search        | O(log n)     |
| AVL Insert        | O(log n)     |
| AVL Delete        | O(log n)     |
| BFS               | O(V + E)     |
| DFS               | O(V + E)     |
| Dijkstra          | O(V²)        |
| Queue Enqueue     | O(1)         |
| Queue Dequeue     | O(1)         |
| Stack Push        | O(1)         |
| Stack Pop         | O(1)         |

---

## Technologies Used

* C++
* Object-Oriented Programming (OOP)
* Dynamic Memory Allocation
* Linked Lists
* Trees
* Graphs
* Queues
* Stacks
* Hashing

---

## Learning Outcomes

This project demonstrates:

* Practical implementation of core Data Structures.
* Efficient searching, insertion, deletion, and traversal operations.
* Real-world application of graphs, AVL trees, hash tables, queues, and stacks.
* Object-Oriented Programming concepts in C++.
* Modular software design.

---

## Conclusion

The University Management System is a comprehensive Data Structures project that integrates multiple advanced data structures into a real-world university environment. It provides user management, navigation, room scheduling, complaint handling, and messaging functionalities while showcasing efficient data organization and algorithmic techniques.
