import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0
import QtQuick 2.5
import QtQuick.Window 2.2

Window {

    width:  Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width : 800
   height: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.height : 480
    visible: true // required, else it will be invisible


    Scene3D {
        id: scene3d
                   anchors.fill:  parent
                   focus: true
                   aspects: ["input", "logic"]
       cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

       ModelScene {
           id: modelScene

       }


    }

    MouseArea {
        id: globalMouseArea
        anchors.fill: parent
       // preventStealing: true
        property real velocity: 0.0
        property vector2d start : Qt.vector2d(0.0,0.0)
        property vector2d prev : Qt.vector2d(0.0,0.0)
        property vector2d angularMomentum : Qt.vector2d(0,0)
        property vector3d angularImpulse : Qt.vector3d(0,0,0)
        property vector2d accumulatedMomentum : Qt.vector2d(0,0)
        property bool tracing: false
        onPressed: {
            start = Qt.vector2d(mouse.x,mouse.y)
            prev = start
            tracing = true
            angularMomentum = Qt.vector2d(0,0)
            accumulatedMomentum = Qt.vector2d(0,0)

            angularRotationTimer.stop();
        }
        onPositionChanged: {
            if ( !tracing ) return

            var currPos = Qt.vector2d(mouse.x,mouse.y);
            var delta = currPos.minus(prev);

            // FIXME fix initial model orientation instead of x,y swap

            angularImpulse = Qt.vector3d(delta.y,delta.x,0.0).times(0.1)
            modelScene.modelRotationAngles = modelScene.modelRotationAngles.plus(angularImpulse)


            prev = currPos
        }
        onReleased: {
            tracing = false

            angularRotationTimer.start();
        }
    }

    Timer {
        repeat: true
        id : angularRotationTimer
        interval : 16
        onTriggered: {
             modelScene.modelRotationAngles = modelScene.modelRotationAngles.plus(globalMouseArea.angularImpulse)
        }
    }

}
