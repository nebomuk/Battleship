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
    preventStealing: true
    property real velocity: 0.0
    property vector2d start : Qt.vector2d(0.0,0.0)
    property vector2d prev : Qt.vector2d(0.0,0.0)
    property bool tracing: false
    onPressed: {
    start = Qt.vector2d(mouse.x,mouse.y)
    prev = start
    tracing = true
    }
    onPositionChanged: {
    if ( !tracing ) return

    var currPos = Qt.vector2d(mouse.x,mouse.y);
    var delta = currPos.minus(prev);
    // FIXME fix initial model orientation instead of x,y swap
    var angularImpulse = Qt.vector3d(delta.y,delta.x,0.0).times(0.1)
    console.log(delta)
    modelScene.modelRotationAngles = modelScene.modelRotationAngles.plus(angularImpulse)

    console.log("onPositionChanged")

    prev = currPos
    }
    onReleased: {
    tracing = false
    console.log("onRelease")

    }
    }

}
