#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

class Bottle
{
public:
    int id;
    bool clean;

    // Constructor to initialize id and clean attributes
    Bottle(int bottleId, bool isClean) : id(bottleId), clean(isClean)
    {
    }

    // Member function to display the bottle's information
    void displayInfo()
    {
        std::cout << "Bottle ID: " << id << std::endl;
        std::cout << "Is Clean: " << (clean ? "Yes" : "No") << std::endl;
    }
};

bool getRandomBoolean()
{
    // Generate a random integer (0 or 1)
    int randomInt = std::rand() % 2;

    // Convert the random integer to a boolean
    return randomInt == 1;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator with the current time

    std::chrono::seconds duration(5); // 1 minute

    auto startTime = std::chrono::system_clock::now();
    auto currentTime = startTime;
    int bottleCounter = 0;

    while (currentTime - startTime < duration)
    {
        // Generate a random number between 1 and 5
        int randomDelay = std::rand() % 50 + 1;

        bottleCounter++;
        // Sleep for the random delay (1 to 5 seconds)
        std::this_thread::sleep_for(std::chrono::milliseconds(randomDelay));

        // creating botlte obj
        Bottle bottle(bottleCounter, getRandomBoolean());
        // std::cout << "Bottle " << bottleCounter << " Info:" << std::endl;

        // Push to data bus
        bottle.displayInfo();

        // Update the current time
        currentTime = std::chrono::system_clock::now();
    }

    return 0;
}
