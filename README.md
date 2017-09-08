# DriverMonitor

Aims to identify whether the driver is drowsy or distracted away from the road.


Version:
- OpenCV 3.2.0
- Dlib 19.4
- Qt 5.6.2 for MinGW 4.9.2 32 bit

To test:
- Create new 'Qt Widgets Application' project.
- Add the header files and source files.
- Change OpenCV and Dlib INCLUDEPATH and LIBS paths in the .pro file.
- Change 'shape_predictor_68_face_landmarks.dat' path at mainwindow.cpp line 114.
- Compile and Run
