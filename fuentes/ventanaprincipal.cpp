#include "ventanaprincipal.hpp"
#include "delegacioniconoestado.hpp"
#include "delegacionbarraprogreso.hpp"
#include "delegacionvelocidad.hpp"
#include "modeloentradas.hpp"
#include "ventanaagregardescarga.hpp"
#include "main.hpp"
#include <QVBoxLayout>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QListView>
#include <QProgressBar>
#include <QTreeView>
#include <QHeaderView>
#include <QSqlRecord>


VentanaPrincipal::VentanaPrincipal(QWidget *parent)
	: QMainWindow(parent) {
	Q_INIT_RESOURCE(iconos);
	construirIU();
}
VentanaPrincipal::~VentanaPrincipal() {
	// Emitir la señal de detención de la aplicación para que todos los hilos de descargas cesen su ejecución
	emit detenerEjecucion();
}

/**
 * Agrega la descarga especificada por el usuario en la ventana 'Agrega descarga'
 */
void VentanaPrincipal::agregarDescarga() {
	_NuevaDescarga datos = _ventanaAgregarDescarga->obtenerDatosDescarga();
	QSqlDatabase bd = QSqlDatabase::database();
	ModeloEntradas *modelo;

	switch (datos.categoria) {
		case _ListadoCategorias::Programas:
			modelo = _modeloCategoriaProgramas.get();
			break;
		case _ListadoCategorias::Musica:
			modelo = _modeloCategoriaMusica.get();
			break;
		case _ListadoCategorias::Videos:
			modelo = _modeloCategoriaVideos.get();
			break;
		case _ListadoCategorias::Otros:
			modelo = _modeloCategoriaOtros.get();
			break;
	}

	bd.transaction();
	QSqlRecord registro = modelo->record();
	registro.remove(0); // Campo 'id'
	registro.setValue("categoria", datos.categoria);
	registro.setValue("estado", (datos.iniciar == true ? _ListadoEstados::EnEspera : _ListadoEstados::Pausada));
	registro.setValue("enlace", datos.enlace);
	registro.setValue("ruta", "");
	registro.setValue("nombre", datos.nombre);
	registro.setValue("completado", 0);
	registro.setValue("velocidad", 0);

	if(modelo->insertRecord(-1, registro)) {
		modelo->submitAll();
		bd.commit();
	} else {
		bd.rollback();
	}

	_modeloCategoriaDescargando->select();
}

/**
 * @brief Evento que se dispara cuando se hace clic en el botón 'Agregar descarga'
 */
void VentanaPrincipal::eventoAgregarDescarga() {
	_ventanaAgregarDescarga->limpiarCampos();
	_ventanaAgregarDescarga->show();
}

void VentanaPrincipal::eventoAgregarDescargasDesdeArchivo() {}

void VentanaPrincipal::eventoEliminarDescarga() {}

void VentanaPrincipal::eventoEliminarTodasDescargas() {}

void VentanaPrincipal::eventoIniciarDescarga() {}

void VentanaPrincipal::eventoPausarDescarga() {}

void VentanaPrincipal::eventoIniciarTodasDescargas() {}

void VentanaPrincipal::eventoPausarTodasDescargas() {}

void VentanaPrincipal::eventoConfiguracion() {}

void VentanaPrincipal::eventoAcerca() {}

/**
 * @brief Evento que se dispara cuando se selecciona un elemento en el listado de categorías
 * @param indice Índice del modelo
 */
void VentanaPrincipal::eventoCategoriaSeleccionada(const QModelIndex &indice) {
	switch (indice.row()) {
		case 0:
			_listadoDescargas->setModel(_modeloCategoriaDescargando.get());
			break;
		case 1:
			_listadoDescargas->setModel(_modeloCategoriaFinalizadas.get());
			break;
		case 2:
			_listadoDescargas->setModel(_modeloCategoriaProgramas.get());
			break;
		case 3:
			_listadoDescargas->setModel(_modeloCategoriaMusica.get());
			break;
		case 4:
			_listadoDescargas->setModel(_modeloCategoriaVideos.get());
			break;
		case 5:
			_listadoDescargas->setModel(_modeloCategoriaOtros.get());
			break;
	}

	if (indice.row() == 1) {
		_listadoDescargas->hideColumn(4);
		_listadoDescargas->hideColumn(5);
	} else {
		_listadoDescargas->showColumn(4);
		_listadoDescargas->showColumn(5);
	}
}

