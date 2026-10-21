#include <iostream>
#include <string>

using namespace std;

struct ActionNode {
    int actionID;
    string actionType;   
    string targetRecord; 
    ActionNode* next;

    ActionNode(int id, string type, string record, ActionNode* n) {
        actionID = id;
        actionType = type;
        targetRecord = record;
        next = n;
    }
};

class UndoStack {
private:
    ActionNode* top;
    int count;

public:
    UndoStack() : top(NULL), count(0) {}

    UndoStack(const UndoStack& other) : top(NULL), count(other.count) {
        if (other.top == NULL) return;
        
        top = new ActionNode(other.top->actionID, other.top->actionType, other.top->targetRecord, NULL);
        ActionNode* current = top;
        ActionNode* otherCurrent = other.top->next;
        
        while (otherCurrent != NULL) {
            current->next = new ActionNode(otherCurrent->actionID, otherCurrent->actionType, otherCurrent->targetRecord, NULL);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }

    UndoStack& operator=(const UndoStack& other) {
        if (this == &other) return *this;

        while (!IsEmpty()) {
            ActionNode* temp = Pop();
            delete temp;
        }

        count = other.count;
        if (other.top != NULL) {
            top = new ActionNode(other.top->actionID, other.top->actionType, other.top->targetRecord, NULL);
            ActionNode* current = top;
            ActionNode* otherCurrent = other.top->next;
            
            while (otherCurrent != NULL) {
                current->next = new ActionNode(otherCurrent->actionID, otherCurrent->actionType, otherCurrent->targetRecord, NULL);
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
        return *this;
    }

    ~UndoStack() {
        while (!IsEmpty()) {
            ActionNode* temp = Pop();
            delete temp;
        }
    }

    void Push(ActionNode* action) {
        if (action == NULL) return;
        action->next = top;
        top = action;
        count++;
    }

    ActionNode* Pop() {
        if (IsEmpty()) {
            return NULL;
        }
        ActionNode* temp = top;
        top = top->next;
        temp->next = NULL; 
        count--;
        return temp;
    }

    ActionNode* GetTopValue() {
        return top;
    }

    bool IsEmpty() {
        return top == NULL;
    }

    bool IsNotEmpty() {
        return top != NULL;
    }

    void PrintStack() {
        if (IsEmpty()) {
            cout << "No actions in the undo history." << endl;
            return;
        }
        
        ActionNode* current = top;
        cout << "--- Admin Action History (Most Recent First) ---" << endl;
        while (current != NULL) {
            cout << "Action ID: " << current->actionID 
                 << " | Type: " << current->actionType 
                 << " | Record: " << current->targetRecord << endl;
            current = current->next;
        }
        cout << "------------------------------------------------" << endl;
    }
};

int main() {
    cout << "========================================================\n";
    cout << "Assignment: Lab 02 - Stack (Part B: Individual Project)\n";
    cout << "Submitted by: abdullah faisal (Roll: Ai 23, Section: A)\n";
    cout << "Group Members: abdullah\n";
    cout << "Instructor: sir Salman \n";
    cout << "Project Context: Library Management System (SQLite/Crow)\n";
    cout << "========================================================\n\n";

    UndoStack adminHistory;

    cout << "[System] Admin performs sequence of database operations...\n";
    adminHistory.Push(new ActionNode(1, "INSERT", "Book: Digital Logic", NULL));
    adminHistory.Push(new ActionNode(2, "DELETE", "Book: Outdated Physics", NULL));
    adminHistory.Push(new ActionNode(3, "UPDATE", "Book: Intro to C++ (Qty: 5)", NULL));

    adminHistory.PrintStack();

    cout << "\n[System] Admin clicks 'Undo' to revert the last mistake...\n";
    ActionNode* undoneAction = adminHistory.Pop();
    if (undoneAction != NULL) {
        cout << "--> Successfully Undid Action: " << undoneAction->actionType 
             << " on " << undoneAction->targetRecord << endl;
        delete undoneAction; 
    }

    cout << "\n[System] Current status of pending actions:\n";
    adminHistory.PrintStack();

    if (adminHistory.IsNotEmpty()) {
        cout << "\n[System] Next action available to undo: " 
             << adminHistory.GetTopValue()->actionType 
             << " on " << adminHistory.GetTopValue()->targetRecord << endl;
    }

    return 0;
}
