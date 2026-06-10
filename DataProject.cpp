#include<iostream>
#include<string>
#include<algorithm> 
using namespace std;

///<----------------------- MODULE A ----------------------------->
////Entry for hash table
struct Person {
	string userName;
	string password;
	string role;     // student / teacher / admin
	string department;
	string email;

	string lastBookedRoom;
	Person* next; 

public:
	Person() {
		userName = "";
		password = "";
		role = "";
		department = "";
		email = "";
		lastBookedRoom = "";
		next = nullptr;
	}
	Person(const string& user, const string& pass, const string& r, const string& dept, const string& mail) {
		userName = user;
		password = pass;
		role = r;
		department = dept;
		email = mail;
		lastBookedRoom = "";
		next = nullptr;
	}
	~Person() {
		next = nullptr;
	}
};

/// hash table implementation  (With Chaining)
class HashTable {
private:
	Person** table; // array of pointers to Person(heads of chains)
	int capacity;  // number of buckets (size of table)
	int size_;  // number of users stored - renamed from 'size' to avoid conflict
	const double loadFactor = 0.7;

	int hashString(const string& s) const {
		int hash = 0;

		for (int i = 0; i < s.length(); i++) {
			hash = (hash * 29) + s[i];    // multiply + add
			if (hash < 0) hash = -hash;   // keep positive
		}

		return hash;
	}
	int indexForKey(const string& key) const {
		return hashString(key) % capacity;
	}

	void rehash(int newCapacity) {
		Person** oldTable = table;
		int oldCap = capacity;

		table = new Person * [newCapacity];
		for (int i = 0; i < newCapacity; ++i) table[i] = nullptr;

		capacity = newCapacity;
		size_ = 0;

		for (int i = 0; i < oldCap; ++i) {
			Person* node = oldTable[i];
			while (node) {
				Person* nextNode = node->next;
				node->next = nullptr;
				insertExistingNode(node); // reinsert node into new table
				node = nextNode;
			}
		}

		// delete old table array
		delete[] oldTable;
	}

	void insertExistingNode(Person* node) {
		int idx = indexForKey(node->userName);
		node->next = table[idx];
		table[idx] = node;
		++size_;
	}

	void ensureCapacityForInsert() {
		double load = (double)(size_ + 1) / (double)capacity;

		if (load > loadFactor) {
			int newCap = capacity * 2;
			if (newCap < 11) newCap = 11;

			rehash(newCap);
		}
	}

public:
	HashTable(int cap = 10) {
		if (cap < 11) cap = 11;
		capacity = cap;
		size_ = 0;
		table = new Person * [capacity];
		for (int i = 0; i < capacity; ++i) table[i] = nullptr;

	}
	~HashTable() {
		for (int i = 0; i < capacity; ++i) {
			Person* node = table[i];
			while (node) {
				Person* tmp = node;
				node = node->next;
				delete tmp;
			}
		}
		delete[] table;
	}
	bool insertUser(const string& user, const string& pass, const string& role, const string& dept, const string& email) {

		if (user.empty()) return false;

		if (searchUser(user) != nullptr) {

			return false;
		}

		ensureCapacityForInsert();

		Person* p = new Person(user, pass, role, dept, email);
		int idx = indexForKey(user);
		p->next = table[idx];
		table[idx] = p;
		++size_;
		return true;
	}

	Person* searchUser(const string& user) const {
		if (user.empty()) return nullptr;
		int idx = indexForKey(user);
		Person* node = table[idx];
		while (node) {
			if (node->userName == user) return node;
			node = node->next;
		}
		return nullptr;
	}

	bool deleteUser(const string& user) {
		if (user.empty()) return false;
		int idx = indexForKey(user);
		Person* node = table[idx];
		Person* prev = nullptr;
		while (node) {
			if (node->userName == user) {
				if (prev) prev->next = node->next;
				else table[idx] = node->next;
				delete node;
				--size_;
				return true;
			}
			prev = node;
			node = node->next;
		}
		return false;
	}

	bool updateProfile(const string& user, const string& newPassword, const string& newRole, const string& newDept, const string& newEmail) {
		Person* p = searchUser(user);
		if (!p) return false;
		if (!newPassword.empty()) p->password = newPassword;
		if (!newRole.empty()) p->role = newRole;
		if (!newDept.empty()) p->department = newDept;
		if (!newEmail.empty()) p->email = newEmail;
		return true;
	}

	bool login(const string& user, const string& pass) const {
		Person* p = searchUser(user);
		if (!p) return false;
		return (p->password == pass);
	}


	void displayAll() const {
		cout << endl;
		cout << "<---------------------------------------------- Hash Table ------------------------------------------------------->" << endl;
		cout << "Capacity = " << capacity << "                   Users = " << size_ << endl;
		for (int i = 0; i < capacity; ++i) {
			cout << "Bucket[ " << i << " ] : ";
			Person* node = table[i];
			if (!node) {
				cout << "(Empty)\n";
				continue;
			}
			while (node) {
				cout << "[ " << node->userName << "  ( " << node->role << " ) ] -> ";
				node = node->next;
			}
			cout << "NULL" << endl;
		}
	}

	void printUserProfile(const string& user) const {
		Person* p = searchUser(user);
		if (!p) {
			cout << "User " << user << " not found.\n";
			return;
		}
		cout << endl;
		cout << "^-^-^-^-^-^-^- Profile of " << p->userName << " -^-^-^-^-^-^-^" << endl;
		cout << "Role       : " << p->role << "\n";
		cout << "Department : " << p->department << "\n";
		cout << "Email      : " << p->email << "\n";
		if ((p->lastBookedRoom.empty())) {
			cout << "Last Room  : None" << endl;
		}
		else {
			cout << "Last Room  : " << p->lastBookedRoom << "\n";
		}
	}
	bool updateLastBookedRoom(const string& user, const string& roomID) {
		Person* p = searchUser(user);
		if (!p) return false;

		p->lastBookedRoom = roomID;
		return true;
	}

	int getSize() { return size_; }  
	int bucketCount() { return capacity; }
	//<------------------------ Module A menu -------------------------->
	void userDirectoryMenu(HashTable& ht) {
		while (true) {
			cout << endl;
			cout << "*-*-*-*-*-*-*-*-*-* USER DIRECTORY *-*-*-*-*-*-*-*-*-*" << endl;
			cout << "1. Register New User" << endl;
			cout << "2. Login " << endl;
			cout << "3. Update Profile" << endl;
			cout << "4. Delete User" << endl;
			cout << "5. Search User" << endl;
			cout << "6. Display All Users (buckets)" << endl;
			cout << "7. Back / Exit " << endl;
			cout << "Choose option: ";
			int opt;

			if (!(cin >> opt)) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid input. Try again.\n";
				continue;
			}
			cin.ignore(1000, '\n'); // clear / ignore  newline

			if (opt == 1) {
				string user, pass, role, dept, email;
				cout << "Enter Username: ";
				getline(cin, user);
				cout << "Enter Password: ";
				getline(cin, pass);
				cout << "Enter Role (Student/Teacher/Admin): ";
				getline(cin, role);
				cout << "Enter Department: ";
				getline(cin, dept);
				cout << "Enter Email: ";
				getline(cin, email);

				if (ht.insertUser(user, pass, role, dept, email)) {
					cout << "Registration successful for '" << user << "'.\n";
				}
				else {
					cout << "Registration failed. Username may already exist or invalid input.\n";
				}
			}
			else if (opt == 2) {
				string user, pass;
				cout << "Username: ";
				getline(cin, user);
				cout << "Password: ";
				getline(cin, pass);
				if (ht.login(user, pass)) cout << "Login successful.\n";
				else cout << "Login failed. Check username/password.\n";
			}
			else if (opt == 3) {
				string user;
				cout << "Enter Username to update: ";
				getline(cin, user);
				if (!ht.searchUser(user)) {
					cout << "User not found." << endl;
					continue;
				}
				string newPass, newRole, newDept, newEmail;
				cout << "(Leave blank to keep current) New Password: "; getline(cin, newPass);
				cout << "New Role: "; getline(cin, newRole);
				cout << "New Department: "; getline(cin, newDept);
				cout << "New Email: "; getline(cin, newEmail);
				if (ht.updateProfile(user, newPass, newRole, newDept, newEmail)) {
					cout << "Profile Updated." << endl;
				}
				else cout << "Failed to Update Profile." << endl;
			}
			else if (opt == 4) {
				string user;
				cout << "Enter Username to delete: ";
				getline(cin, user);
				if (ht.deleteUser(user)) cout << "User " << user << " deleted." << endl;
				else cout << "Delete failed. User may not exist." << endl;
			}
			else if (opt == 5) {
				string user;
				cout << "Search Username: ";
				getline(cin, user);
				ht.printUserProfile(user);
			}
			else if (opt == 6) {
				ht.displayAll();
			}
			
			else if (opt == 7) {
				cout << "Returning to main " << endl;
				break;
			}
			else {
				cout << "Invalid choice. Try again." << endl;
			}
		}
	}

};

