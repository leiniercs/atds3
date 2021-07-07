#ifndef _MODELOPAQUETES_HPP
#define _MODELOPAQUETES_HPP

#include "todus.hpp"
#include "http.hpp"
#include <QSqlTableModel>
#include <QSettings>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QTcpSocket>


class ModeloTareas;

class Paquete : public QObject {
	Q_OBJECT

	public:
		qint64 id;
		int fila;
		int tipo;
		int categoria;
		int formato;
		QString nombre;
		qint64 tamano;
		qint64 tamanoTransferido;
		int completado;
		int velocidad;
		QTimer temporizadorActualizacionCampos;
};

class Tarea : public QObject {
	Q_OBJECT

	public:
		qint64 paquete;
		int fila;
		QString nombre;
		QString ruta;
		QString enlace;
		qint64 tamano;
		qint64 tamanoTransferido;
		int completado;
		int velocidad;
		qint64 ultimoTiempoTransferencia;
		HTTP http;
		QFile archivo;
};

class ModeloPaquetes : public QSqlTableModel {
	Q_OBJECT

	public:
		enum Tipos {
			Publicacion,
			Descarga
		};
		enum Formatos {
			S3,
			Clasico
		};
		enum Estados {
			Pausado,
			EnEsperaIniciar,
			Iniciado,
			Finalizado,
			Error
		};
		QMap<qint64, ModeloTareas *> _modelosTareas;

		ModeloPaquetes(QObject *padre = nullptr);

		QHash<int, QByteArray> roleNames() const;
		QVariant data(const QModelIndex &indice, int rol = Qt::DisplayRole) const;
		bool setData(const QModelIndex &indice, const QVariant &valor, int rol = Qt::EditRole);

		Q_INVOKABLE void establecerFiltroCategoria(int categoria);
		Q_INVOKABLE void establecerModeloTareas(qint64 paquete, ModeloTareas *modelo);
		Q_INVOKABLE void eliminarModeloTareas(qint64 paquete);
		Q_INVOKABLE void eliminarFiltros();
		Q_INVOKABLE QVariantMap obtener(int fila) const;
		QVariantMap obtenerDesdeId(qint64 id) const;
		QVariantMap obtenerTareaDesdeId(qint64 id) const;
		void actualizarEnlaces(qint64 id, const QString &enlace, const QString &enlaceFirmado);
		void actualizarEnlaceFirmado(qint64 id, const QString &enlace);
		void actualizarEstado(qint64 id, Estados estado);
		Q_INVOKABLE void eliminar(int fila);
		Q_INVOKABLE void eliminarTodas();
		Q_INVOKABLE void agregarDescargasDesdeArchivos(QList<QUrl> archivos);
		Q_INVOKABLE void agregarPublicacion(const QString &titulo, QAbstractItemModel *modeloArchivos, int formato);
		Q_INVOKABLE void iniciar(int fila);
		Q_INVOKABLE void pausar(int fila);
		Q_INVOKABLE void iniciarTodas();
		Q_INVOKABLE void pausarTodas();
		Q_INVOKABLE void confirmarCodigo(const QString &codigo);
		void iniciarDescarga(qint64 paquete, qint64 id, const QString &enlace);
		void iniciarPublicacion(qint64 paquete, qint64 id, const QString &enlace, const QString &enlaceFirmado);
		void procesarColaEjecucion();
		Q_INVOKABLE void iniciarSesionToDus();
		Q_INVOKABLE void crearDirectorio(const QString &ubicacion);
		Q_INVOKABLE void notificar(const QString &llave, bool valorPredeterminado, const QString &titulo, const QString &mensaje, const QString &sonido);
		Q_INVOKABLE void restablecerDatosFabrica();

	private slots:
		void mostrarOcultarVentana(QSystemTrayIcon::ActivationReason razon);
		void verificarDisponibilidadTodus();
		void eventoConexionTodus();
		void eventoErrorConexionTodus(QAbstractSocket::SocketError errorSocalo);

	private:
		enum EstadosTodus {
			Desconocido,
			Disponible,
			Perdido
		};
		int _categoria;
		QSettings _configuraciones;
		toDus _toDus;
		QMap<qint64, QPointer<Paquete>> _paquetes;
		QMap<qint64, QPointer<Tarea>> _tareasPublicaciones;
		QMap<qint64, QPointer<Tarea>> _tareasDescargas;
		QSystemTrayIcon _bandejaIcono;
		QTimer _temporizadorVerificadorDisponibilidadTodus;
		QTcpSocket _socaloTCPDisponibilidadTodus;
		EstadosTodus _estadoDisponibilidadTodus;

		void procesarArchivoDescargaClasico(const QUrl &url);
		void actualizarCampo(int fila, const QString &campo, const QVariant &valor);
		void actualizarCampos(int fila, const QVariantMap &campos);
		bool corregirFila(int &fila, qint64 id);
		void eventoPaqueteActualizarCampos(qint64 idPaquete);
		void eventoTareaIniciada(qint64 id);
		void eventoTareaTotalBytesTransferidos(qint64 id, qint64 transferido);
		void eventoTareaDetenida(qint64 id);
		void eventoTareaDetenidaParaReiniciar(qint64 id);
		void eventoTareaFinalizada(qint64 id);
		void generarArchivoDescarga(qint64 paquete);
		void generarArchivoDescargaS3(const QVariantMap &registroPaquete);
		void generarArchivoDescargaClasico(const QVariantMap &registroPaquete);
		QString obtenerFiltroCategoria(int categoria);
		void agregarPaqueteAlListado(qint64 id, int fila);
		void eliminarPaqueteDelListado(qint64 id);
/*
		void agregarTareaAlListado(qint64 id, int fila);
		void eliminarTareaDelListado(qint64 id);
*/
};

#endif // _MODELOPAQUETES_HPP
