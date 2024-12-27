#include<chrono>
#include<cstdio>
#include<ctime>
#include<fstream>
#include<iostream>
#include<random>
#include<string>
#include<thread>

int main(int argc, char** argv) {
    // Initialize variables
    std::string hostName = argv[1];
    std::string otherHost = argv[2];

    long long prime = 15485863; // Static prime - could have sessions communicate and agree on random prime/generator
    long long generator = 2;
    srand(time(0));
    long long privateKey = rand();

    std::cout << "Prime: " << prime << std::endl;
    std::cout << "Generator: " << generator << std::endl;
    std::cout << "Private Key: " << privateKey << std::endl;



    // Calculate public key
    long long publicKey = 1;
    for (int i = 0; i < privateKey; i++) {
        publicKey *= generator;
        publicKey %= prime;
    }

    std::cout << "Public Key: " << publicKey << std::endl;



    // Write public key to file
    std::ofstream outputFile(hostName);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file " << hostName << std::endl;
        return 0;
    }

    outputFile << publicKey;
    outputFile.close();



    // Read public key from other session
    bool done = false;
    do { // Wait for other process to create public key file
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::ifstream testFile(otherHost);
        done = testFile.good();
        testFile.close();
    } while (!done);

    std::ifstream inputFile(otherHost);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file " << otherHost << std::endl;
    }

    int otherPublicKey;
    inputFile >> otherPublicKey;
    inputFile.close();

    std::cout << "Other public key: " << otherPublicKey << std::endl;



    // Calculate shared secret using other session public key + our private key
    long long sharedSecret = 1;
    for (int i = 0; i < privateKey; i++) {
        sharedSecret *= otherPublicKey;
        sharedSecret %= prime;
    }

    std::cout << "Shared Secret: " << sharedSecret << std::endl;



    // Delete files generated
    std::remove(&otherHost[0]); // Convert to C string
}
