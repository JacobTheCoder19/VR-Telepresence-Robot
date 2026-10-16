# Head-Tracking VR Telepresence Robot 🤖🥽

<p align="center">
  
</p>

<p align="center"><i>When I turn my head, the robot turns its head. When I look through the headset, I see what the robot sees.</i></p>

<h2><em>💎 Key Features 💎</em></h2>
<div>
  🟩 <strong>Real-Time Head Tracking</strong><br>
  &emsp;🔸 An MPU6050 gyroscope on top of the headset measures which way I'm looking, and the robot's head follows my movements in real time.<br><br>
</div>
<div>
  🟩 <strong>First-Person VR Video Feed</strong><br>
  &emsp;🔸 An ESP32 camera board sits behind the robot's eye and streams live video. The feed is duplicated side by side so that when my phone is placed in a simple VR headset, it looks like a normal first-person view through the robot's eye.<br><br>
</div>
<div>
  🟩 <strong>Wireless ESP32-to-ESP32 Link</strong><br>
  &emsp;🔸 The headset ESP32 hosts its own WiFi network and streams head angles to a second ESP32 on the robot, so there are no wires between me and the robot and no router is needed.<br><br>
</div>
<div>
  🟩 <strong>Recenter Button & Status LED</strong><br>
  &emsp;🔸 A button on the headset sets "straight ahead" at any time, just like the recenter feature on commercial VR headsets. The onboard LED shows whether the headset is calibrating, waiting for the robot, or streaming.<br><br>
</div>
<div>
  🟩 <strong>Automatic Reconnection</strong><br>
  &emsp;🔸 The robot keeps retrying its connection to the headset, so either side can be powered on first or restarted and the link recovers on its own.<br><br>
</div>
<div>
  🟩 <strong>Smooth Servo Motion</strong><br>
  &emsp;🔸 The Arduino Mega smooths the incoming angles before moving the servo, so the head glides instead of jittering.<br><br>
</div>
<div>
  🟩 <strong>LED Ring Eye & Sound</strong><br>
  &emsp;🔸 A NeoPixel LED ring surrounds the camera eye and spins red, green, and blue on startup, while a piezo buzzer plays a boot-up sound.<br><br>
</div>
<div>
  🟩 <strong>Fully Custom 3D-Printed Body</strong><br>
  &emsp;🔸 I hand-modeled every 3D-printed part myself, including the head, the removable "JG" back panel, the camera and LED eye mount, the neck with its threaded bolts and nuts, and the servo connector, then printed them all on my Flashforge Adventurer 5M.<br><br>
</div>

<h2><em>✨ Purpose / Inspiration ✨</em></h2>
&emsp;This is my first real robot, built from scratch. The idea came from wanting to actually <i>be</i> the robot instead of just programming it. I wanted to put on a headset, turn my head, and have the robot look around with me so I could see the world from its point of view. I didn't have a real VR headset, so I used an old phone VR viewer (the kind with just two lenses and no electronics) and made the robot do all the work: the camera stream is split into two side-by-side images so it looks right through the lenses, and I mounted a gyro and an ESP32 on top of the headset to turn it into a head tracker.

&emsp;Building it taught me a lot more than just code. I designed and modeled the entire body myself, went through multiple print iterations to get parts like the neck and servo connector to fit properly, and debugged real hardware problems like power brownouts, wireless reconnection, and gyro drift.

<h2><em>⚙️ How It Works ⚙️</em></h2>
&emsp;The system has three microcontrollers working together, plus a separate camera board.

```
 HEADSET                                   ROBOT
┌───────────────────┐   WiFi (TCP)   ┌───────────────────┐  Serial   ┌──────────────┐
│ MPU6050 gyro      │ ─────────────▶ │ ESP32 receiver    │ ────────▶ │ Arduino Mega │
│ ESP32 transmitter │  head angle    │                   │  115200   │ servo, LEDs, │
│ (WiFi access pt.) │  20x / second  └───────────────────┘           │ buzzer       │
└───────────────────┘                                                └──────────────┘
        ▲
        │ phone in VR viewer shows split-screen video
        │
┌───────────────────┐
│ ESP32 camera      │ ◀── sits behind the robot's eye
│ (hosts own WiFi)  │
└───────────────────┘
```

&emsp;<b>1. The headset.</b> When the headset powers on, it sits flat on a table while the ESP32 calibrates the MPU6050 by measuring the gyro's built-in error so it can be subtracted later. It then creates its own WiFi network and waits for the robot to connect. Once connected, it reads the gyro's yaw angle and sends it to the robot 20 times per second. Pressing the recenter button saves the current direction as "straight ahead."

