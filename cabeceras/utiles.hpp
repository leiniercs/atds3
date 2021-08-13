#ifndef _UTILES_HPP
#define _UTILES_HPP

#include <QObject>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTcpSocket>


class ModeloPaquetes;

class Utiles : public QObject {
	Q_OBJECT

	public:
		enum EstadosTodus {
			Desconocido,
			Disponible,
			Perdido
		};
		enum ProgramacionAcciones {
			Ninguna,
			ApagarPantalla,
			Suspender,
			Hibernar,
			Apagar
		};
		enum EstadosACPI {
			SuspenderHaciaRAM = 0x03,
			SuspenderHaciaDiscoDuro,
			ApagarSistema
		};

		Utiles(QObject *padre = nullptr);

		Q_INVOKABLE void establecerObjetoModeloPaquetes(ModeloPaquetes *objeto);

		Q_INVOKABLE void iniciarMonitorizacionConexionTodus();
		Q_INVOKABLE void crearBandejaIcono();
		Q_INVOKABLE void notificar(const QString &llave, bool valorPredeterminado, const QString &titulo, const QString &mensaje, const QString &sonido);
		Q_INVOKABLE void restablecerDatosFabrica();
		Q_INVOKABLE QString rutaDesdeURI(const QString &uri);
		Q_INVOKABLE void activarProgramacionInicioColaTransferencias(bool activar);
		Q_INVOKABLE void activarProgramacionFinalizarColaTransferencias(bool activar);

	private slots:
		void mostrarOcultarVentana(QSystemTrayIcon::ActivationReason razon);
		void verificarDisponibilidadTodus();
		void eventoConexionTodus();
		void eventoErrorConexionTodus(QAbstractSocket::SocketError errorSocalo);
		void verificarProgramacionInicioColaTransferencias();
		void verificarProgramacionFinalizarColaTransferencias();
		void ejecutarAccionApagarPantalla();
		void ejecutarAccionSuspender();
		void ejecutarAccionHibernar();
		void ejecutarAccionApagar();
#if defined(Q_OS_UNIX) && !defined(Q_OS_LINUX)
		void BSDSolicitarEstadoACPI(int estadoACPI);
#endif
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
		void LinuxSolicitarEstadoACPI(const std::string &estadoACPI);
#endif
#ifdef Q_OS_WINDOWS
		bool WindowsActivarPrivilegios(const std::string privilegio);
#endif

	private:
		ModeloPaquetes *_modeloPaquetes;
		QSettings _configuraciones;
		QSystemTrayIcon _bandejaIcono;
		QTimer _temporizadorVerificadorDisponibilidadTodus;
		QTcpSocket _socaloTCPDisponibilidadTodus;
		EstadosTodus _estadoDisponibilidadTodus;
		QTimer _temporizadorProgramacionInicioColaTransferencias;
		QTimer _temporizadorProgramacionFinalizarColaTransferencias;
};
#endif // _UTILES_HPP
