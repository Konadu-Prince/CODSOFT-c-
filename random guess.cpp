#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // Initialize random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));

    int randomNumber = rand() % 100 + 1; // Generate a random number between 1 and 100
    int userGuess;
    int attempts = 0;

    cout << "Welcome to the Number Guessing Game!" << endl;

    while (true) {
        cout << "Enter your guess: ";
        cin >> userGuess;
        attempts++;

        if (userGuess < 1 || userGuess > 100) {
            cout << "Please enter a number between 1 and 100." << endl;
        } else if (userGuess < randomNumber) {
            cout << "Too low! Try again." << endl;
        } else if (userGuess > randomNumber) {
            cout << "Too high! Try again." << endl;
        } else {
            cout << "Congratulations! You guessed the number " << randomNumber << " in " << attempts << " attempts." << endl;
            break;
        }
    }

    return 0;
}
