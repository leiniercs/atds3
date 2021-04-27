#ifndef MAIN_HPP
#define MAIN_HPP


#include <QString>


/**
 * @brief Título de la aplicación
 */
extern QString _aplicacionTitulo;

/**
 * @brief Versión de la aplicación
 */
extern QString _aplicacionVersion;

/**
 * @brief Listado de categorías
 */
enum _ListadoCategorias {
	Programas = 0x01,
	Musica = 0x02,
	Videos = 0x03,
	Otros = 0x04
};

/**
 * @brief Listado de estados
 */
enum _ListadoEstados {
	Pausada = 0x01,
	EnEspera = 0x02,
	Iniciada = 0x03,
	Finalizada = 0x04
};

#endif // MAIN_HPP

