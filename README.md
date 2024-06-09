# Advanced-Mechatronics-Projects

<a name="readme-top"></a>

<!-- OPENER -->
<br />
<div align="center">
  <img
    src="images/Northwestern Engineering.png"
    alt="Logo"
    width="600"
    height="75"
  />
  <h3 align="center">ME433: Advanced Mechatronics</h3>
  <p align="center">
    <a
      href="https://github.com/Geo-Li/Advanced-Mechatronics-Projects/issues/new?labels=bug&template=bug-report.md"
      >Report Bug</a
    >
    ·
    <a
      href="https://github.com/Geo-Li/Advanced-Mechatronics-Projects/issues/new?labels=enhancement&template=feature-request.md"
      >Request Feature</a
    >
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#link-to-mini-projects">Link to Mini Projects</a></li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

<p align="center">This repository contains projects and homework assignments I completed for the ME433 class at Northwestern University</P>

### Built With

- [![C]][C-url]

- [![Python]][Python-url]

- [![Raspberry Pi Pico]][Raspberry Pi Pico-url]

- [![Raspberry Pi Zero W]][Raspberry Pi Zero W-url]


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

- Follow the [instructions](https://vanhunteradams.com/Pico/Setup/PicoSetupMac.html) for setting up the Raspberry Pi Pico

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/Geo-Li/Advanced-Mechatronics-Projects
   ```
2. Check each project link for instructions on how to build and run the project

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Usage

### Link to Mini Projects

- **Homework 1:** GitHub Practice
  - Shared our recipes on the [website](https://ndm736.github.io/ME433.Kitchen/recipes/scallion_oil_flavored_swimming_crab.html)
- [**Homework 2**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%202): Blink
  - Blink LED on Pico
- [**Homework 3**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%203): CDC, IO, ADC
  - Read and print the ADC value from RP2040 to the screen program
- [**Homework 4**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%204): SPI DAC
  - Generate 2Hz sine wave and 1Hz triangle wave with SPI DAC on Pico
- [**Homework 5**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%205): I2C IO Expander
  - Using MCP23008 as an I2C bus to read the button input and control a LED
- [**Homework 6**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%206): I2C OLED Display
  - Read the ADC values and display them in real-time on the SSD1306 screen
- [**Homework 7**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%207): UART
  - Retrieve information from user-input characters on the screen program, send it through the CP2102N (USB to UART converter), and display it on the SSD1306 screen
- [**Homework 8**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%208): Pi Zero
  - Initialized the Raspberry Pi Zero W to control an LED
- [**Homework 9**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%209): Pico to Zero
  - Send a user-typed integer from the Pico to the Pi Zero, and have the Pi Zero echo back the integer
- **Homework 10**: KiCad SCH and PCB
  - Skipped this project because Nick didn't expect us to complete it, given the minimal time he spent on the related material
- [**Homework 11**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%2011): Zero Camera
  - Captured pictures with the Arducam 5MP Camera and processed the images to identify the center of the line
- [**Homework 12**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%2012): RC Servo
  - Controlled an RC servo to perform specific rotations by sending PWM signals from the Pico
- [**Homework 13**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%2013): IMU Mouse
  - Read gyroscope and acceleration data from the MPU6050, and initialized the Pico as an input device (in this case, mouse) for my computer. Rotated the breadboard to send MPU6050 data to the computer, enabling mouse control
- [**Homework 14**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/blob/main/Homework/Homework%2014/dsp/dsp.ipynb): DSP
  - Process signals with time and apply filtering, then compare the performance of the raw and filtered data using the Fast Fourier Transform (FFT) algorithm
- **Homework 15**: Robot CAD
  - Skipped this project because I teamed up with my friend who is handling the CAD design for the final robot project, while I focus on the coding and algorithms
- [**Homework 16**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Homework%2016): Motor Control
  - Drove two blue TT motors using a DRV8833 breakout board, adjusting their speed based on the assumed line position number
- [**Final Competition Project**](https://github.com/Geo-Li/Advanced-Mechatronics-Projects/tree/main/Homework/Tech%20Cup): Tech Cup
  - Designed and programmed a differential drive robot to follow a line for the final competition project

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Zhuoyuan (Geo) Li - [@LinkedIn](https://www.linkedin.com/in/geo-li/) - [@Personal Portfolio](https://geo-li-portfolio.vercel.app/) - geoli.work@gmail.com

Project Link: [https://github.com/Geo-Li/Advanced-Mechatronics-Projects](https://github.com/Geo-Li/Advanced-Mechatronics-Projects)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [Project WiKi](https://github.com/ndm736/ME433_2024/wiki)
* [How to build an awesome README](https://github.com/othneildrew/Best-README-Template/tree/master)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- MARKDOWN LINKS & IMAGES -->
<!-- ## Build With -->
[C]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://devdocs.io/c/
[Raspberry Pi Pico]: https://img.shields.io/badge/Raspberry%20Pi%20Pico-C51A4A?style=for-the-badge&logo=raspberry-pi&logoColor=white
[Python]: https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white
[Python-url]: https://www.python.org/doc/
[Raspberry Pi Pico-url]: https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html
[Raspberry Pi Zero W]: https://img.shields.io/badge/Raspberry%20Pi%20Zero%20W-B03060?style=for-the-badge&logo=raspberry-pi&logoColor=white
[Raspberry Pi Zero W-url]: https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#raspberry-pi-zero

