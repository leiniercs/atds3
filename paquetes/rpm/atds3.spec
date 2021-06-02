Name:			atds3
Version:		0.7.0
Release:		1%{?dist}
Summary:		Administrador de Transferencias para toDus (S3)

License:		BSD

Source0:		atds3-0.7.0.tar.gz

BuildArch:		x86_64

BuildRequires:	rpm-build rpm-devel rpmlint rpmdevtools coreutils diffutils patch gcc make openssl-devel protobuf-devel protobuf-compiler qt5-qtbase-devel qt5-qtsvg-devel qt5-qtbase qt5-qtbase-gui qt5-qtsvg

Requires:		openssl-libs protobuf-devel qt5-qtbase qt5-qtbase-gui qt5-qtsvg

%description
ATDS3 es una aplicación para escritorio que automatiza el proceso de descarga y subida de archivos desde/hacia los servidores de la red toDus (S3).

ATDS3 posee las siguientes características:

 * Multiplataforma: ATDS3 puede ser utilizado en cualquier sistema operativo que en donde la librería Qt pueda funcionar: UNIX (FreeBSD, NetBSD, OpenBSD), Linux, macOS y Windows.
 * Fácil de usar: ATDS3 es fácil de usar gracias a la intuitiva interfaz de usuario que posee.
 * Configurable: ATDS3 ofrece diversas opciones configurables que definen el comportamiento de diversas secciones.
 * Completamente asincrónico: ATDS3 es totalmente asincrónico, por lo que podrá realizar varias operaciones de descargas y subidas al mismo tiempo.
 * Ajustado al protocolo de red de toDus: ATDS3 Está ajustado lo mayormente posible al protocolo de red que utiliza toDus, incluyendo inicios de sesión partiendo del número telefónico.
 * Inteligene: ATDS3 tiene escrito código para comportarse de forma inteligente dependiendo de la situación de la red y cómo descargar los archivos.

%prep
%setup -q
protoc --proto_path=%{_builddir}/%{name}-%{version}/documentos/protobuf --cpp_out=%{_builddir}/%{name}-%{version} todus.proto
mv %{_builddir}/%{name}-%{version}/todus.pb.h %{_builddir}/%{name}-%{version}/cabeceras/
mv %{_builddir}/%{name}-%{version}/todus.pb.cc %{_builddir}/%{name}-%{version}/fuentes/
qmake-qt5 %{_builddir}/%{name}-%{version}/%{name}.pro QMAKE_PREFIX=%{buildroot} QMAKE_CFLAGS_ISYSTEM=-I QMAKE_CXXFLAGS+=-g CONFIG-=debug CONFIG-=qml_debug CONFIG-=qtquickcompiler CONFIG+=release CONFIG+=protobuf

%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}%{_bindir} %{buildroot}%{_datadir}/pixmaps %{buildroot}%{_datadir}/applications
install -m 0755 %{_builddir}/%{name}-%{version}/%{name} %{buildroot}%{_bindir}/%{name}
install -m 0644 %{_builddir}/%{name}-%{version}/recursos/iconos/%{name}.svg %{buildroot}%{_datadir}/pixmaps/%{name}.svg
install -m 0644 %{_builddir}/%{name}-%{version}/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop

%check
strip %{buildroot}%{_bindir}/%{name}

%files
%license LICENSE
%{_bindir}/%{name}
%{_datadir}/pixmaps/%{name}.svg
%{_datadir}/applications/%{name}.desktop

