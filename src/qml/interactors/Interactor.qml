import QtQuick 2.5

QtObject {
    // One of the following: ready, running, awaiting, falure, finished
    property string state: "ready"

    signal started
    signal updated
    signal awaiting
    signal finished
    signal failed

    function start() {}
    onStarted: state = "running"
    onAwaiting: state = "awaiting"
    onFinished: state = "finished"
    onFailed: state = "failed"
}
