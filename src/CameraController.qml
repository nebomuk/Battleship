import QtQuick 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Logic 2.0
import QtQml 2.2

Entity {
    id: root
    property Camera camera
    property real linearSpeed: 10.0
    property real orbitSpeed: -180.0
    property real lookSpeed: 180.0
    property bool firstPersonMode: true

    QtObject {
        id: d
        readonly property vector3d firstPersonUp: Qt.vector3d(0, 1, 0)
        property bool leftMouseButtonPressed: false
        property bool rightMouseButtonPressed: false
        property real vx: 0;
        property real vy: 0;
        property real vz: 0;
        property real dx: 0
        property real dy: 0
        property bool fineMotion: false
    }

    KeyboardDevice {
        id: keyboardSourceDevice
    }

    MouseDevice {
        id: mouseSourceDevice
        sensitivity: d.fineMotion ? 0.01 : 0.1
    }

    LogicalDevice {
        id: cameraControlDevice

        actions: [
            Action {
                name: "LMB"
                inputs: [
                    ActionInput {
                        sourceDevice: mouseSourceDevice
                        buttons: [MouseEvent.LeftButton]
                    }
                ]
            },
            Action {
                name: "RMB"
                inputs: [
                    ActionInput {
                        sourceDevice: mouseSourceDevice
                        buttons: [MouseEvent.RightButton]
                    }
                ]
            },
            Action {
                name: "fineMotion"
                inputs: [
                    ActionInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_Shift]
                    }
                ]
            }

        ] // actions

        axes: [
            // Rotation
            Axis {
                name: "RX"
                inputs: [
                    AnalogAxisInput {
                        sourceDevice: mouseSourceDevice
                        axis: MouseDevice.X
                    }
                ]
            },
            Axis {
                name: "RY"
                inputs: [
                    AnalogAxisInput {
                        sourceDevice: mouseSourceDevice
                        axis: MouseDevice.Y
                    }
                ]
            },
            // Translation
            Axis {
                name: "TX"
                inputs: [
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_Left]
                        scale: -1.0
                    },
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_Right]
                        scale: 1.0
                    }
                ]
            },
            Axis {
                name: "TZ"
                inputs: [
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_Up]
                        scale: 1.0
                    },
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_Down]
                        scale: -1.0
                    }
                ]
            },
            Axis {
                name: "TY"
                inputs: [
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_PageUp]
                        scale: 1.0
                    },
                    ButtonAxisInput {
                        sourceDevice: keyboardSourceDevice
                        buttons: [Qt.Key_PageDown]
                        scale: -1.0
                    }
                ]
            }
        ] // axes
    }

    components: [
        AxisActionHandler {
            id: handler
            logicalDevice: cameraControlDevice

            onAxisValueChanged: {

                switch (name) {

                case "TX": {
                    d.vx = axisValue * linearSpeed
                    break;
                }

                case "TY": {
                    d.vy = axisValue * linearSpeed
                    break;
                }

                case "TZ": {
                    d.vz = axisValue * linearSpeed
                    break;
                }

                case "RX": {
                    d.dx = axisValue;
                    break;
                }

                case "RY": {
                    d.dy = axisValue;
                    break;
                }

                }
            }

            onActionStarted: {

                switch (name) {

                case "LMB": {
                    d.leftMouseButtonPressed = true;
                    break;
                }

                case "RMB": {
                    d.rightMouseButtonPressed = true;
                    break;
                }

                case "fineMotion": {
                    console.log("fineMotion started")
                    d.fineMotion = true;
                    break;
                }

                }

            }

            onActionFinished: {

                switch (name) {

                case "LMB": {
                    d.leftMouseButtonPressed = false;
                    break;
                }

                case "RMB": {
                    d.rightMouseButtonPressed = false;
                    break;
                }

                case "fineMotion": {
                    console.log("fineMotion finished")
                    d.fineMotion = false;
                    break;
                }

                }
            }
        },

        FrameAction {
            onTriggered: {
                // The time difference since the last frame is passed in as the
                // argument dt. It is a floating point value in units of seconds.
                root.camera.translate(Qt.vector3d(d.vx, d.vy, d.vz).times(dt))

                if (d.leftMouseButtonPressed) {
                    if (root.firstPersonMode)
                        root.camera.pan(root.lookSpeed * d.dx * dt, d.firstPersonUp)
                    else
                        root.camera.pan(root.lookSpeed * d.dx * dt)
                    root.camera.tilt(root.lookSpeed * d.dy * dt)
                } else if (d.rightMouseButtonPressed) {
                    if (root.firstPersonMode)
                        root.camera.panAboutViewCenter(root.lookSpeed * d.dx * dt, d.firstPersonUp)
                    else
                        root.camera.panAboutViewCenter(root.lookSpeed * d.dx * dt)
                    root.camera.tiltAboutViewCenter(root.orbitSpeed * d.dy * dt)
                }
            }
        }
    ] // components
}