// Module A complete ;)

//<---------------------------------- MODULE B :)------------------------------------->

struct Edge {
	string destBuildingName;
	string relation;    // road / walkway
	string status;      // open / closed
	Edge* next;

	Edge(string dest, string rel, string stat) : destBuildingName(dest), relation(rel), status(stat), next(nullptr) {}
	Edge() : destBuildingName(""), relation(""), status(""), next(nullptr) {}

};

struct BuildingNode {
	string buildingName;
	Edge* adjList;      // head of linked list of edges
	BuildingNode* next; // next building in graph (linked list)
	bool visited;      // for BFS/DFS
	int distance;      // for Dijkstra
	string parent;     // to rebuild path

	BuildingNode(string name) : buildingName(name), adjList(nullptr), next(nullptr), visited(false), distance(999999), parent("") {}
	BuildingNode() : buildingName(""), adjList(nullptr), next(nullptr), visited(false), distance(999999), parent("") {}
};

// Queue implementation for BFS
struct QNode {
	string name;
	QNode* next;
	QNode(string n) : name(n), next(nullptr) {}
	QNode() : name(""), next(nullptr) {}
};

class Queue {
	QNode* front;
	QNode* rear;

public:
	Queue() : front(nullptr), rear(nullptr) {}

	bool isEmpty() { return front == nullptr; }

	void enqueue(string n) {
		QNode* temp = new QNode(n);
		if (!rear) {
			front = rear = temp;
			return;
		}
		rear->next = temp;
		rear = temp;
	}

	string dequeue() {
		if (!front) return "";
		QNode* temp = front;
		string val = temp->name;
		front = front->next;
		if (!front) rear = nullptr;
		delete temp;
		return val;
	}
};

//stack implementation for DFS

struct SNode {
	string name;
	SNode* next;
	SNode(string s) : name(s), next(nullptr) {}
	SNode() : name(""), next(nullptr) {}
};

class Stack {
	SNode* top;

public:
	Stack() : top(nullptr) {}

	bool isEmpty() { return top == nullptr; }

	void push(string n) {
		SNode* temp = new SNode(n);
		temp->next = top;
		top = temp;
	}

	string pop() {
		if (!top) return "";
		SNode* temp = top;
		string val = temp->name;
		top = top->next;
		delete temp;
		return val;
	}
};

class Graph {
private:
	BuildingNode* head;
	//O(V)
	BuildingNode* findBuilding(string name) {
		BuildingNode* temp = head;
		while (temp) {
			if (temp->buildingName == name) return temp;
			temp = temp->next;
		}
		return nullptr;
	}

public:
	Graph() : head(nullptr) {}
	//o(v)
	bool addBuilding(string name) {
		if (findBuilding(name)) return false;
		BuildingNode* newNode = new BuildingNode(name);
		newNode->next = head;
		head = newNode;
		return true;
	}
	// O(v)
	bool addPath(string src, string dest, string relation, string status) {
		BuildingNode* s = findBuilding(src);
		BuildingNode* d = findBuilding(dest);

		if (!s || !d) return false;

		// Add edge src->dest
		Edge* e1 = new Edge(dest, relation, status);
		e1->next = s->adjList;
		s->adjList = e1;

		// Add edge dest->src (undirected)
		Edge* e2 = new Edge(src, relation, status);
		e2->next = d->adjList;
		d->adjList = e2;

		return true;
	}
	//(OV+E)
	bool removePath(string src, string dest) {
		BuildingNode* s = findBuilding(src);
		BuildingNode* d = findBuilding(dest);

		if (!s || !d) return false;

		// Remove src->dest
		Edge** ptr = &s->adjList;
		while (*ptr) {
			if ((*ptr)->destBuildingName == dest) {
				Edge* del = *ptr;
				*ptr = (*ptr)->next;
				delete del;
				break;
			}
			ptr = &((*ptr)->next);
		}

		// Remove dest->src
		ptr = &d->adjList;
		while (*ptr) {
			if ((*ptr)->destBuildingName == src) {
				Edge* del = *ptr;
				*ptr = (*ptr)->next;
				delete del;
				break;
			}
			ptr = &((*ptr)->next);
		}

		return true;
	}
	//O(v)*O(V+E)
	bool removeBuilding(string name) {
		if (!head) return false;

		//  Remove building node from building list
		BuildingNode** ptr = &head;
		while (*ptr) {
			if ((*ptr)->buildingName == name) {
				BuildingNode* del = *ptr;
				*ptr = (*ptr)->next;

				// free edges
				Edge* e = del->adjList;
				while (e) {
					Edge* next = e->next;
					delete e;
					e = next;
				}
				delete del;
				break;
			}
			ptr = &((*ptr)->next);
		}

		       //  Remove all edges pointing to this building
	BuildingNode* b = head;
		while (b) {
			removePath(b->buildingName, name);  // safe if no path exists
			b = b->next;
		}

		cout << "Building " << name << " has been removed." << endl;

		return true;
	}

	void display() {
		BuildingNode* temp = head;
		while (temp) {
			cout << temp->buildingName << " -> ";
			Edge* e = temp->adjList;
			while (e) {
				cout << "( " << e->destBuildingName << " , " << e->relation << " , " << e->status << " ) -> ";
				e = e->next;
			}
			cout << "NULL\n";
			temp = temp->next;
		}
	}

