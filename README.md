# IoT-Based Smart Pet Feeder

## Team Members
- **GUNARATHNA A.M.S.N.** (2021/E/003)
- **SALGADU D.P.** (2021/E/026)
- **DISSANAYAKA D.M.U.G.D.L** (2021/E/029)
- **WIJESURIYA P.D.D.N.** (2021/E/118)

## Introduction
The IoT-Based Smart Pet Feeder is a modern solution for simplifying pet care. By automating the feeding process, the system ensures pets are fed on time, even when their owners are away. With features like real-time food-level monitoring, customizable portions, and remote feeding control via a mobile app, it enhances convenience, reduces food waste, and promotes pet well-being.

---

## Problem and Solution

### Problem
Pet owners often face difficulties maintaining consistent feeding schedules due to work commitments or travel. This can lead to irregular feeding patterns and potential health risks for pets. Traditional feeding methods lack real-time monitoring for food levels and timely refills.

### Solution
The IoT-Based Smart Pet Feeder provides an intelligent and efficient way to address these challenges. The system leverages IoT technology to:

- Automate pet feeding schedules.
- Monitor food levels in real-time.
- Notify owners via a mobile application.

### Key Features
- **Remote Feeding Control**: Feed pets from anywhere using the mobile app.
- **Low Food Alerts**: Get automatic notifications when food storage is low.
- **Customizable Portions**: Adjust food portions based on pet dietary needs.
- **Flexible Feeding Modes**: Choose between manual or scheduled feeding.

---

## Novelty

1. **Dynamic Portion Calculation**: Uses an algorithm to customize food portions based on the pet's size, weight, and age.
2. **Offline Functionality**: Pre-scheduled feeding ensures feeding continuity without an active internet connection.
3. **Waste Minimization**: Ultrasonic sensors prevent overfilling and food spoilage.
4. **Future Scalability**: Potential integration with AI for tracking pet eating habits and automated dietary adjustments.

---

## High-Level Architecture

### Technology Choices
- **IoT Platform**: ESP32 for Wi-Fi-enabled communication between hardware and the mobile app.
- **Sensors**: Ultrasonic sensors for accurate food level monitoring.
- **Control Mechanism**: Servo motors for precise dispensing.
- **Mobile Application**: Built using React Native for cross-platform compatibility.
- **Database**: Firebase for real-time data synchronization and notifications.

### Justification
- ESP32 provides cost-effective, low-power wireless connectivity.
- Ultrasonic sensors ensure precise measurements for food and water levels.
- React Native enables a user-friendly interface compatible with Android and iOS.
- Firebase offers secure, real-time communication and data storage.

---

## Getting Started

### Prerequisites
- ESP32 microcontroller
- Ultrasonic sensors
- Servo motors
- Firebase account
- Node.js and React Native setup

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/<your-repo>/iot-smart-pet-feeder.git
   ```
2. Navigate to the project directory:
   ```bash
   cd iot-smart-pet-feeder
   ```
3. Install dependencies for the mobile app:
   ```bash
   cd mobile-app
   npm install
   ```
4. Upload the firmware to ESP32 using Arduino IDE or PlatformIO.

### Running the Mobile App
1. Start the development server:
   ```bash
   npm start
   ```
2. Run the app on an emulator or connected device:
   ```bash
   npm run android
   # or
   npm run ios
   ```

---

## Usage
1. Set up the hardware as per the provided circuit diagram.
2. Connect the mobile app to the ESP32.
3. Configure feeding schedules and portion sizes via the mobile app.
4. Receive notifications and monitor food levels in real-time.

---

## Future Enhancements
- AI-based dietary recommendations.
- Integration with smart home ecosystems (e.g., Alexa, Google Home).
- Camera module for live pet monitoring.

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Contact
For any questions or feedback, feel free to reach out to the team at [your-email@example.com].
