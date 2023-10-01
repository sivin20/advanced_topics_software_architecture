#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <fstream>
#include <string.h>
#include <sstream>
#include <mosquitto.h>
#include <mosquittopp.h>

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

class myMosq : public mosqpp::mosquittopp
{
private:
    const char *host;
    const char *id;
    const char *topic;
    int port;
    int keepalive;

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);

public:
    myMosq(const char *id, const char *_topic, const char *host, int port);
    ~myMosq();
    bool send_message(const char *_message);
};

myMosq::myMosq(const char *_id, const char *_topic, const char *_host, int _port) : mosquittopp(_id)
{
    mosqpp::lib_init();   // Mandatory initialization for mosquitto library
    this->keepalive = 60; // Basic configuration setup for myMosq class
    this->id = _id;
    this->port = _port;
    this->host = _host;
    this->topic = _topic;
    connect_async(host, // non blocking connection to broker request
                  port,
                  keepalive);
    loop_start(); // Start thread managing connection / publish / subscribe
};

myMosq::~myMosq()
{
    loop_stop();           // Kill the thread
    mosqpp::lib_cleanup(); // Mosquitto library cleanup
}

bool myMosq::send_message(const char *_message)
{
    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = publish(NULL, this->topic, strlen(_message), _message, 1, false);
    return (ret == MOSQ_ERR_SUCCESS);
}

void myMosq::on_disconnect(int rc)
{
    std::cout << ">> myMosq - disconnection(" << rc << ")" << std::endl;
}

void myMosq::on_connect(int rc)
{
    if (rc == 0)
    {
        std::cout << ">> myMosq - connected with server" << std::endl;
    }
    else
    {
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")" << std::endl;
    }
}

void myMosq::on_publish(int mid)
{
    std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}

struct MosquittoConfig
{
    int listener = 0;
    bool allowAnonymous = false;
    std::string logType;
};

MosquittoConfig readMosquittoConfig(const std::string &configFile)
{
    MosquittoConfig config;

    std::ifstream file(configFile);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << configFile << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;

        if (key == "listener")
        {
            config.listener = std::stoi(value);
        }
        else if (key == "allow_anonymous")
        {
            config.allowAnonymous = (value == "true");
        }
        else if (key == "log_type")
        {
            config.logType = value;
        }
    }

    return config;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator with the current time

    std::chrono::seconds duration(5); // 1 minute

    auto startTime = std::chrono::system_clock::now();
    auto currentTime = startTime;
    int bottleCounter = 0;

    MosquittoConfig config = readMosquittoConfig("../mosquitto.conf");

    // Output the loaded values
    std::cout << "Listener: " << config.listener << std::endl;
    std::cout << "Allow Anonymous: " << (config.allowAnonymous ? "true" : "false") << std::endl;
    std::cout << "Log Type: " << config.logType << std::endl;

    // myMosq myMosq(0, )

    // while (currentTime - startTime < duration)
    // {
    //     // Generate a random number between 1 and 5
    //     int randomDelay = std::rand() % 50 + 1;

    //     bottleCounter++;
    //     // Sleep for the random delay (1 to 5 seconds)
    //     std::this_thread::sleep_for(std::chrono::milliseconds(randomDelay));

    //     // creating botlte obj
    //     Bottle bottle(bottleCounter, getRandomBoolean());
    //     // std::cout << "Bottle " << bottleCounter << " Info:" << std::endl;

    //     // Push to data bus
    //     bottle.displayInfo();

    //     // Update the current time
    //     currentTime = std::chrono::system_clock::now();
    // }

    return 0;
}