&emsp;<b>2. The robot's ESP32.</b> This board connects to the headset's WiFi network and forwards every angle it receives to the Arduino Mega over a serial connection. If the connection drops or no data arrives for two seconds, it automatically reconnects.

&emsp;<b>3. The Arduino Mega.</b> The Mega reads each angle, smooths it to remove jitter, maps my head range of -70° to 70° onto the servo's 0° to 180°, and moves the servo that turns the robot's head. It also runs the LED ring eye and the buzzer.

&emsp;<b>4. The camera.</b> An ESP32 camera board behind the robot's eye hosts its own WiFi network and video stream. I connect my phone to it, open the page, and place the phone in the VR viewer. The page loads the stream only once and draws each frame to both halves of the screen (the ESP32 can only serve one stream at a time), so each lens sees one copy and it looks like a single normal view.

<h2><em>🔌 Hardware 🔌</em></h2>

| Part | Purpose |
|---|---|
| ESP32 + MPU6050 gyro | Head tracker on the headset |
| ESP32 | Wireless receiver on the robot |
| Arduino Mega 2560 | Main robot controller |
| SG90 servo | Turns the robot's head |
| Freenove ESP32-WROVER (camera) | Live video from the robot's eye |
| NeoPixel LED ring | Animated eye around the camera |
| Piezo buzzer | Startup sounds |
| Pushbutton | Recenter the view |
| USB power bank | Powers the headset |
| Phone VR viewer | Displays the split-screen video |
| Custom 3D-printed parts | Head, neck, back panel, servo connector (all hand-modeled) |

<h2><em>📁 Repository Layout 📁</em></h2>

```
Headset_ESP32_Transmitter/   Headset code: gyro, recenter button, status LED, WiFi server
Robot_ESP32_Receiver/        Robot ESP32 code: auto-reconnecting WiFi receiver
Robot_Soul_v1/Main/          Arduino Mega code: servo control, LED eye, buzzer
Robot_Eye_Camera/            ESP32 camera code: hosts WiFi, streams split-screen VR view
3D_Models/                   STL files for the hand-modeled printed parts
images/                      Photos of the robot and headset
```

<h2><em>🔧 Setup 🔧</em></h2>

1. Set the same WiFi network name and password (8+ characters) at the top of both ESP32 sketches.
2. Upload `Headset_ESP32_Transmitter` to the headset ESP32 and `Robot_ESP32_Receiver` to the robot ESP32.
3. Upload `Robot_Soul_v1/Main` to the Arduino Mega. Wire ESP32 GPIO 17 to Mega pin 19 (RX1) and connect their grounds.
4. Upload `Robot_Eye_Camera` to the Freenove ESP32-WROVER camera board (select the ESP32 Wrover board with PSRAM enabled).
5. Power on the headset lying flat. Once the LED blips once per second, put it on, look straight ahead, and press the recenter button.
6. Connect your phone to the `RobotEye` WiFi network, open `http://192.168.4.1`, turn the phone sideways, and place it in the VR viewer.

<h2><em>🛠️ Challenges I Solved 🛠️</em></h2>
<div>
  🟩 <strong>Power Brownouts</strong><br>
  &emsp;🔸 The headset worked over USB but not on a 9V battery. The ESP32's WiFi draws large current spikes that made the battery's voltage sag and reset the board, so I switched to a USB power bank.<br><br>
</div>
<div>
  🟩 <strong>Lost Connections</strong><br>
  &emsp;🔸 The robot originally only tried to connect once at startup, so the power-on order mattered. I rewrote the receiver to reconnect automatically and the headset to replace dead connections.<br><br>
</div>
<div>
  🟩 <strong>Gyro Drift</strong><br>
  &emsp;🔸 A gyro alone can't know where "forward" is and slowly drifts over time, so I added a recenter button and calibrate the sensor while it's perfectly still.<br><br>
</div>
<div>
  🟩 <strong>Mechanical Fit</strong><br>
  &emsp;🔸 The servo connector's spline socket was too loose, so the head wobbled. I measured the socket against the servo's spline and redesigned it for a tight press fit with a stronger screw floor.<br><br>
</div>

<h2><em>🚀 What's Next 🚀</em></h2>

- [ ] Add a second servo so the robot can nod up and down (the gyro already measures pitch)
- [ ] Make the LED eye react to where I'm looking
- [ ] Send audio from the robot to the headset
- [ ] Design a cleaner enclosure for the headset electronics

<p align="center"><img src="images/printing.jpg" alt="Parts printing on my Flashforge Adventurer 5M" width="60%"></p>