	void resetVisited() {
		BuildingNode* t = head;
		while (t) {
			t->visited = false;
			t = t->next;
		}
	}
	///// Breadth-First Search (BFS)    O(v^2 +VE)
void BFS(string start) {

	// normaly o(v+E)
		BuildingNode* s = findBuilding(start);
		if (!s) {
			cout << "Building not found." << endl;
			return;
		}

		resetVisited();
		Queue q;

		s->visited = true;
		q.enqueue(s->buildingName);

		cout << "BFS Order: ";

		while (!q.isEmpty()) {
			string curr = q.dequeue();
			cout << curr << " ";

			BuildingNode* node = findBuilding(curr);
			Edge* e = node->adjList;

			while (e) {
				BuildingNode* neigh = findBuilding(e->destBuildingName);
				if (!neigh->visited) {
					neigh->visited = true;
					q.enqueue(neigh->buildingName);
				}
				e = e->next;
			}
		}
		cout << endl;
	}
//O(V^2 + VE)
	//------- Depth-First Search (DFS) ----------------
	void DFS(string start) {
		BuildingNode* s = findBuilding(start);
		if (!s) {
			cout << "Building not found." << endl;
			return;
		}

		resetVisited();
		Stack st;

		st.push(s->buildingName);
		cout << "DFS Order: ";

		while (!st.isEmpty()) {
			string curr = st.pop();
			BuildingNode* node = findBuilding(curr);

			if (!node->visited) {
				node->visited = true;
				cout << curr << " ";
			}

			// Push neighbors
			Edge* e = node->adjList;
			while (e) {
				BuildingNode* neigh = findBuilding(e->destBuildingName);
				if (!neigh->visited)
					st.push(neigh->buildingName);
				e = e->next;
			}
		}
		cout << endl;
	}
	/// O ( v^2 + VE)
	//////// Dijkstra's Algorithm for Shortest Path
void shortestPath(string src, string dest) {
		BuildingNode* s = findBuilding(src);
		BuildingNode* d = findBuilding(dest);

		if (!s || !d) {
			cout << "Invalid buildings" << endl;
			return;
		}

		// Reset
		BuildingNode* t = head;
		while (t) {
			t->distance = 999999;
			t->parent = "";
			t->visited = false;
			t = t->next;
		}

	s->distance = 0;

		// Dijkstra using manual selection
		while (true) {
			// Step 1: Find unvisited node with minimum distance
			BuildingNode* curr = nullptr;
			int minDist = 999999;

			BuildingNode* scan = head;
			while (scan) {
				if (!scan->visited && scan->distance < minDist) {
					minDist = scan->distance;
					curr = scan;
				}
				scan = scan->next;
			}

	if (!curr) break;     // no unvisited nodes left
    if (curr == d) break; // reached destination

			curr->visited = true;

			// Step 2: Relax edges
			Edge* e = curr->adjList;
			while (e) {
				BuildingNode* neigh = findBuilding(e->destBuildingName);

				// Assume weight = 1 OR based on building names etc.
				int weight = 1;

				if (curr->distance + weight < neigh->distance) {
					neigh->distance = curr->distance + weight;
					neigh->parent = curr->buildingName;
				}

				e = e->next;
			}
		}

		cout << "\nShortest path from " << src << " to " << dest << ": " << endl;

		if (d->distance >= 999999) {
			cout << "No path exists.\n";
			return;
		}

		// Reconstruct path
		string path = "";
		string curr = dest;

		while (curr != "") {
			path = curr + " -> " + path;
			curr = findBuilding(curr)->parent;
		}

		cout << path << "\n";
		cout << "Distance: " << d->distance << "\n";
	}
// Campus Map Menu        also show the paths that laready in system ( yeh bilkul alag hai from everything has no relation with other buildings and rooms )
	void campusMapMenu(Graph& g) {
		while (true) {
			cout << "===========================================================" << endl;
			cout << "Already registered Buildings and Paths:" << endl;
			cout << "Builgings: " << endl;
			cout << "Main Gate , Admin Block , CS Department , Library , Cafeteria , Auditorium " << endl;
			cout << "Paths: " << endl;
			cout << "Main Gate <-> Admin Block , road , open " << endl;
			cout << "Admin Block <->CS Department, walkway, open " << endl;
			cout << "CS Department <-> Library , road , open " << endl;
			cout << "Library <-> Auditorium , walkway , open " << endl;
			cout << "Auditorium <-> Cafeteria , road , open " << endl;
			cout << "Cafeteria <-> Main Gate , road , open " << endl;
			cout << "================================================================" << endl;
			cout << endl;
			cout << " NOTE : This module is independant of Rooms and Buldings Module " << endl;
			cout << "*-*-*-*-*-*-*-*-*-* CAMPUS MAP MENU (To test BFS  DFS DIJIKSTRA) *-*-*-*-*-*-*-*-*-*" << endl;
			cout << "1. Add Building" << endl;
			cout << "2. Remove Building" << endl;
			cout << "3. Add Path" << endl;
			cout << "4. Remove Path" << endl;
			cout << "5. BFS from Building" << endl;
			cout << "6. DFS from Building" << endl;
			cout << "7. Shortest Path (Dijkstra)" << endl;
			cout << "8. Display Adjacency List" << endl;
			cout << "9. Back" << endl;

			int ch;
			cout << "Choose: ";
			cin >> ch;
			cin.ignore();

			if (ch == 1) {
				string name;
				cout << "Enter building name: ";
				getline(cin, name);
				if (g.addBuilding(name))
					cout << "Added." << endl;
				else
					cout << "Already exists." << endl;
			}

			else if (ch == 2) {
				string name;
				cout << "Enter building name: ";
				getline(cin, name);
				g.removeBuilding(name);
			}

			else if (ch == 3) {
				string a, b, rel, stat;
				cout << "Source building: ";
				getline(cin, a);
				cout << "Destination building: ";
				getline(cin, b);
				cout << "Relation (road/walkway): ";
				getline(cin, rel);
				cout << "Status (open/closed): ";
				getline(cin, stat);

				if (g.addPath(a, b, rel, stat))
					cout << "Path added." << endl;
				else
					cout << "Failed." << endl;
			}

			else if (ch == 4) {
				string a, b;
				cout << "Source building: ";
				getline(cin, a);
				cout << "Destination building: ";
				getline(cin, b);
				g.removePath(a, b);
			}

			else if (ch == 5) {
				string start;
				cout << "Start building: ";
				getline(cin, start);
				g.BFS(start);
			}

			else if (ch == 6) {
				string start;
				cout << "Start building: ";
				getline(cin, start);
				g.DFS(start);
			}

			else if (ch == 7) {
				string a, b;
				cout << "Source: ";
				getline(cin, a);
				cout << "Destination: ";
				getline(cin, b);
				g.shortestPath(a, b);
			}

			else if (ch == 8) {
				g.display();
			}

			else if (ch == 9) {
				break;
			}
		}
	}


};

// Forward declaration of AVLNode
struct AVLNode;

struct RoomList {
	AVLNode* room;
	RoomList* next;

	RoomList(AVLNode* r) : room(r), next(nullptr) {}
};

struct AVLNode {
	string roomID;
	int floorNumber;
	string roomType; // classroom/lab/office
	bool isReserved;
	string reservedBy;
	string buildingName; // which building this room belongs to
	int height;
	AVLNode* left;
	AVLNode* right;

	AVLNode(string id, int floor, string type, string building)
		: roomID(id), floorNumber(floor), roomType(type), buildingName(building),
		isReserved(false), reservedBy(""), height(1), left(nullptr), right(nullptr) {
	}
};

class AVLTree {
private:
	AVLNode* root;
	int roomCount;

	// Helper functions
	int getHeight(AVLNode* node) {
		if (node) return node->height;
		else return  0;
	}

	int getBalanceFactor(AVLNode* node) {
		if (node) return getHeight(node->left) - getHeight(node->right);
		else return  0;
	}

	void updateHeight(AVLNode* node) {
		if (node) {
			node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		}
	}

	AVLNode* rotateRight(AVLNode* y) {
		AVLNode* x = y->left;
		AVLNode* T2 = x->right;

		x->right = y;
		y->left = T2;

		updateHeight(y);
		updateHeight(x);

		return x;
	}

	AVLNode* rotateLeft(AVLNode* x) {
		AVLNode* y = x->right;
		AVLNode* T2 = y->left;

		y->left = x;
		x->right = T2;

		updateHeight(x);
		updateHeight(y);

		return y;
	}

	AVLNode* balance(AVLNode* node) {
		if (!node) return node;

		updateHeight(node);
		int balanceFactor = getBalanceFactor(node);

		// Left Left Case
		if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
			return rotateRight(node);
		}