/**
 * @brief Construye los botones de la barra de herramientas
 * @param barraHerramientas Puntero al objeto de la barra de herramientas
 */
void VentanaPrincipal::construirBotonesBarraHerramientas(QToolBar *barraHerramientas) {

	QAction *accionAgregarDescarga = new QAction();
	accionAgregarDescarga->setIcon(QIcon(":/iconos/agregar.svg"));
	accionAgregarDescarga->setText("Agregar");
	accionAgregarDescarga->setToolTip("Agrega una descarga al listado");
	accionAgregarDescarga->setStatusTip("Agrega una descarga al listado");
	accionAgregarDescarga->setShortcut(QKeySequence(Qt::Key_Insert));
	connect(accionAgregarDescarga, &QAction::triggered, this, &VentanaPrincipal::eventoAgregarDescarga);
	barraHerramientas->addAction(accionAgregarDescarga);

	QAction *accionAgregarDescargasDesdeArchivo = new QAction();
	accionAgregarDescargasDesdeArchivo->setIcon(QIcon(":/iconos/agregar-desde-archivo.svg"));
	accionAgregarDescargasDesdeArchivo->setText("Agregar desde archivo");
	accionAgregarDescargasDesdeArchivo->setToolTip("Agrega un listado de descargas procesados desde un archivo");
	accionAgregarDescargasDesdeArchivo->setStatusTip("Agrega un listado de descargas procesados desde un archivo");
	accionAgregarDescargasDesdeArchivo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Insert));
	connect(accionAgregarDescargasDesdeArchivo, &QAction::triggered, this, &VentanaPrincipal::eventoAgregarDescargasDesdeArchivo);
	barraHerramientas->addAction(accionAgregarDescargasDesdeArchivo);

	QAction *accionEliminarDescarga = new QAction();
	accionEliminarDescarga->setIcon(QIcon(":/iconos/eliminar.svg"));
	accionEliminarDescarga->setText("Eliminar");
	accionEliminarDescarga->setToolTip("Elimina la(s) descarga(s) selecionada(s)");
	accionEliminarDescarga->setStatusTip("Elimina la(s) descarga(s) selecionada(s)");
	accionEliminarDescarga->setShortcut(QKeySequence(Qt::Key_Delete));
	connect(accionEliminarDescarga, &QAction::triggered, this, &VentanaPrincipal::eventoEliminarDescarga);
	barraHerramientas->addAction(accionEliminarDescarga);

	QAction *accionEliminarTodasDescargas = new QAction();
	accionEliminarTodasDescargas->setIcon(QIcon(":/iconos/limpiar.svg"));
	accionEliminarTodasDescargas->setText("Eliminar todas");
	accionEliminarTodasDescargas->setToolTip("Elimina todas las descargas del listado");
	accionEliminarTodasDescargas->setStatusTip("Elimina todas las descargas del listado");
	accionEliminarTodasDescargas->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
	connect(accionEliminarTodasDescargas, &QAction::triggered, this, &VentanaPrincipal::eventoEliminarTodasDescargas);
	barraHerramientas->addAction(accionEliminarTodasDescargas);

	barraHerramientas->addSeparator();

	QAction *accionIniciarDescarga = new QAction();
	accionIniciarDescarga->setIcon(QIcon(":/iconos/iniciar.svg"));
	accionIniciarDescarga->setText("Iniciar descarga");
	accionIniciarDescarga->setToolTip("Inicia la descarga seleccionada");
	accionIniciarDescarga->setStatusTip("Inicia la descarga seleccionada");
	accionIniciarDescarga->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	connect(accionIniciarDescarga, &QAction::triggered, this, &VentanaPrincipal::eventoIniciarDescarga);
	barraHerramientas->addAction(accionIniciarDescarga);

	QAction *accionPausarDescarga = new QAction();
	accionPausarDescarga->setIcon(QIcon(":/iconos/pausar.svg"));
	accionPausarDescarga->setText("Pausar descarga");
	accionPausarDescarga->setToolTip("Pausa la descarga seleccionada");
	accionPausarDescarga->setStatusTip("Pausa la descarga seleccionada");
	accionPausarDescarga->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	connect(accionPausarDescarga, &QAction::triggered, this, &VentanaPrincipal::eventoPausarDescarga);
	barraHerramientas->addAction(accionPausarDescarga);

	QAction *accionIniciarTodasDescargas = new QAction();
	accionIniciarTodasDescargas->setIcon(QIcon(":/iconos/iniciar-todas.svg"));
	accionIniciarTodasDescargas->setText("Iniciar todas las descargas");
	accionIniciarTodasDescargas->setToolTip("Inicia todas las descargas");
	accionIniciarTodasDescargas->setStatusTip("Inicia todas las descargas");
	accionIniciarDescarga->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_I));
	connect(accionIniciarTodasDescargas, &QAction::triggered, this, &VentanaPrincipal::eventoIniciarTodasDescargas);
	barraHerramientas->addAction(accionIniciarTodasDescargas);

	QAction *accionPausarTodasDescargas = new QAction();
	accionPausarTodasDescargas->setIcon(QIcon(":/iconos/pausar-todas.svg"));
	accionPausarTodasDescargas->setText("Pausar todas las descargas");
	accionPausarTodasDescargas->setToolTip("Pausa todas las descargas");
	accionPausarTodasDescargas->setStatusTip("Pausa todas las descargas");
	accionPausarTodasDescargas->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_D));
	connect(accionPausarTodasDescargas, &QAction::triggered, this, &VentanaPrincipal::eventoPausarTodasDescargas);
	barraHerramientas->addAction(accionPausarTodasDescargas);

	barraHerramientas->addSeparator();

	QAction *accionConfiguracion = new QAction();
	accionConfiguracion->setIcon(QIcon(":/iconos/configurar.svg"));
	accionConfiguracion->setText("Configuración");
	accionConfiguracion->setToolTip("Configura datos de la aplicación");
	accionConfiguracion->setStatusTip("Configura datos de la aplicación");
	accionConfiguracion->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	connect(accionConfiguracion, SIGNAL(triggered(bool)), this, SLOT(eventoConfiguracion()));
	barraHerramientas->addAction(accionConfiguracion);

	barraHerramientas->addSeparator();

	QAction *accionAcerca = new QAction();
	accionAcerca->setIcon(QIcon(":/iconos/acerca.svg"));
	accionAcerca->setText("Acerca de");
	accionAcerca->setToolTip("Muestra información del creador");
	accionAcerca->setStatusTip("Muestra información del creador");
	connect(accionAcerca, SIGNAL(triggered(bool)), this, SLOT(eventoAcerca()));
	barraHerramientas->addAction(accionAcerca);

	addToolBar(barraHerramientas);
}

