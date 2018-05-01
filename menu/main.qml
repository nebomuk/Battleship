import QtQuick.Window 2.2
import QtQuick 2.5
import QtGraphicalEffects 1.0


Window {

    width:  Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.width : 800
   height: Qt.platform.os == "android" || Qt.platform.os == "ios" ? Screen.height : 480
    visible: true // required, else it will be invisible

    property real divider : 1.5;


    ModelScene
    {
        width: Math.min(img.width /divider, img.height / divider)
        height: width
        anchors.verticalCenter:  img.verticalCenter
        anchors.right: img.right

    }

    Image
    {
        id : img
        source: "qrc:/woodTile.svg"
        anchors.fill: parent
        fillMode: Image.Tile
           layer.enabled: true
           layer.effect: OpacityMask {
               invert : true
               maskSource: Item {
                   width: img.width
                   height: img.height
                   Rectangle {
                       anchors.verticalCenter:  parent.verticalCenter
                       anchors.right: parent.right
                       anchors.rightMargin: 90/ 2  * Math.min(img.width , img.height) / 390
                       width: Math.min(img.width , img.height)/divider- 90  *Math.min(img.width , img.height) / 390
                       height: width
                       radius: Math.min(width, height)
                   }
               }
           }
    }

    // this image always vanishes as soon as ModelScene is rendered
    Image
    {
        id : boatWindow
        source : "qrc:/boatWindow.svg"
        anchors.verticalCenter:  img.verticalCenter
        anchors.right: img.right
        width: Math.min(img.width /divider, img.height / divider)
        height: width

    }

}