		// Right Right Case
		if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
			return rotateLeft(node);
		}

		// Left Right Case
		if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
			node->left = rotateLeft(node->left);
			return rotateRight(node);
		}

		// Right Left Case
		if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
			node->right = rotateRight(node->right);
			return rotateLeft(node);
		}

		return node;
	}

	AVLNode* insertHelper(AVLNode* node, AVLNode* newNode) {
		if (!node) {
			roomCount++;
			return newNode;
		}

		if (newNode->roomID < node->roomID) {
			node->left = insertHelper(node->left, newNode);
		}
		else if (newNode->roomID > node->roomID) {
			node->right = insertHelper(node->right, newNode);
		}
		else {
			// Room ID already exists
			delete newNode;
			return node;
		}

		return balance(node);
	}

	AVLNode* minValueNode(AVLNode* node) {
		AVLNode* current = node;
		while (current && current->left) {
			current = current->left;
		}
		return current;
	}

	AVLNode* deleteHelper(AVLNode* root, string roomID) {
		if (!root) return root;

		if (roomID < root->roomID) {
			root->left = deleteHelper(root->left, roomID);
		}
		else if (roomID > root->roomID) {
			root->right = deleteHelper(root->right, roomID);
		}
		else {
			// Node found
			if (!root->left || !root->right) {
				AVLNode* temp = root->left ? root->left : root->right;

				if (!temp) {
					temp = root;
					root = nullptr;
				}
				else {
					*root = *temp;
				}
				delete temp;
				roomCount--;
			}
			else {
				AVLNode* temp = minValueNode(root->right);
				root->roomID = temp->roomID;
				root->floorNumber = temp->floorNumber;
				root->roomType = temp->roomType;
				root->isReserved = temp->isReserved;
				root->reservedBy = temp->reservedBy;
				root->buildingName = temp->buildingName;
				root->right = deleteHelper(root->right, temp->roomID);
			}
		}

		if (!root) return root;

		return balance(root);
	}
	// Search for rooms reserved by a specific user
	void searchReservationsByUserHelper(AVLNode* node, string userName, int& count) {
		if (node) {
			if (node->isReserved && node->reservedBy == userName) {
				cout << "  - " << node->roomID << " in " << node->buildingName
					<< " (Floor: " << node->floorNumber << ", Type: " << node->roomType << ")\n";
				count++;
			}
			searchReservationsByUserHelper(node->left, userName, count);
			searchReservationsByUserHelper(node->right, userName, count);
		}
	}

	// Public function to search reservations by user
	int searchReservationsByUser(string userName) {
		int count = 0;
		searchReservationsByUserHelper(root, userName, count);
		return count;
	}

	AVLNode* searchHelper(AVLNode* node, string roomID) {
		if (!node || node->roomID == roomID) {
			return node;
		}

		if (roomID < node->roomID) {
			return searchHelper(node->left, roomID);
		}
		return searchHelper(node->right, roomID);
	}

	void inorderHelper(AVLNode* node) {
		if (node) {
			inorderHelper(node->left);
			cout << "Room: " << node->roomID << " | Floor: " << node->floorNumber << " | Type: " << node->roomType << " | ";
			if (node->isReserved) cout << "Reserved: Yes ( " << node->reservedBy << " )" << endl;
			else cout << "Reserved: No" << endl;
			inorderHelper(node->right);
		}
	}

	void preorderHelper(AVLNode* node) {
		if (node) {
			cout << "Room: " << node->roomID << " | Floor: " << node->floorNumber << " | Type: " << node->roomType << " | ";
			if (node->isReserved) cout << "Reserved: Yes ( " << node->reservedBy << " )" << endl;
			else cout << "Reserved: No" << endl;
			preorderHelper(node->left);
			preorderHelper(node->right);
		}
	}

	void postorderHelper(AVLNode* node) {
		if (node) {
			postorderHelper(node->left);
			postorderHelper(node->right);
			cout << "Room: " << node->roomID << " | Floor: " << node->floorNumber << " | Type: " << node->roomType << " | ";
			if (node->isReserved) cout << "Reserved: Yes ( " << node->reservedBy << " )" << endl;
			else cout << "Reserved: No" << endl;
		}
	}

	// Helper function to collect rooms by type using linked list
	void collectByTypeHelper(AVLNode* node, string type, RoomList*& head, RoomList*& tail, int& count) {
		if (node) {
			if (node->roomType == type) {
				RoomList* newRoom = new RoomList(node);
				if (!head) {
					head = tail = newRoom;
				}
				else {
					tail->next = newRoom;
					tail = newRoom;
				}
				count++;
			}
			collectByTypeHelper(node->left, type, head, tail, count);
			collectByTypeHelper(node->right, type, head, tail, count);
		}
	}

	int countLeavesHelper(AVLNode* node) {
		if (!node) return 0;
		if (!node->left && !node->right) return 1;
		return countLeavesHelper(node->left) + countLeavesHelper(node->right);
	}

	int countInternalNodesHelper(AVLNode* node) {
		if (!node) return 0;
		if (!node->left && !node->right) return 0;
		return 1 + countInternalNodesHelper(node->left) + countInternalNodesHelper(node->right);
	}

	void clearTree(AVLNode* node) {
		if (node) {
			clearTree(node->left);
			clearTree(node->right);
			delete node;
		}
	}

public:
	AVLTree() : root(nullptr), roomCount(0) {}

	~AVLTree() {
		clearTree(root);
	}

	bool insertRoom(string roomID, int floor, string type, string building) {
		if (roomID.empty() || building.empty()) return false;

		AVLNode* newNode = new AVLNode(roomID, floor, type, building);
		root = insertHelper(root, newNode);
		return true;
	}

	bool deleteRoom(string roomID) {
		if (roomID.empty()) return false;

		int oldCount = roomCount;
		root = deleteHelper(root, roomID);
		return roomCount < oldCount;
	}

	AVLNode* searchRoom(string roomID) {
		return searchHelper(root, roomID);
	}

	// Returns the number of rooms found and fills the head pointer
	int searchRoomsByType(string type, RoomList*& head) {
		RoomList* tail = nullptr;
		int count = 0;
		head = nullptr;

		collectByTypeHelper(root, type, head, tail, count);
		return count;
	}

	bool reserveRoom(string roomID, string userName) {
		AVLNode* room = searchRoom(roomID);
		if (!room || room->isReserved) {
			return false;
		}
		room->isReserved = true;
		room->reservedBy = userName;
		return true;
	}

	bool cancelReservation(string roomID) {
		AVLNode* room = searchRoom(roomID);
		if (!room || !room->isReserved) {
			return false;
		}
		room->isReserved = false;
		room->reservedBy = "";
		return true;
	}

	void printInorder() {
		cout << endl;
		cout << "************ Inorder Traversal ****************\n";
		if (!root) {
			cout << "No rooms in this building.\n";
			return;
		}
		inorderHelper(root);
	}

	void printPreorder() {
		cout << endl;
		cout << "************* Preorder Traversal ****************\n";
		if (!root) {
			cout << "No rooms in this building.\n";
			return;
		}
		preorderHelper(root);
	}

	void printPostorder() {
		cout << endl;
		cout << "************ Postorder Traversal ****************\n";
		if (!root) {
			cout << "No rooms in this building.\n";
			return;
		}
		postorderHelper(root);
	}

	int getHeight() {
		return getHeight(root);
	}

	int countLeaves() {
		return countLeavesHelper(root);
	}

	int countInternalNodes() {
		return countInternalNodesHelper(root);
	}

	int getRoomCount() {
		return roomCount;
	}

	bool isEmpty() {
		return root == nullptr;
	}
};

// BuildingManager class to manage multiple buildings and their AVL trees
class BuildingManager {
private:
	struct BuildingInfo {
		string name;
		AVLTree* roomTree;
		BuildingInfo* next;

		BuildingInfo(string n) : name(n), roomTree(new AVLTree()), next(nullptr) {}
		~BuildingInfo() { delete roomTree; }
	};

	BuildingInfo* head;

	BuildingInfo* findBuilding(string name) {
		BuildingInfo* temp = head;
		while (temp) {
			if (temp->name == name) return temp;
			temp = temp->next;
		}
		return nullptr;
	}

public:
	BuildingManager() : head(nullptr) {}

	~BuildingManager() {
		BuildingInfo* current = head;
		while (current) {
			BuildingInfo* next = current->next;
			delete current;
			current = next;
		}
	}

	bool addBuilding(string name) {
		if (findBuilding(name)) return false;

		BuildingInfo* newBuilding = new BuildingInfo(name);
		newBuilding->next = head;
		head = newBuilding;
		return true;
	}

	bool removeBuilding(string name) {
		if (!head) return false;

		BuildingInfo** ptr = &head;
		while (*ptr) {
			if ((*ptr)->name == name) {
				BuildingInfo* toDelete = *ptr;
				*ptr = (*ptr)->next;
				delete toDelete;
				return true;
			}
			ptr = &((*ptr)->next);
		}
		return false;
	}

	AVLTree* getBuildingTree(string name) {
		BuildingInfo* building = findBuilding(name);
		return building ? building->roomTree : nullptr;
	}

	void displayBuildings() {
		cout << endl;
		cout << "********************* Available Buildings **********************" << endl;
		BuildingInfo* temp = head;
		if (!temp) {
			cout << "No buildings added yet." << endl;
			return;
		}
		while (temp) {
			cout << "- " << temp->name << " ( " << temp->roomTree->getRoomCount() << " rooms )" << endl;
			temp = temp->next;
		}
	}