/**
 * @brief Construye las categorías
 * @return Puntero al elemento del listado de categorías
 */
QListView *VentanaPrincipal::construirListadoCategorias() {

	_modeloListadoCategorias = std::make_unique<QStandardItemModel>();

	QStandardItem *categoriaDescargando = new QStandardItem();
	categoriaDescargando->setIcon(QIcon(":/iconos/descarga.svg"));
	categoriaDescargando->setText("Descargando");
	categoriaDescargando->setToolTip("Descargas activas");
	_modeloListadoCategorias->appendRow(categoriaDescargando);

	QStandardItem *categoriaFinalizadas = new QStandardItem();
	categoriaFinalizadas->setIcon(QIcon(":/iconos/finalizado.svg"));
	categoriaFinalizadas->setText("Finalizadas");
	categoriaFinalizadas->setToolTip("Descargas finalizadas");
	_modeloListadoCategorias->appendRow(categoriaFinalizadas);

	QStandardItem *categoriaProgramas = new QStandardItem();
	categoriaProgramas->setIcon(QIcon(":/iconos/categoria-programas.svg"));
	categoriaProgramas->setText("Programas");
	categoriaProgramas->setToolTip("Categoría 'Programas'");
	_modeloListadoCategorias->appendRow(categoriaProgramas);

	QStandardItem *categoriaMusica = new QStandardItem();
	categoriaMusica->setIcon(QIcon(":/iconos/categoria-musica.svg"));
	categoriaMusica->setText("Música");
	categoriaMusica->setToolTip("Categoría 'Música'");
	_modeloListadoCategorias->appendRow(categoriaMusica);

	QStandardItem *categoriaVideo = new QStandardItem();
	categoriaVideo->setIcon(QIcon(":/iconos/categoria-videos.svg"));
	categoriaVideo->setText("Videos");
	categoriaVideo->setToolTip("Categoría 'Videos'");
	_modeloListadoCategorias->appendRow(categoriaVideo);

	QStandardItem *categoriaOtros = new QStandardItem();
	categoriaOtros->setIcon(QIcon(":/iconos/categoria-otros.svg"));
	categoriaOtros->setText("Otros");
	categoriaOtros->setToolTip("Categoría 'Otros'");
	_modeloListadoCategorias->appendRow(categoriaOtros);

	_listadoCategorias = std::make_unique<QListView>();
	_listadoCategorias->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_listadoCategorias->setFlow(QListView::TopToBottom);
	_listadoCategorias->setModel(_modeloListadoCategorias.get());
	_listadoCategorias->setUniformItemSizes(true);
	_listadoCategorias->setStyleSheet("QListView::item { width: 120px; }");
	_listadoCategorias->setWrapping(true);
	_listadoCategorias->setViewMode(QListView::IconMode);
	_listadoCategorias->setCurrentIndex(_modeloListadoCategorias->indexFromItem(categoriaDescargando));
	connect(_listadoCategorias.get(), &QListView::activated, this, &VentanaPrincipal::eventoCategoriaSeleccionada);

	return _listadoCategorias.get();
}