%changelog
* Wed Jun 02 2021 No maintainer <no@maintain.er> - 0.7.0-1
- Reemplazado código base de comunicación HTTP con los servidores de la red toDus. Ahora es más sólido y eficiente.
- Incrustada la librería Protocol Buffers en los binarios para Windows.
- Mejorado el gestor de descargas.
- Mejoradas las descargas.
- Mejorada la reanudación de la descarga cuando se congela.
- Agregada la habilidad de agrupar en una carpeta las descargas que conforman un conjunto de archivos compromidos por volúmenes.
- Actualizado agente de usuario utilizado en las peticiones HTTP hacia los servidores de la red toDus.
- Actualizado el número de versión utilizado en las peticiones HTTP hacia los servidores de la red toDus.
* Fri May 28 2021 No maintainer <no@maintain.er> - 0.6.1-1
- Corregido un error en la sección de reanudación automática de las descargas que provocaba que las descargas se iniciaran múltipleces veces.
* Fri May 28 2021 No maintainer <no@maintain.er> - 0.6.0-1
- Corrigido un error en el módulo de inicio de sesión de toDus.
- Corrigido un error en hacia fallar al programa cuando se cambiaba el número de teléfono y/o la ficha de acceso.
- Mejorado el módulo de descargas.
- Se detecta correctamente cuando se queda colgada una descarga.
- Se reanudan las descargas automáticamente si se quedan colgadas o si no obtuvieron correctamente el enlace firmado de la red toDus.
- Mejorada el mecanismo del gestor de descargas para ofrecer descargas contínuas sin interrupciones.
- Incrustada la dependencia 'Google Protocol Buffers' al binario para FreeBSD y Linux (solo derivados de Debian GNU/Linux).
- Eliminada la dependencia de 'Google Protocol Buffers' de los paquetes FreeBSD y DEB.
* Wed May 26 2021 No maintainer <no@maintain.er> - 0.5.0-1
- Elimina la dependencia de la librería SQLite3.
- Mejorado el rendimiento de la aplicación considerablemente. Ahora todo es más fluido.
- Mejorad el proceso de actualización y visualización de los campos de las descargas. Ahora todo es más fluido.
- Agregado soporte Unicode al procesado de los archivos de descargas.
- Agregado el icono del sistema a la barra de herramientas del sistema.
- La aplicación continúa corriendo en segundo plano al cerrarse si existen descargas activas.
- La aplicación se cierra completamente al cerrarse si no existen descargas activas.
- Corrigidas las operaciones a ejecutar cuando se cambia de número de teléfono o ficha de acceso.
- Cambiada la clave de cifrado para proteger la contraseña del proxy.
- Cambiado el tamaño mínimo de la aplicación a 705x430 píxeles.
* Mon May 24 2021 No maintainer <no@maintain.er> - 0.4.0-1
- Se agrega una nueva dependencia: SQLite3.
- Se maneja el acceso a la base de datos del programa a bajo nivel para obtener mayor rendimiento.
- Mejorado considerablemente el sistema de descargas. Todo es más fluido ahora.
- Se cambia la visualización del porciento completado de la descarga a una barra de progreso.
- Mejorada la operación 'Agregar descargas desde archivo'.
- Mejorada la operación 'Eliminar descarga'.
- Mejorada la operación 'Eliminar todas las descargas'.
- Mejorada la operación 'Iniciar descarga'.
- Mejorada la operación 'Iniciar todas las descargas'.
- Mejorada la operación 'Pausar descarga'.
- Mejorada la operación 'Pausar todas las descargas'.
- Mejoradas las operaciones de actualización de los parámetros de cada descarga. Todo es más fluido ahora.
- Agregado nuevo estado de descarga: 'Fallo' (círculo con barra diagonal). Este estado se asigna siempre que se detecte un fallo en la descarga, tal como enlace expirado, no encontrado y prohibido.
- Las descargas con errores no se pueden iniciar ni pausar, debido a que son descargas de enlaces expirados o prohibidos.
- Agregado un nuevo formato de archivo de descargas: S3P (S3 Plano: .s3p). Este formato es el mismo que el clásico TXT. Con esto se preparan las bases para la implementación de nuevos formatos de descargas que estarán disponibles en próximas versiones.
* Sun May 23 2021 No maintainer <no@maintain.er> - 0.3.0-1
- Definición de variables requeridas para la generación del binario para Windows.
- Modificado el comportamiento cuando se detecta el rechazo de la autentificación en la sesión toDus para obtener una reconexión sin solicitar el código por SMS.
- Se corrige el comportamiento del botón 'Iniciar todas las descargas' en la categoría 'Descargas'.
- Agregada la configuración 'Número de versión de toDus' en la ventana 'Configuración'.
- Corregida la cabecera del listado de archivos procesados en la ventana 'Agregar descargas desde archivo'.
- Se recuerda la última categoría seleccionada en las ventanas 'Agregar descarga' y 'Agregar descargas desde archivo'.
- Se recuerda el valor de la casilla de verificación 'Iniciar descarga?' en las ventanas 'Agregar descarga' y 'Agregar descargas desde archivo'.
- Se recuerda el último directorio utilizado en la ventana 'Agregar descargas desde archivo'.
- Se actualiza el listado de la categoría 'Descarga' cada vez que finaliza una descarga.
- Mejorado el sistema de actualización de los campos de las descargas.
- Corregida la visualización de la barra de progreso del campo 'Completado' del listado de las descargas.
- Optimización en la visualización de los campos 'Estado', 'Completado' y 'Velocidad' en el listado de descargas.
- Se actualiza el listado de la categoría a la que pertenece una descarga al eliminarse tras finalizar.
- Visualización correcta de los estados de las descargas al iniciar la aplicación.
- Mejorada la operación 'Agregar descarga'.
- Mejorada la operación 'Agregar descargas desde archivo'.
- Mejorada la operación 'Eliminar descarga'.
- Mejorada la operación 'Eliminar todas las descargas'.
- Mejorada la operación 'Iniciar descarga'.
- Mejorada la operación 'Iniciar todas las descargas'.
- Mejorada la operación 'Pausar descarga'.
- Mejorada la operación 'Pausar todas las descargas'.
- Mejorado el gestor de descargas.
- Las descargas autoreconectan en caso de fallo de red que se pueda recuperar.
* Fri May 21 2021 No maintainer <no@maintain.er> - 0.2.0-1
- Cambiado el mínimo del tamaño de la ventana hacia 1/3 de la pantalla primaria o 600x400.
- Se recuerda el tamaño de la geometría de la ventana principal al inicar.
- Se recuerda la última categoría seleccionada al iniciar.
- Se mejora el mecanismo de arrastrar y soltar. Ahora se pueden arrastrar archivos desde Telegram (descargar primero en Telegram).
- Se cambió el nombre de la categoría 'Descargando' hacia 'Descargas'.
- La categoría 'Descargas' muestra todas las descarga en todos sus estados.
- Se puede iniciar, detener y eliminar descarga desde la categoria 'Descargas'.
- Se pueden eliminar descargas finalizadas desde la categoria 'Finalizadas'.
* Thu May 20 2021 No maintainer <no@maintain.er> - 0.1.0-1
- Construida la ventana principal.
- Agregada la barra de herramientas en la 'Ventana principal'.
- Agregada el listado de categorías en la 'Ventana principal'.
- Agregada el listado de descargas en la 'Ventana principal'.
- Construida la ventana 'Agregar descarga'.
- Habilidad de agregar una descarga desde la ventana 'Agregar descarga'.
- Construida la ventana 'Agregar descargas desde archivo'.
- Habilidad de agregar varias descargas desde la ventana 'Agregar descargas desde archivo'.
- Habilidad de eliminar una descarga del listado.
- Habilidad de eliminar todas las descargas del listado.
- Habilidad de iniciar una descarga.
- Habilidad de pausar una descarga.
- Habilidad de iniciar todas las descargas.
- Habilidad de pausar todas las descargas.
- Construida la ventana 'Configuración'
- Agregada la categoría 'toDus' en la ventana 'Configuración'.
- Implementada la configuración 'Número de teléfono' en la categoría 'toDus'.
- Implementada la configuración 'Ficha de acceso' en la categoría 'toDus'.
- Agregada la categoría 'Descargas' en la ventana 'Configuración'.
- Implementada la configuración 'Ruta de guardado' en la categoría 'Descargas'.
- Implementada la configuración 'Total de descargas paralelas' en la categoría 'Descargas'.
- Implementada la configuración 'Eliminar del listado al finalizar?' en la categoría 'Descarga'.
- Agregada la categoría 'Proxy' en la ventana 'Configuración'.
- Implementada la configuración 'Tipo de proxy' en la categoría 'Proxy'.
- Implementada la configuración 'Servidor' en la categoría 'Proxy'.
- Implementada la configuración 'Puerto' en la categoría 'Proxy'.
- Implementada la configuración 'Usuario' en la categoría 'Proxy'.
- Implementada la configuración 'Contraseña' en la categoría 'Proxy'.
- Agregada la categoría 'Avanzadas' en la ventana 'Configuración'.
- Implementada la configuración 'IP Servidor Autentificación' en la categoría 'Avanzadas'.
- Implementada la configuración 'Puerto Servidor Autentificación' en la categoría 'Avanzadas'.
- Implementada la configuración 'Nombre DNS Servidor Autentificación' en la categoría 'Avanzadas'.
- Implementada la configuración 'IP Servidor Sesión' en la categoría 'Avanzadas'.
- Implementada la configuración 'Puerto Servidor Sesión' en la categoría 'Avanzadas'.
- Implementada la configuración 'Nombre DNS Servidor Sesión' en la categoría 'Avanzadas'.
- Implementada la configuración 'IP Servidor S3' en la categoría 'Avanzadas'.
- Implementada la configuración 'Puerto Servidor S3' en la categoría 'Avanzadas'.
- Implementada la configuración 'Nombre DNS Servidor S3' en la categoría 'Avanzadas'.
- Implementada la configuración 'Nombre Agente Usuario' en la categoría 'Avanzadas'.
- Habilidad de iniciar sesión en la red toDus partiendo del número de teléfono.
- Habilidad de iniciar sesión en la red toDus partiendo de la ficha de acceso obtenida desde otra fuente.
- Habilidad de mostrar el estado del progreso del inicio de sesión en la red toDus.
- Se muestra el nombre y la foto de perfil del usuario después de iniciar la sesión en la red toDus.
- Habilidad de reconectar la sesión toDus si se interrumpe la conexión.
- Habilidad de descargar un archivo desde la red toDus.
- Habilidad de mostrar el progreso de la descarga de un archivo desde la red toDus.
- Habilidad de arrastrar y soltar archivos con extensión 'txt' desde otras ventanas.