	// AVL Menu for Rooms & Scheduling
	void roomSchedulingMenu(HashTable& userTable) {
		while (true) {
			cout << "==============================================================" << endl;
			cout << "Already regustered buildings : " << endl;
			cout << "Main Building , CS Department , Library " << endl;
			cout << "Rooms : " << endl;
			cout << "Main BUilding : 11->classroom , 22-> classroom " << endl;
			cout << "CS Department : 33-> lab" << endl;
			cout << "Library : 44-> office " << endl;
			cout << "==============================================================" << endl;
			cout << " NOTE : Buildings must be created here to manage rooms within them. " << endl;
			cout << "\n*-*-*-*-*-*-*-*-*-* ROOMS & SCHEDULING *-*-*-*-*-*-*-*-*-*\n";
			cout << "1. Select Building\n";
			cout << "2. Insert Room\n";
			cout << "3. Delete Room\n";
			cout << "4. Search Room\n";
			cout << "5. Search Rooms by Type\n";
			cout << "6. Reserve Room\n";
			cout << "7. Cancel Reservation\n";
			cout << "8. Print Inorder Traversal\n";
			cout << "9. Print Preorder Traversal\n";
			cout << "10. Print Postorder Traversal\n";
			cout << "11. Display AVL Tree Statistics\n";
			cout << "12. Back\n";
			cout << "Choose option: ";

			int opt;
			cin >> opt;
			cin.ignore();

			if (opt == 1) {
				displayBuildings();
			}
			else if (opt >= 2 && opt <= 11) {
				// First select building for room operations
				displayBuildings();
				cout << "\nEnter building name: ";
				string buildingName;
				getline(cin, buildingName);

				AVLTree* buildingTree = getBuildingTree(buildingName);
				if (!buildingTree) {
					cout << "Building not found. Would you like to create it? (y/n): ";
					char choice;
					cin >> choice;
					cin.ignore();
					if (choice == 'y' || choice == 'Y') {
						addBuilding(buildingName);
						buildingTree = getBuildingTree(buildingName);
						cout << "Building created.\n";
					}
					else {
						continue;
					}
				}

				if (opt == 2) {
					string roomID, roomType;
					int floor;
					cout << "Enter Room ID: ";
					getline(cin, roomID);
					cout << "Enter Floor Number: ";
					cin >> floor;
					cin.ignore();
					cout << "Enter Room Type (classroom/lab/office): ";
					getline(cin, roomType);

					if (buildingTree->insertRoom(roomID, floor, roomType, buildingName)) {
						cout << "Room inserted successfully.\n";
					}
					else {
						cout << "Failed to insert room (duplicate ID or invalid input).\n";
					}
				}
				else if (opt == 3) {
					string roomID;
					cout << "Enter Room ID to delete: ";
					getline(cin, roomID);

					if (buildingTree->deleteRoom(roomID)) {
						cout << "Room deleted successfully.\n";
					}
					else {
						cout << "Room not found.\n";
					}
				}
				else if (opt == 4) {
					string roomID;
					cout << "Enter Room ID to search: ";
					getline(cin, roomID);

					AVLNode* room = buildingTree->searchRoom(roomID);
					if (room) {
						cout << endl;
						cout << "**************** Room Found ***************\n";
						cout << "Room ID: " << room->roomID << endl;
						cout << "Building: " << room->buildingName << endl;
						cout << "Floor: " << room->floorNumber << endl;
						cout << "Type: " << room->roomType << endl;
						cout << "Status: " << (room->isReserved ?
							"Reserved by " + room->reservedBy : "Available") << endl;
					}
					else {
						cout << "Room not found.\n";
					}
				}
				else if (opt == 5) {
					string roomType;
					cout << "Enter room type to search (classroom/lab/office): ";
					getline(cin, roomType);

					RoomList* rooms = nullptr;
					int count = buildingTree->searchRoomsByType(roomType, rooms);

					if (count == 0) {
						cout << "No rooms of type '" << roomType << "' found.\n";
					}
					else {
						cout << "************** Rooms of type " << roomType << " **************\n";
						RoomList* current = rooms;
						while (current) {
							AVLNode* room = current->room;
							cout << "Room: " << room->roomID << " | Floor: " << room->floorNumber
								<< " | Reserved: ";
							if (room->isReserved) cout << "Yes" << endl;
							else cout << "No" << endl;
							current = current->next;
						}
						cout << "Total: " << count << " rooms\n";
					}

					// Clean up the room list
					RoomList* current = rooms;
					while (current) {
						RoomList* temp = current;
						current = current->next;
						delete temp;
					}
				}
				else if (opt == 6) {
					string roomID, userName;
					cout << "Enter Room ID to reserve: ";
					getline(cin, roomID);
					cout << "Enter your username: ";
					getline(cin, userName);

					// Check if user exists
					if (!userTable.searchUser(userName)) {
						cout << "User not found. Please register first.\n";
						continue;
					}

					if (buildingTree->reserveRoom(roomID, userName)) {
						cout << "Room reserved successfully.\n";
						// Update last booked room in hash table
						userTable.updateLastBookedRoom(userName, roomID);
					}
					else {
						cout << "Failed to reserve room (room not found or already reserved).\n";
					}
				}
				else if (opt == 7) {
					string roomID;
					cout << "Enter Room ID to cancel reservation: ";
					getline(cin, roomID);

					if (buildingTree->cancelReservation(roomID)) {
						cout << "Reservation cancelled successfully.\n";
					}
					else {
						cout << "Failed to cancel reservation (room not found or not reserved).\n";
					}
				}
				else if (opt == 8) {
					buildingTree->printInorder();
				}
				else if (opt == 9) {
					buildingTree->printPreorder();
				}
				else if (opt == 10) {
					buildingTree->printPostorder();
				}
				else if (opt == 11) {
					cout << "\n--- AVL Tree Statistics ---\n";
					cout << "Total Rooms: " << buildingTree->getRoomCount() << endl;
					cout << "Tree Height: " << buildingTree->getHeight() << endl;
					cout << "Leaf Nodes: " << buildingTree->countLeaves() << endl;
					cout << "Internal Nodes: " << buildingTree->countInternalNodes() << endl;
					cout << "Is Empty: ";
					if (buildingTree->isEmpty()) cout << "Yes" << endl;
					else cout << "No" << endl;
				}
			}
			else if (opt == 12) {
				break;
			}
			else {
				cout << "Invalid option. Try again.\n";
			}
		}
	}
};

//<---------------------------------- MODULE D ------------------------------------->

struct ComplaintNode {
	int ticketID;
	string raisedBy;
	string building;
	string roomID;
	string description;
	string timestamp;
	ComplaintNode* next;

	ComplaintNode(int id, string user, string bldg, string room,
		string desc, string time)
		: ticketID(id), raisedBy(user), building(bldg), roomID(room),
		description(desc), timestamp(time), next(nullptr) {
	}
};

class ComplaintQueue {
private:
	ComplaintNode* front;
	ComplaintNode* rear;
	int nextTicketID;
	int totalComplaints;
	int processedComplaints;

public:
	ComplaintQueue() : front(nullptr), rear(nullptr),
		nextTicketID(1001), totalComplaints(0),
		processedComplaints(0) {
	}

	~ComplaintQueue() {
		while (front) {
			ComplaintNode* temp = front;
			front = front->next;
			delete temp;
		}
	}

	bool isEmpty() {
		return front == nullptr;
	}

	void enqueue(string user, string building, string room, string desc, string time) {
		ComplaintNode* newNode = new ComplaintNode(nextTicketID, user, building, room, desc, time);
		nextTicketID++;
		totalComplaints++;

		if (!rear) {
			front = rear = newNode;
		}
		else {
			rear->next = newNode;
			rear = newNode;
		}

		cout << "Complaint submitted successfully. Ticket ID: " << newNode->ticketID << endl;
	}

	ComplaintNode* dequeue() {
		if (isEmpty()) {
			return nullptr;
		}

		ComplaintNode* temp = front;
		front = front->next;

		if (!front) {
			rear = nullptr;
		}

		temp->next = nullptr;
		processedComplaints++;
		return temp;
	}

	ComplaintNode* peek() {
		return front;
	}

	void displayQueue() {
		if (isEmpty()) {
			cout << "No pending complaints.\n";
			return;
		}

		cout << "\n--- Pending Complaints ---\n";
		ComplaintNode* current = front;
		while (current) {
			cout << "Ticket #" << current->ticketID << " | By: " << current->raisedBy
				<< " | Building: " << current->building << " | Room: " << current->roomID
				<< "\nDescription: " << current->description
				<< "\nTime: " << current->timestamp << "\n" << endl;
			current = current->next;
		}
	}

	void displayStatistics() {
		cout << endl;
		cout << "**************** Complaint Statistics ********************\n";
		cout << "Total Complaints Submitted: " << totalComplaints << endl;
		cout << "Processed Complaints: " << processedComplaints << endl;
		cout << "Pending Complaints: " << (totalComplaints - processedComplaints) << endl;
		cout << "Next Ticket ID: " << nextTicketID << endl;
	}

