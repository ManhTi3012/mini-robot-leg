# Mini Robot Leg
This is our first time in building legged robot, intended to use in a quadruple one, everything is designed from scratch.

For the actuator, we use a cheap 5$ BL3525O, rewinded the stator from 15 to 30 turns. With 1:10 Cycloidal reducer, it have a theoretical max velocity of 240RPM.

To control the BLDC, we used my previous project, a [DuoDrive](https://github.com/ManhTi3012/DuoDrive) running SimpleFOC library, the code can be found in the repo.

<p align="center" style="display: flex; gap: 10px;">
  <img src="https://github.com/ManhTi3012/mini-cycloidal-drive/blob/main/photo/motor.jpg" width="49%">
  <img src="https://github.com/ManhTi3012/mini-cycloidal-drive/blob/main/photo/duodrive.jpg" width="49%">
</p>


**⚠️  Even with 1:10 reduction ratio, the actuator was too weak to hold the leg itself up, so this is only used for study inverse kinematic and motion control**


# Demo
![demo](https://github.com/ManhTi3012/mini-cycloidal-drive/blob/main/photo/demo_cad.gif)

![demo](https://github.com/ManhTi3012/mini-cycloidal-drive/blob/main/photo/demo_live.gif)
