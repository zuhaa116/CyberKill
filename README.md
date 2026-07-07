# CyberKill

CyberKill is a social media privacy simulator that teaches users to recognize and respond to common online threats; like phishing, scams, and privacy violations, through interactive, timed decision-making. Built in C++ with FLTK, it uses randomized threat scenarios and a passive score-drain mechanic to simulate real-world urgency, reinforcing quick, informed judgment in digital security contexts.

## What This Demonstrates

- **Inheritance**: Threat is an abstract base class with concrete subclasses (FakeFriendRequest, SuspiciousLogin, DataLeak, MaliciousApp) implemented in Threat.h. Each subclass implements specific behavior for its type.
- **Polymorphism**: The app treats different Threat subclasses through the Threat interface. See PrivacyManager::spawnThreats and AppDashboard::updateThreatsDisplay for how virtual methods are used to display and handle threats.
- **Encapsulation**: UserProfile and PrivacySettings keep internal state private and expose access through public methods. Check UserProfile.h and PrivacySettings.h for examples.
- **Abstraction**: Threat exposes a clean virtual interface (e.g. handle(), display()) while hiding implementation details of each subclass, so the rest of the app interacts with threats generically.
- **Composition**: PrivacyManager is composed of a UserProfile and a collection of Threat objects, coordinating their interaction rather than inheriting from them.
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


## Copyright

© 2026 ZuhaaNadeem, Eman Fatima, Emaan Abrar. All rights reserved. This project and its content, including code, design, and assets, may not be copied, reproduced, or reused without permission.