	// Complaint system menu
	void complaintSystemMenu(HashTable& userTable) {
		while (true) {
			cout << "\n*-*-*-*-*-*-*-*-*-* COMPLAINT SYSTEM *-*-*-*-*-*-*-*-*-*\n";
			cout << "1. Submit New Complaint\n";
			cout << "2. Process Next Complaint\n";
			cout << "3. View Next Complaint\n";
			cout << "4. Display All Pending Complaints\n";
			cout << "5. Complaint Statistics\n";
			cout << "6. Back\n";
			cout << "Choose option: ";

			int opt;
			cin >> opt;
			cin.ignore();

			if (opt == 1) {
				string user, building, room, desc, time;

				cout << "Enter your username: ";
				getline(cin, user);

				// Validate user
				if (!userTable.searchUser(user)) {
					cout << "User not found. Please register first.\n";
					continue;
				}

				cout << "Enter building name: ";
				getline(cin, building);
				cout << "Enter room ID (or 'none'): ";
				getline(cin, room);
				cout << "Enter complaint description: ";
				getline(cin, desc);
				cout << "Enter timestamp (e.g., 2024-12-09 14:30): ";
				getline(cin, time);

				enqueue(user, building, room, desc, time);
			}
			else if (opt == 2) {
				ComplaintNode* processed = dequeue();
				if (processed) {
					cout << endl;
					cout << "******************* Processing Complaint ******************\n";
					cout << "Ticket #" << processed->ticketID << " processed.\n";
					cout << "User: " << processed->raisedBy << endl;
					cout << "Issue: " << processed->description << endl;
					cout << "Sending maintenance team to " << processed->building;
					if (!processed->roomID.empty() && processed->roomID != "none") {
						cout << ", Room " << processed->roomID;
					}
					cout << endl;

					delete processed;
				}
				else {
					cout << "No complaints to process.\n";
				}
			}
			else if (opt == 3) {
				ComplaintNode* nextComplaint = peek();
				if (nextComplaint) {
					cout << endl;
					cout << "******************* Next Complaint in Queue ********************\n";
					cout << "Ticket ID: " << nextComplaint->ticketID << endl;
					cout << "Raised by: " << nextComplaint->raisedBy << endl;
					cout << "Location: " << nextComplaint->building;
					if (!nextComplaint->roomID.empty() && nextComplaint->roomID != "none") {
						cout << " (Room: " << nextComplaint->roomID << ")";
					}
					cout << endl;
					cout << "Description: " << nextComplaint->description << endl;
					cout << "Submitted: " << nextComplaint->timestamp << endl;
				}
				else {
					cout << "No pending complaints.\n";
				}
			}
			else if (opt == 4) {
				displayQueue();
			}
			else if (opt == 5) {
				displayStatistics();
			}
			else if (opt == 6) {
				break;
			}
			else {
				cout << "Invalid option. Try again.\n";
			}
		}
	}
};

//<---------------------------------- MODULE E------------------------------------->

struct MessageNode {
	string fromUser;
	string toUser;
	string messageText;
	string timestamp;
	MessageNode* next;

	MessageNode(string from, string to, string msg, string time) : fromUser(from), toUser(to), messageText(msg), timestamp(time), next(nullptr) {
	}
};

struct Conversation {
	string friendName;
	MessageNode* messageStack; // Stack of messages (most recent on top)
	Conversation* next;

	Conversation(string friendName)
		: friendName(friendName), messageStack(nullptr), next(nullptr) {
	}
};

class MessageSystem {
private:
	struct UserInbox {
		string userName;
		Conversation* conversations; // Linked list of conversations
		UserInbox* next;

		UserInbox(string user) : userName(user), conversations(nullptr), next(nullptr) {}

		~UserInbox() {
			Conversation* currConv = conversations;
			while (currConv) {
				Conversation* nextConv = currConv->next;

				// Delete all messages in this conversation stack
				while (currConv->messageStack) {
					MessageNode* temp = currConv->messageStack;
					currConv->messageStack = currConv->messageStack->next;
					delete temp;
				}

				delete currConv;
				currConv = nextConv;
			}
		}
	};

	UserInbox* inboxHead;

	UserInbox* findUserInbox(string userName) {
		UserInbox* temp = inboxHead;
		while (temp) {
			if (temp->userName == userName) return temp;
			temp = temp->next;
		}
		return nullptr;
	}

	Conversation* findConversation(UserInbox* inbox, string friendName) {
		Conversation* conv = inbox->conversations;
		while (conv) {
			if (conv->friendName == friendName) return conv;
			conv = conv->next;
		}
		return nullptr;
	}

	Conversation* createConversation(UserInbox* inbox, string friendName) {
		Conversation* newConv = new Conversation(friendName);
		newConv->next = inbox->conversations;
		inbox->conversations = newConv;
		return newConv;
	}

	void ensureUserInbox(string userName) {
		if (!findUserInbox(userName)) {
			UserInbox* newInbox = new UserInbox(userName);
			newInbox->next = inboxHead;
			inboxHead = newInbox;
		}
	}

public:
	MessageSystem() : inboxHead(nullptr) {}

	~MessageSystem() {
		UserInbox* current = inboxHead;
		while (current) {
			UserInbox* next = current->next;
			delete current;
			current = next;
		}
	}

	bool sendMessage(string fromUser, string toUser, string message, string timestamp) {
		if (fromUser == toUser) {
			cout << "Cannot send message to yourself." << endl;
			return false;
		}

		// Ensure both users have inboxes
		ensureUserInbox(fromUser);
		ensureUserInbox(toUser);

		// Create message node
		MessageNode* newMsg = new MessageNode(fromUser, toUser, message, timestamp);

		// Add to sender's conversation with receiver
		UserInbox* senderInbox = findUserInbox(fromUser);
		Conversation* senderConv = findConversation(senderInbox, toUser);
		if (!senderConv) {
			senderConv = createConversation(senderInbox, toUser);
		}
		// Push to stack (LIFO)
		newMsg->next = senderConv->messageStack;
		senderConv->messageStack = newMsg;

		// Add to receiver's conversation with sender (copy of message)
		MessageNode* receiverMsg = new MessageNode(fromUser, toUser, message, timestamp);
		UserInbox* receiverInbox = findUserInbox(toUser);
		Conversation* receiverConv = findConversation(receiverInbox, fromUser);
		if (!receiverConv) {
			receiverConv = createConversation(receiverInbox, fromUser);
		}
		receiverMsg->next = receiverConv->messageStack;
		receiverConv->messageStack = receiverMsg;

		cout << "Message sent successfully." << endl;
		return true;
	}

	MessageNode* viewLatestMessage(string user, string friendName) {
		UserInbox* inbox = findUserInbox(user);
		if (!inbox) {
			cout << "User not found in messaging system." << endl;
			return nullptr;
		}

		Conversation* conv = findConversation(inbox, friendName);
		if (!conv || !conv->messageStack) {
			cout << "No messages with " << friendName << endl;
			return nullptr;
		}

		return conv->messageStack;
	}

	MessageNode* popLatestMessage(string user, string friendName) {
		UserInbox* inbox = findUserInbox(user);
		if (!inbox) return nullptr;

		Conversation* conv = findConversation(inbox, friendName);
		if (!conv || !conv->messageStack) return nullptr;

		MessageNode* topMsg = conv->messageStack;
		conv->messageStack = topMsg->next;
		topMsg->next = nullptr;

		return topMsg;
	}

	void displayConversation(string user1, string user2) {
		UserInbox* inbox = findUserInbox(user1);
		if (!inbox) {
			cout << "User " << user1 << " not found." << endl;
			return;
		}

		Conversation* conv = findConversation(inbox, user2);
		if (!conv || !conv->messageStack) {
			cout << "No conversation found between " << user1 << " and " << user2 << endl;
			return;
		}
	cout << endl;
		cout << "***************** Conversation between " << user1 << " and " << user2 << " *******************" << endl;

		MessageNode* current = conv->messageStack;
int count = 0;
		while (current && count < 20) { // Show last 20 messages max
			cout << endl;
			cout << "[ " << current->timestamp << " ]" << endl;
			cout << current->fromUser << " -> " << current->toUser << " : ";
			cout << current->messageText << endl;
			current = current->next;
			count++;
		}

		if (current) {
			cout << "... (more messages)" << endl;
		}
	}

