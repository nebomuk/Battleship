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

}
