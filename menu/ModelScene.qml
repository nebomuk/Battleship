import QtQuick 2.5
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Scene3D 2.0
import Qt.labs.folderlistmodel 2.1

Item
{

FolderListModel {
        folder: "qrc:/models"
        id: folderModel
        showFiles: true
        showDirs: false

        // model loads asynchronously after Component.onCompleted
        onCountChanged :
        {
            modelScene.meshSource = get(currentMeshIndex,"fileURL");
        }

        property int currentMeshIndex: 0

        function nextMesh()
        {
          currentMeshIndex++;
            currentMeshIndex = currentMeshIndex % folderModel.count;
            modelScene.meshSource = get(currentMeshIndex,"fileURL");
        }
    }

Scene3D {

    id: scene3d
               anchors.fill:  parent
               focus: true
               aspects: ["input", "logic"]
   cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

   ModelEntity {
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
    property vector3d angularImpulse : Qt.vector3d(1,1,1).times(0.5)
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

        folderModel.nextMesh();
        angularRotationTimer.start();
    }
}

Timer {
    repeat: true
    running: true
    id : angularRotationTimer
    interval : 16
    onTriggered: {
         modelScene.modelRotationAngles = modelScene.modelRotationAngles.plus(globalMouseArea.angularImpulse)
    }
}
}
