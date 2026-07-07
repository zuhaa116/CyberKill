# CyberKill

A small C++ FLTK desktop simulation that teaches social media privacy concepts by letting the player make decisions to handle simulated threats.

## What This Demonstrates

- **Inheritance**: Threat is an abstract base class with concrete subclasses (FakeFriendRequest, SuspiciousLogin, DataLeak, MaliciousApp) implemented in Threat.h. Each subclass implements specific behavior for its type.
- **Polymorphism**: The app treats different Threat subclasses through the Threat interface. See PrivacyManager::spawnThreats and AppDashboard::updateThreatsDisplay for how virtual methods are used to display and handle threats.
- **Encapsulation**: UserProfile and PrivacySettings keep internal state private and expose access through public methods. Check UserProfile.h and PrivacySettings.h for examples.
- **Resource management**: PrivacyManager owns dynamically created Threat objects and deletes them in its destructor.

## Tech Stack

- Language: C++ (C++11)
- GUI: FLTK

## Build and Run (Linux example)

**1. Install dependencies on Debian or Ubuntu**
```
sudo apt update
sudo apt install build-essential libfltk1.3-dev cmake
```

**2. Build with CMake (recommended)**
```
mkdir build && cd build
cmake ..
make
```
From the build directory run:
```
./CyberKill
```

**3. Or compile directly**
```
g++ -std=c++11 main.cpp AppDashboard.cpp PrivacyManager.cpp PrivacySettings.cpp UserProfile.cpp -lfltk -o CyberKill
./CyberKill
```

## Notes

- The app writes a runtime log file named cyberkill_log.txt. This file is excluded from version control by .gitignore.
- If you want a packaged binary for a client demo, build for the target operating system and attach the binary to a release.


## License

This repository includes an MIT license. See LICENSE.
