import QtQuick.Window 2.2
import QtQuick 2.5


Window {

    id : window

    Component.onCompleted:
        {
            if(Qt.platform.os !== "android" && Qt.platform.os !== "ios" && Qt.platform.os !== "tvos")
            {
                width = 800
                height = 480
            }


    }

    property bool modelSceneEnabled: Qt.platform.os != "android" && Qt.platform.os != "ios"

    property real boatWinWidthHeight : Math.min(img.width /divider, img.height / divider)

    visible: true // required, else it will be invisible

    color: Qt.rgba(1, 1, 1, 1)

    property real divider : 1.5;

    // 3d model scene not loaded on android/ios due to issues when overdrawing on top of the scene
    Timer
    {
         interval: 150; running: modelSceneEnabled ; repeat: false
        onTriggered: {
            var component = Qt.createComponent("ModelScene.qml");
            var scene = component.createObject(window,
                                           {
                                               "z": 2,
                                               "width": boatWinWidthHeight,
                                               "height": boatWinWidthHeight,
                                               "anchors.verticalCenter":  img.verticalCenter,
                                               "anchors.right": img.right
                                           }

                                           );
        }
    }

    Rectangle {
        id : modelSceneBackground
        z : 1
        width: boatWinWidthHeight
        height : boatWinWidthHeight
        anchors.verticalCenter:  img.verticalCenter
        anchors.right: img.right
        gradient: Gradient.MalibuBeach
        border.width: 0
        radius: width/2
    }

    // this image always vanishes as soon as ModelScene is rendered
    Image
    {
        id : boatWindow
        z : 3
        source : "qrc:/menu_images/boatWindow.svg"
        anchors.verticalCenter:  img.verticalCenter
        anchors.right: img.right
        width: Math.min(img.width /divider, img.height / divider)
        height: width
    }





    Image
    {
        visible: modelSceneEnabled // this 2d animated submarine is used instead of model scene on android and ios
        id : submarine
        width : boatWindow.width * 0.2
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images2/orangeSubmarine.svg"
        anchors.verticalCenter : img.verticalCenter
        anchors.right : parent.right

        ParallelAnimation {
                    id: xAnim
                    // Animations on properties start running by default
                    running: true
                    loops: Animation.Infinite
                    NumberAnimation {
                        target: submarine
                        property: "anchors.rightMargin"
                        from : 0.5* Screen.width; to : -200; duration: 5000; easing.type: Easing.Linear }

                    ScaleAnimator {
                                  target: submarine
                                  from: 4
                                  to: 1
                                  duration: 5000
                              }
                }
    }


    Image
    {
        id : img
        source: "qrc:/menu_images/woodTile.svg"
        anchors.fill: parent
        fillMode: Image.Tile
    }



    Image
    {
        id : boatsWindow
        source : "qrc:/menu_images/boatsWheel.svg"
        width: Math.min(img.width /divider, img.height / divider)
        height: width
        anchors.verticalCenter:  img.verticalCenter
        anchors.left: img.left

        SequentialAnimation
        {
            running: true

            loops: Animation.Infinite

            RotationAnimator {
                      target: boatsWindow;
                      from: 0;
                      to: 360;
                      duration: 3000
                      easing.type : Easing.InOutQuad

                  }
            RotationAnimator {
                      target: boatsWindow;
                      from: 360;
                      to: 0;
                      duration: 3000
                      easing.type : Easing.InOutQuad
                  }
        }
    }

    Image
    {
        id : playButton
        source: "qrc:/menu_images/playButton.svg"
        width: Math.min(img.width /divider, img.height / divider)/3
        height: width
        opacity: 0
        anchors.horizontalCenter:  img.horizontalCenter

        MouseArea{
            anchors.fill: parent
            onClicked: {

                GraphicsView.show();
                window.hide();
            }
        }

        property real yFraction: 0.0

        onYFractionChanged:
        {
            y = yFraction * img.height
        }

        ParallelAnimation
        {
            running: true


                      NumberAnimation{
                      property : "yFraction"
                      target: playButton;
                      from: 0.0;
                      to: 0.4;
                      duration: 1500
                      easing.type : Easing.OutBounce
                      }

        OpacityAnimator {
                  target: playButton;
                  from: 0.1;
                  to: 0.9;
                  duration: 2000
              }

        }

    }

}