	// Messaging system menu
	void messagingSystemMenu(HashTable& userTable) {
		while (true) {
			cout << endl;
			cout << "*-*-*-*-*-*-*-*-*-* MESSAGING SYSTEM *-*-*-*-*-*-*-*-*-*" << endl;
			cout << "1. Send Message to User" << endl;
			cout << "2. View Latest Message in Conversation" << endl;
			cout << "3. Pop Latest Message" << endl;
			cout << "4. Display Conversation" << endl;
			cout << "5. Back" << endl;
			cout << "Choose option: ";

			int opt;
			cin >> opt;
			cin.ignore();

			if (opt == 1) {
				string fromUser, toUser, message, timestamp;

				cout << "Enter your username: ";
				getline(cin, fromUser);

				// Validate sender exists
				if (!userTable.searchUser(fromUser)) {
					cout << "Sender not found. Please register first." << endl;
					continue;
				}

				cout << "Enter recipient username: ";
				getline(cin, toUser);

				// Validate recipient exists
				if (!userTable.searchUser(toUser)) {
					cout << "Recipient not found." << endl;
					continue;
				}

				cout << "Enter message: ";
				getline(cin, message);

				cout << "Enter timestamp (e.g. 2024-12-09 14:30): ";
				getline(cin, timestamp);

				sendMessage(fromUser, toUser, message, timestamp);
			}
			else if (opt == 2) {
				string user, friendName;
				cout << "Enter your username : ";
				getline(cin, user);
				cout << "Enter friend's username : ";
				getline(cin, friendName);

				MessageNode* latest = viewLatestMessage(user, friendName);
				if (latest) {
					cout << endl;
					cout << "************ Latest Message ************\n";
					cout << "From : " << latest->fromUser << endl;
					cout << "To : " << latest->toUser << endl;
					cout << "Time : " << latest->timestamp << endl;
					cout << "Message : " << latest->messageText << endl;
				}
			}
			else if (opt == 3) {
				string user, friendName;
				cout << "Enter your username : ";
				getline(cin, user);
				cout << "Enter friend's username : ";
				getline(cin, friendName);

				MessageNode* popped = popLatestMessage(user, friendName);
				if (popped) {
					cout << endl;
					cout << "*********** Popped Message *************" << endl;
					cout << "Message removed from stack : " << endl;
					cout << "From : " << popped->fromUser << " | To : " << popped->toUser << endl;
					cout << "Time : " << popped->timestamp << endl;
					cout << "Content : " << popped->messageText << endl;
					delete popped;
				}
				else {
					cout << "No messages to pop." << endl;
				}
			}
			else if (opt == 4) {
				string user1, user2;
				cout << "Enter first username : ";
				getline(cin, user1);
				cout << "Enter second username : ";
				getline(cin, user2);

				displayConversation(user1, user2);
			}
			else if (opt == 5) {
				break;
			}
			else {
				cout << "Invalid option. Try again." << endl;
			}
		}
	}
};
//<---------------------------------- MAIN FUNCTION WITH MENU ------------------------------------->

