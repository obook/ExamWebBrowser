import QtQuick 2.1
import QtWebEngine 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtQuick.Controls.Private 1.0

ApplicationWindow {
    id: browserWindow
    function load(url) { currentWebView.url = url }
    property Item currentWebView: tabs.currentIndex < tabs.count ? tabs.getTab(tabs.currentIndex).item : null

    width: 1300
    height: 900
    visible: true
    title: currentWebView && currentWebView.title

    // Create a styleItem to determine the platform.
    // When using style "mac", ToolButtons are not supposed to accept focus.
    StyleItem { id: styleItem }
    property bool platformIsMac: styleItem.style == "mac"

    Action {
        id: focus
        shortcut: "Ctrl+L"
        onTriggered: {
            addressBar.forceActiveFocus();
            addressBar.selectAll();
        }
    }
    Action {
        shortcut: "Ctrl+R"
        onTriggered: {
            if (currentWebView)
                currentWebView.reload()
        }
    }
    Action {
        shortcut: "Ctrl+T"
        onTriggered: {
            tabs.createEmptyTab()
            addressBar.forceActiveFocus();
            addressBar.selectAll();
        }
    }
    Action {
        shortcut: "Ctrl+W"
        onTriggered: {
            if (tabs.count == 1)
                browserWindow.close()
                else
                    tabs.removeTab(tabs.currentIndex)
        }
    }

    toolBar: ToolBar {
        id: navigationBar
        RowLayout {
            anchors.fill: parent;
            ToolButton {
                id: backButton
                iconSource: "icons/go-previous.png"
                onClicked: currentWebView.goBack()
                enabled: currentWebView && currentWebView.canGoBack
                activeFocusOnTab: !browserWindow.platformIsMac
            }
            ToolButton {
                id: forwardButton
                iconSource: "icons/go-next.png"
                onClicked: currentWebView.goForward()
                enabled: currentWebView && currentWebView.canGoForward
                activeFocusOnTab: !browserWindow.platformIsMac
            }
            ToolButton {
                id: reloadButton
                iconSource: currentWebView && currentWebView.loading ? "icons/process-stop.png" : "icons/view-refresh.png"
                onClicked: currentWebView && currentWebView.loading ? currentWebView.stop() : currentWebView.reload()
                activeFocusOnTab: !browserWindow.platformIsMac
            }
            TextField {
                id: addressBar
                Image {
                    anchors.verticalCenter: addressBar.verticalCenter;
                    x: 5
                    z: 2
                    id: faviconImage
                    width: 16; height: 16
                    source: currentWebView && currentWebView.icon
                }
                style: TextFieldStyle {
                    padding {
                        left: 26;
                    }
                }
                focus: true
                Layout.fillWidth: true
                text: currentWebView && currentWebView.url
                onAccepted: currentWebView.url = utils.fromUserInput(text)
            }
        }
        ProgressBar {
            id: progressBar
            height: 3
            anchors {
                left: parent.left
                top: parent.bottom
                right: parent.right
                leftMargin: -parent.leftMargin
                rightMargin: -parent.rightMargin
            }
            style: ProgressBarStyle {
                background: Item {}
            }
            z: -2;
            minimumValue: 0
            maximumValue: 100
            value: (currentWebView && currentWebView.loadProgress < 100) ? currentWebView.loadProgress : 0
        }
    }

    TabView {
        id: tabs
        function createEmptyTab() {
            var tab = addTab("", tabComponent)
            // We must do this first to make sure that tab.active gets set so that tab.item gets instantiated immediately.
            tabs.currentIndex = tabs.count - 1
            tab.title = Qt.binding(function() { return tab.item.title })
            return tab
        }

        anchors.fill: parent
        Component.onCompleted: createEmptyTab()

        Component {
            id: tabComponent
            WebEngineView {
                id: webEngineView
                focus: true

                onLinkHovered: {
                    if (hoveredUrl == "")
                        resetStatusText.start()
                        else {
                            resetStatusText.stop()
                            statusText.text = hoveredUrl
                        }
                }
            }
        }
    }

    Rectangle {
        id: statusBubble
        color: "oldlace"
        property int padding: 8

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: statusText.paintedWidth + padding
        height: statusText.paintedHeight + padding

        Text {
            id: statusText
            anchors.centerIn: statusBubble
            elide: Qt.ElideMiddle

            Timer {
                id: resetStatusText
                interval: 750
                onTriggered: statusText.text = ""
            }
        }
    }
}
