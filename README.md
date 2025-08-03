# IoT-Based Human Activity Recognition using ESP32 and Neural Network

This project implements a human activity recognition (HAR) system using an ESP32 microcontroller and a pre-trained neural network. The system captures sensor data, uses the model to predict the corresponding human activity, and sends the result over a UDP connection. This project is designed to be deployed on an ESP32 board and managed using PlatformIO.

## Project Overview

The core of this project is a neural network model trained to classify different human activities based on sensor readings. The model is deployed on an ESP32, which receives sensor data, performs inference, and communicates the results over Wi-Fi.

- The machine learning model is developed and trained in the `ml-project & udp test.ipynb` Jupyter Notebook.
- The ESP32 firmware is located in the `src` directory, with the main logic in `main.cpp`.
- The project uses PlatformIO for dependency management and building.

## Features

- **Real-time Inference**: Performs activity recognition in near real-time on the microcontroller.
- **Wireless Communication**: Uses Wi-Fi to connect to a network and UDP to send and receive data.
- **Data Preprocessing**: Implements on-device scaling of sensor data to match the model's input requirements.
- **Deep Learning Model**: Utilizes a multi-layer neural network for classification.

## Hardware Requirements

- An ESP32-based development board (e.g., ESP32-DevKitC).

## Software & Dependencies

- **PlatformIO**: For building, uploading, and managing the project dependencies.
- **Arduino Framework**: As the base for the ESP32 firmware.
- **Libraries**:
  - `bblanchon/ArduinoJson`: For efficient JSON serialization and deserialization.
  - `TensorFlow Lite for Microcontrollers`: The `tfmicro` library in the `lib` directory is used to run the model.

## Setup and Installation

1.  **Clone the repository:**
    ```bash
    git clone <your-repository-url>
    cd IoT_proj
    ```

2.  **Open with PlatformIO**: Open the project folder in an IDE with the PlatformIO extension (e.g., VSCode).

3.  **Configure Wi-Fi**: Open `src/main.cpp` and update the `SSID` and `PASSWORD` with your Wi-Fi credentials:
    ```cpp
    #define SSID "your_wifi_ssid"
    #define PASSWORD "your_wifi_password"
    ```

4.  **Build and Upload**: Use the PlatformIO interface to build and upload the firmware to your ESP32 board.

5.  **Monitor**: Open the serial monitor to see the device's IP address once it connects to your Wi-Fi network.

## Usage

The ESP32 listens for UDP packets on port `30000`. You can send sensor data to it using a script, like the one provided in the `ml-project & udp test.ipynb` notebook.

- **Input Data**: The data sent to the ESP32 should be a JSON array containing 12 floating-point numbers representing the sensor readings.
- **Prediction**: The ESP32 will process the data, predict the activity, and send back a JSON object with the predicted class index (e.g., `{"pred": 5}`).

## Machine Learning Model

The Jupyter Notebook `ml-project & udp test.ipynb` details the entire process of creating the machine learning model:

1.  **Dataset**: The model is trained on the MHEALTH dataset, which contains sensor data for various physical activities.
2.  **Preprocessing**: The notebook covers loading the data, balancing the dataset by sampling, and splitting it into training and testing sets.
3.  **Scaling**: `StandardScaler` is used to normalize the features. The `mean_` and `scale_` values from the scaler are hardcoded into `src/main.cpp` to preprocess incoming data on the ESP32.
4.  **Model Architecture**: A `Sequential` model is built with several `Dense` layers using `relu` activation and a final `softmax` layer for multi-class classification.
5.  **Training & Evaluation**: The model is compiled and trained. Its performance is evaluated on the test set.
6.  **Conversion to TFLite**: The trained Keras model is converted into a TensorFlow Lite model (`.tflite`) and then into a C source file (`model_data.cc`) for use with the `tfmicro` library on the ESP32.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.