/**
 * @brief Inicializa la base de datos
 */
void VentanaPrincipal::inicializarBaseDatos() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

	db.setDatabaseName("darts3.db");
	db.open();

	/**
	 * Crea las estructuras de las tablas
	 */
	QSqlQuery solicitudSQL;
	solicitudSQL.exec("CREATE TABLE IF NOT EXISTS entradas (id INTEGER PRIMARY KEY AUTOINCREMENT, estado INTEGER NOT NULL, nombre TEXT NOT NULL, completado INTEGER DEFAULT 0, velocidad INTEGER DEFAULT 0, enlace TEXT NOT NULL, categoria INTEGER NOT NULL, ruta TEXT NOT NULL, totalADescargar INTEGER DEFAULT 0, totalDescargado INTEGER DEFAULT 0)");
}

/**
 * @brief Construye el elemento del listado de descargas
 * @return Puntero al elemento del listado de descargas
 */
QTreeView *VentanaPrincipal::construirListadoDescargas() {
	/**
	 * Modelo del listado de la categoría 'Descargando'
	 */
	_modeloCategoriaDescargando = std::make_unique<ModeloEntradas>();
	_modeloCategoriaDescargando->setFilter(QString("estado = %1 OR estado = %2").arg(_ListadoEstados::EnEspera).arg(_ListadoEstados::Iniciada));
	_modeloCategoriaDescargando->select();

	/**
	 * Modelo del listado de la categoría 'Finalizadas'
	 */
	_modeloCategoriaFinalizadas = std::make_unique<ModeloEntradas>();
	_modeloCategoriaFinalizadas->setFilter(QString("estado = %1").arg(_ListadoEstados::Finalizada));
	_modeloCategoriaFinalizadas->select();

	/**
	 * Modelo del listado de la categoría 'Programas'
	 */
	_modeloCategoriaProgramas = std::make_unique<ModeloEntradas>();
	_modeloCategoriaProgramas->setFilter(QString("categoria = %1").arg(_ListadoCategorias::Programas));
	_modeloCategoriaProgramas->select();

	/**
	 * Modelo del listado de la categoría 'Música'
	 */
	_modeloCategoriaMusica = std::make_unique<ModeloEntradas>();
	_modeloCategoriaMusica->setFilter(QString("categoria = %1").arg(_ListadoCategorias::Musica));
	_modeloCategoriaMusica->select();

	/**
	 * Modelo del listado de la categoría 'Videos'
	 */
	_modeloCategoriaVideos = std::make_unique<ModeloEntradas>();
	_modeloCategoriaVideos->setFilter(QString("categoria = %1").arg(_ListadoCategorias::Videos));
	_modeloCategoriaVideos->select();

	/**
	 * Modelo del listado de la categoría 'Otros'
	 */
	_modeloCategoriaOtros = std::make_unique<ModeloEntradas>();
	_modeloCategoriaOtros->setFilter(QString("categoria = %1").arg(_ListadoCategorias::Otros));
	_modeloCategoriaOtros->select();

	_listadoDescargas = std::make_unique<QTreeView>();
	_listadoDescargas->setAlternatingRowColors(true);
	_listadoDescargas->setEditTriggers(QAbstractItemView::NoEditTriggers);
	_listadoDescargas->setItemsExpandable(false);
	_listadoDescargas->setModel(_modeloCategoriaDescargando.get());
	_listadoDescargas->setMinimumSize(QSize((400*16)/9, 400));
	_listadoDescargas->setRootIsDecorated(false);
	_listadoDescargas->setSortingEnabled(true);
	_listadoDescargas->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	_listadoDescargas->header()->setStretchLastSection(true);
	_listadoDescargas->hideColumn(0);
	for(int i = 6; i < 10; i++) {
		_listadoDescargas->hideColumn(i);
	}
	_elementoIconoEstado = std::make_unique<DelegacionIconoEstado>();
	_elementoBarraProgreso = std::make_unique<DelegacionBarraProgreso>();
	_elementoVelocidad = std::make_unique<DelegacionVelocidad>();
	_listadoDescargas->setItemDelegateForColumn(1, _elementoIconoEstado.get());
	_listadoDescargas->setItemDelegateForColumn(3, _elementoBarraProgreso.get());
	_listadoDescargas->setItemDelegateForColumn(4, _elementoVelocidad.get());

	return _listadoDescargas.get();
}