int main() {
	HashTable userDirectory;
	Graph campusMap;
	BuildingManager roomManager;
	ComplaintQueue complaintSystem;
	MessageSystem messagingSystem;

	// Add some initial data for testing

	/// username    password    role        department          email
userDirectory.insertUser("Ayesha Masood", "ayesha123", "admin", "Administration", "ayesha@nu.edu.pk");
userDirectory.insertUser("Ayesha", "ayesha123", "student", "Computer Science", "ayesha@nu.edu.pk");
userDirectory.insertUser("Areeba", "areeba123", "teacher", "Software Engineering", "areeba@nu.edu.pk");

	// Add some initial buildings to reserve rooms 
	roomManager.addBuilding("Main Building");
	roomManager.addBuilding("CS Department");
	roomManager.addBuilding("Library");

	// Add some rooms for testing
	/// ID     floor   type        building
	roomManager.getBuildingTree("Main Building")->insertRoom("11", 1, "classroom", "Main Building");
	roomManager.getBuildingTree("Main Building")->insertRoom("22", 1, "classroom", "Main Building");
	roomManager.getBuildingTree("CS Department")->insertRoom("33", 2, "lab", "CS Department");
	roomManager.getBuildingTree("Library")->insertRoom("44", 0, "office", "Library");

	// Add some initial campus buildings and paths  
	campusMap.addBuilding("Main Gate");
	campusMap.addBuilding("Admin Block");
	campusMap.addBuilding("CS Department");
	campusMap.addBuilding("Library");
	campusMap.addBuilding("Auditorium");
	campusMap.addBuilding("Cafeteria");

	campusMap.addPath("Main Gate", "Admin Block", "road", "open");
	campusMap.addPath("Admin Block", "CS Department", "walkway", "open");
	campusMap.addPath("CS Department", "Library", "road", "open");
	campusMap.addPath("Library", "Auditorium", "walkway", "open");
	campusMap.addPath("Auditorium", "Cafeteria", "road", "open");
	campusMap.addPath("Cafeteria", "Main Gate", "road", "open");
	cout << endl;

	///////// welcome page 
	cout << "=========================================================" << endl;
	cout << "     *  WELCOME TO UNIVERSITY MANAGEMENT SYSTEM  *" << endl;
	cout << endl;
	cout << "                   Developed by: "<<endl;
	cout << "            Areeba Fatima 24F-0619 (^-^)" << endl;
	cout << "            Ayesha Yasin  24F-0565 (^-^)" << endl;
	cout << endl;
	cout << "  National University of Computer & Emerging Sciences" << endl;
	cout << "          Faisalabad - Chiniot Campus" << endl;
	cout << "==========================================================" << endl;
	cout << endl;
	cout << "Already registered people:" << endl;
	cout << "Admin : Username: Ayesha Masood Password: ayesha123" << endl;
	cout << "Teacher : Username: Areeba      Password: areeba123" << endl;
	cout << "Student : Username: Ayesha      Password: ayesha123" << endl;
	cout << "==========================================================" << endl;
	cout << endl;
	string currentUser = "";
	string currentRole = "";

	// Login loop
	while (currentUser.empty()) {
		cout << endl;
		cout << "Please login to continue:\n";
		cout << "1. Login\n";
		cout << "2. Register\n";
		cout << "3. Exit System\n";
		cout << "Choose option: ";

		int loginChoice;
		cin >> loginChoice;
		cin.ignore();

		if (loginChoice == 1) {
			string username, password;
			cout << "Username: ";
			getline(cin, username);
			cout << "Password: ";
			getline(cin, password);

			if (userDirectory.login(username, password)) {
				currentUser = username;
				Person* user = userDirectory.searchUser(username);
				currentRole = user->role;
				/// welcome message for user ;)
				cout << "\n==============================================================\n";
				cout << "   Welcome  " << username << " (" << currentRole << ") (^-^)\n";
				cout << "================================================================\n";
			}
			else {
				cout << "Login failed. Invalid username or password.\n";
			}
		}
		else if (loginChoice == 2) {
			string user, pass, role, dept, email;
			cout << "Enter Username: ";
			getline(cin, user);
			cout << "Enter Password: ";
			getline(cin, pass);
			cout << "Enter Role (student/teacher/admin): ";
			getline(cin, role);
			cout << "Enter Department: ";
			getline(cin, dept);
			cout << "Enter Email: ";
			getline(cin, email);

			if (userDirectory.insertUser(user, pass, role, dept, email)) {
				cout << "Registration successful! Please login.\n";
			}
			else {
				cout << "Registration failed. Username may already exist.\n";
			}
		}
		else if (loginChoice == 3) {
			cout << endl;
			cout << "=======================================================" << endl;
			cout << "   Thank you for using University Management System" << endl;
			cout << "                 Goodbye! (^-^)\n";
			cout << endl;
			cout << "         Regards Develpors of this System" << endl;
			cout << "             Areeba Fatima (24F-0619)" << endl;
			cout << "             Ayesha Yasin  (24F-0565)" << endl;
			cout << "=======================================================" << endl;
			return 0;
		}
		else {
			cout << "Invalid choice. Try again.\n";
		}
	}

	// Main system loop after login
	while (true) {
		cout << "\n*-*-*-*-*-*-*-*-*-* MAIN MENU *-*-*-*-*-*-*-*-*-*\n";
		cout << "Logged in as: " << currentUser << " (" << currentRole << ")\n";
		cout << "-----------------------------------------------\n";
		cout << "1. User Management\n";
		cout << "2. Campus Navigation\n";
		cout << "3. Room Booking System\n";
		cout << "4. Complaint System\n";
		cout << "5. Messaging System\n";
		cout << "6. View My Profile\n";

		if (currentRole == "admin") {
			cout << "7. Admin Dashboard\n";
			cout << "8. Logout\n";
			cout << "9. Exit System\n";
		}
		else {
			cout << "7. Logout\n";
			cout << "8. Exit System\n";
		}

		cout << "Choose option: ";

		int mainChoice;
		cin >> mainChoice;
		cin.ignore();

		if (mainChoice == 1) {
			// User Management (Hash Table)
			cout << "\n";
			userDirectory.userDirectoryMenu(userDirectory);
		}
		else if (mainChoice == 2) {
			// Campus Navigation (Graph)
			cout << "\n";
			campusMap.campusMapMenu(campusMap);
		}
		else if (mainChoice == 3) {
			// Room Booking System (AVL Tree)
			cout << "\n";
			roomManager.roomSchedulingMenu(userDirectory);
		}
		else if (mainChoice == 4) {
			// Complaint System (Queue)
			cout << "\n";
			complaintSystem.complaintSystemMenu(userDirectory);
		}
		else if (mainChoice == 5) {
			// Messaging System (Stack)
			cout << "\n";
			messagingSystem.messagingSystemMenu(userDirectory);
		}
		else if (mainChoice == 6) {
			// View My Profile
			cout << "\n";
			userDirectory.printUserProfile(currentUser);


		}
		else if (mainChoice == 7) {
			if (currentRole == "admin") {
				// Admin Dashboard
				cout << "\n*-*-*-*-*-*-*-*-*-* ADMIN DASHBOARD *-*-*-*-*-*-*-*-*-*\n";
				cout << "1. System Statistics\n";
				cout << "2. View All Users\n";
				cout << "3. Manage Buildings\n";
				cout << "4. View All Complaints\n";
				cout << "5. Back to Main Menu\n";
				cout << "Choose option: ";

				int adminChoice;
				cin >> adminChoice;
				cin.ignore();

				if (adminChoice == 1) {
					cout << endl;
					cout << "********** SYSTEM STATISTICS *********\n";
					cout << endl;
					cout << "User Directory:\n";
					userDirectory.displayAll();

					cout << "\nBuildings with Rooms:\n";
					roomManager.displayBuildings();

					cout << "\nComplaint Statistics:\n";
					complaintSystem.displayStatistics();
				}
				else if (adminChoice == 2) {
					userDirectory.displayAll();
				}
				else if (adminChoice == 3) {
					cout << endl;
					cout << "******** BUILDING MANAGEMENT ***********\n";
					roomManager.displayBuildings();

					cout << "\n1. Add New Building\n";
					cout << "2. Remove Building\n";
					cout << "3. Back\n";
					cout << "Choose: ";

					int buildingChoice;
					cin >> buildingChoice;
					cin.ignore();

					if (buildingChoice == 1) {
						string buildingName;
						cout << "Enter new building name: ";
						getline(cin, buildingName);
						if (roomManager.addBuilding(buildingName)) {
							cout << "Building added successfully.\n";
							// Also add to campus map
							campusMap.addBuilding(buildingName);
							cout << "Building also added to campus map.\n";
						}
						else {
							cout << "Building already exists.\n";
						}
					}
					else if (buildingChoice == 2) {
						string buildingName;
						cout << "Enter building name to remove: ";
						getline(cin, buildingName);
						if (roomManager.removeBuilding(buildingName)) {
							cout << "Building removed from room system.\n";
							// Also remove from campus map
							campusMap.removeBuilding(buildingName);
							cout << "Building also removed from campus map.\n";
						}
						else {
							cout << "Building not found.\n";
						}
					}
				}
				else if (adminChoice == 4) {
					complaintSystem.displayQueue();
				}
				else if (adminChoice == 5) {
					// Back to main menu
					continue;
				}
			}
			else {
				// Logout for non-admin users
				cout << "Logging out...\n";
				currentUser = "";
				currentRole = "";

				// Go back to login screen
				while (currentUser.empty()) {
					cout << "\nPlease login to continue:\n";
					cout << "1. Login\n";
					cout << "2. Register\n";
					cout << "3. Exit System\n";
					cout << "Choose option: ";

					int loginChoice;
					cin >> loginChoice;
					cin.ignore();

					if (loginChoice == 1) {
						string username, password;
						cout << "Username: ";
						getline(cin, username);
						cout << "Password: ";
						getline(cin, password);

						if (userDirectory.login(username, password)) {
							currentUser = username;
							Person* user = userDirectory.searchUser(username);
							currentRole = user->role;
							cout << "\nWelcome back, " << username << "!\n";
						}
						else {
							cout << "Login failed.\n";
						}
					}
					else if (loginChoice == 2) {
						string user, pass, role, dept, email;
						cout << "Enter Username: ";
						getline(cin, user);
						cout << "Enter Password: ";
						getline(cin, pass);
						cout << "Enter Role (Student/Teacher/Admin): ";
						getline(cin, role);
						cout << "Enter Department: ";
						getline(cin, dept);
						cout << "Enter Email: ";
						getline(cin, email);

						if (userDirectory.insertUser(user, pass, role, dept, email)) {
							cout << "Registration successful! Please login.\n";
						}
						else {
							cout << "Registration failed.\n";
						}
					}
					else if (loginChoice == 3) {
						cout << endl;
						cout << "=======================================================" << endl;
						cout << "   Thank you for using University Management System" << endl;
						cout << "                 Goodbye! (^-^)\n";
						cout << endl;
						cout << "         Regards Develpors of this System" << endl;
						cout << "             Areeba Fatima (24F-0619)" << endl;
						cout << "             Ayesha Yasin  (24F-0565)" << endl;
						cout << "=======================================================" << endl;
						return 0;
					}
				}
			}
		}
		else if ((currentRole == "admin" && mainChoice == 8) ||
			(currentRole != "admin" && mainChoice == 7)) {
			// Logout
			cout << "Logging out...\n";
			currentUser = "";
			currentRole = "";

			// Go back to login screen
			while (currentUser.empty()) {
				cout << "\nPlease login to continue:\n";
				cout << "1. Login\n";
				cout << "2. Register\n";
				cout << "3. Exit System\n";
				cout << "Choose option: ";

				int loginChoice;
				cin >> loginChoice;
				cin.ignore();

				if (loginChoice == 1) {
					string username, password;
					cout << "Username: ";
					getline(cin, username);
					cout << "Password: ";
					getline(cin, password);

					if (userDirectory.login(username, password)) {
						currentUser = username;
						Person* user = userDirectory.searchUser(username);
						currentRole = user->role;
						cout << "\nWelcome back, " << username << "!\n";
					}
					else {
						cout << "Login failed.\n";
					}
				}
				// to register new one 
				else if (loginChoice == 2) {
					string user, pass, role, dept, email;
					cout << "Enter Username: ";
					getline(cin, user);
					cout << "Enter Password: ";
					getline(cin, pass);
					cout << "Enter Role (Student/Teacher/Admin): ";
					getline(cin, role);
					cout << "Enter Department: ";
					getline(cin, dept);
					cout << "Enter Email: ";
					getline(cin, email);

					if (userDirectory.insertUser(user, pass, role, dept, email)) {
						cout << "Registration successful! Please login.\n";
					}
					else {
						cout << "Registration failed.\n";
					}
				}
				else if (loginChoice == 3) {
					cout << endl;
					cout << "=======================================================" << endl;
					cout << "   Thank you for using University Management System" << endl;
					cout << "                 Goodbye! (^-^)\n";
					cout << endl;
					cout << "         Regards Develpors of this System" << endl;
					cout << "             Areeba Fatima (24F-0619)" << endl;
					cout << "             Ayesha Yasin  (24F-0565)" << endl;
					cout << "=======================================================" << endl;
					return 0;
				}
			}
		}
		else if ((currentRole == "admin" && mainChoice == 9) ||
			(currentRole != "admin" && mainChoice == 8)) {
			// Good byeeeeeeeeeeeeeeeeeeeee
			cout << endl;
			cout << "======================================================="<<endl;
			cout << "   Thank you for using University Management System" << endl;
			cout << "                 Goodbye! (^-^)\n";
			cout << endl;
			cout << "         Regards Develpors of this System" << endl;
			cout << "             Areeba Fatima (24F-0619)" << endl;
			cout << "             Ayesha Yasin  (24F-0565)" << endl;
			cout << "=======================================================" << endl;
			break;
		}
		else {
			cout << "Invalid choice. Please try again."<<endl;
		}
	}

	return 0;
}