#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Task structure to store task information
struct Task {
    string description;
    bool completed;
};

// Function to add a task to the list
void addTask(vector<Task>& tasks, const string& description) {
    Task newTask = {description, false};
    tasks.push_back(newTask);
    cout << "Task added: " << description << endl;
}

// Function to view tasks
void viewTasks(const vector<Task>& tasks) {
    if (tasks.empty()) {
        cout << "No tasks to display." << endl;
        return;
    }

    cout << "Tasks:" << endl;
    for (size_t i = 0; i < tasks.size(); i++) {
        cout << i + 1 << ". " << (tasks[i].completed ? "[X] " : "[ ] ") << tasks[i].description << endl;
    }
}

// Function to mark a task as completed
void markCompleted(vector<Task>& tasks, int index) {
    if (index >= 1 && index <= tasks.size()) {
        tasks[index - 1].completed = true;
        cout << "Task marked as completed." << endl;
    } else {
        cout << "Invalid task index." << endl;
    }
}

// Function to remove a task
void removeTask(vector<Task>& tasks, int index) {
    if (index >= 1 && index <= tasks.size()) {
        tasks.erase(tasks.begin() + index - 1);
        cout << "Task removed." << endl;
    } else {
        cout << "Invalid task index." << endl;
    }
}

int main() {
    vector<Task> tasks;
    string command;

    while (true) {
        cout << "Commands: add, view, complete, remove, quit" << endl;
        cout << "Enter a command: ";
        cin >> command;

        if (command == "add") {
            cin.ignore(); // Clear the newline character from the buffer
            string description;
            cout << "Enter the task description: ";
            getline(cin, description);
            addTask(tasks, description);
        } else if (command == "view") {
            viewTasks(tasks);
        } else if (command == "complete") {
            int index;
            cout << "Enter the task index to mark as completed: ";
            cin >> index;
            markCompleted(tasks, index);
        } else if (command == "remove") {
            int index;
            cout << "Enter the task index to remove: ";
            cin >> index;
            removeTask(tasks, index);
        } else if (command == "quit") {
            break;
        } else {
            cout << "Invalid command. Try again." << endl;
        }
    }

    return 0;
}
