import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12


RowLayout {
	Layout.alignment: Qt.AlignLeft | Qt.AlignTop
	Layout.minimumHeight: tamanoIconos === 48 ? 40 : 32
	Layout.maximumHeight: tamanoIconos === 48 ? 40 : 32
	Layout.fillWidth: true
	spacing: 8

	Image {
		id: iconoEstado
		Layout.minimumWidth: anchoCabeceraEstado
		Layout.maximumWidth: anchoCabeceraEstado
		fillMode: Image.PreserveAspectFit
		sourceSize.height: tamanoIconos === 48 ? 24 : 16
		sourceSize.width: tamanoIconos === 48 ? 24 : 16
		state: model.estado
/*
		ColorOverlay {
			anchors.fill: parent
			source: parent
			color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
		}
*/
		states: [
			State {
				name: Paquetes.Estados.Pausado
				PropertyChanges {
					target: iconoEstado
					source: "qrc:/svg/pause.svg"
					rotation: 0
				}
			},
			State {
				name: Paquetes.Estados.EnEsperaIniciar
				PropertyChanges {
					target: iconoEstado
					source: "qrc:/svg/clock.svg"
					rotation: 0
				}
			},
			State {
				name: Paquetes.Estados.Iniciado
				PropertyChanges {
					target: iconoEstado
					source: "qrc:/svg/play.svg"
					rotation: 0
				}
			},
			State {
				name: Paquetes.Estados.Finalizado
				PropertyChanges {
					target: iconoEstado
					source: "qrc:/svg/check.svg"
					rotation: 0
				}
			},
			State {
				name: Paquetes.Estados.Error
				PropertyChanges {
					target: iconoEstado
					source: "qrc:/svg/skull.svg"
					rotation: 0
				}
			}
		]

		NumberAnimation on rotation {
			from: 0
			to: 360
			running: model.estado === Paquetes.Estados.EnEsperaIniciar
			duration: 1500
			loops: Animation.Infinite
		}
	}

	Label {
		Layout.fillWidth: true
		color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
		elide: Label.ElideMiddle
		text: model.nombre !== undefined ? decodeURIComponent(model.nombre) : ""
		verticalAlignment: Qt.AlignVCenter
	}

	Label {
		Layout.minimumWidth: anchoCabeceraTamano
		Layout.maximumWidth: anchoCabeceraTamano
		color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
		text: model.tamano !== undefined ? representarTamano(model.tamano) : "0B"
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
	}

	Label {
		Layout.minimumWidth: anchoCabeceraTamanoTransferido
		Layout.maximumWidth: anchoCabeceraTamanoTransferido
		color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
		text: model.tamanoTransferido !== undefined ? representarTamano(model.tamanoTransferido) : "0B"
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
	}

	RowLayout {
		Layout.minimumWidth: anchoCabeceraCompletado
		Layout.maximumWidth: anchoCabeceraCompletado
		spacing: 4

		ProgressBar {
			Layout.fillWidth: true
			indeterminate: model.estado === Paquetes.Estados.EnEsperaIniciar
			from: 0
			to: 100
			value: model.completado !== undefined ? model.completado : 0
		}
		Label {
			color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
			text: model.completado !== undefined ? `${model.completado}%` : "0%"
			verticalAlignment: Qt.AlignVCenter
		}
	}

	Label {
		Layout.minimumWidth: anchoCabeceraVelocidad
		Layout.maximumWidth: anchoCabeceraVelocidad
		color: model.error === 1 ? Material.color(Material.Red) : Material.foreground
		text: model.velocidad !== undefined ? `${representarTamano(model.velocidad)}/s` : "0B/s"
		horizontalAlignment: Qt.AlignHCenter
		verticalAlignment: Qt.AlignVCenter
	}
}