/**
 * @brief Construye la interfaz de usuario
 */
void VentanaPrincipal::construirIU() {
	setWindowIcon(QPixmap(":/iconos/descarga.svg"));

	/**
	 * Barra de herramientas
	 */
	QToolBar *barraHerramientas = new QToolBar();
	barraHerramientas->setIconSize(QSize(48, 48));
	barraHerramientas->setMovable(false);

	construirBotonesBarraHerramientas(barraHerramientas);

	/**
	 * Listado de categorías
	 */
	QDockWidget *puertoCategorias = new QDockWidget();
	puertoCategorias->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	puertoCategorias->setFeatures(QDockWidget::DockWidgetMovable);
	puertoCategorias->setFixedWidth(128);

	puertoCategorias->setWidget(construirListadoCategorias());

	addDockWidget(Qt::LeftDockWidgetArea, puertoCategorias);

	/**
	 * Inicializa la base de datos
	 */
	inicializarBaseDatos();

	/**
	 * Construir elemento central: listado de descargas
	 */
	setCentralWidget(construirListadoDescargas());
	setContentsMargins(QMargins(5, 5, 5, 5));

	/**
	 * Construye la ventana 'Agregar descarga'
	 */
	_ventanaAgregarDescarga = std::make_unique<VentanaAgregarDescarga>(this);
	_ventanaAgregarDescarga->setModal(true);
	connect(_ventanaAgregarDescarga.get(), &VentanaAgregarDescarga::accepted, this, &VentanaPrincipal::agregarDescarga);

	statusBar()->showMessage("Listo");
}
