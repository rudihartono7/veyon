import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.0

Dialog {
    title: qsTr("Kirim Memo")
	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	standardButtons: Dialog.Ok | Dialog.Cancel
	visible: true
	modal: true

	onAccepted: {
		context.acceptTextMessage(textArea.text)
		qmlCore.deleteLater(this)
	}
	onRejected: qmlCore.deleteLater(this)

	ColumnLayout {
		anchors.fill: parent
		Label {
            text: qsTr("Silahkan klik tombol OK untuk mengirimkan pesan ke semua komputer.")
			wrapMode: Label.WordWrap
			Layout.fillWidth: true
		}

		TextArea {
			id: textArea
			focus: true
			Layout.fillWidth: true
			Layout.fillHeight: true
		}
	}
}
