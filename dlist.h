#ifndef __DLIST_H_INC__
#define __DLIST_H_INC__

#ifdef __cplusplus
extern "C"{
#endif

/**
	\file
	\brief Gestión de lista doblemente enlazada para contener datos genéricos
	\author PMCL
	\date 14/06/2018

	A través de la gestión de un cursor que recorre la lista se consigue un almacén de datos
	dinámico, con sus operaciones de posición, borrado, inserción, etc.  La memoria de los datos
	no se maneja en esta lista, el programador es el encargado de gestionar dicha memoria.
*/

#define DLIST_NO_VALID_POSITION -1 /**< Posición no válida. */
#define DLIST_ERROR -2 /**< Error que devuelven algunas de las funciones de esta librería. */
#define DLIST_OK 0 /**< Indicador de operación correcta devuelto por algunas de las funciones de esta librería. */
#define DLIST_TRUE 1 /**< Indicador de verdadero usado por las funciones de esta librería. */
#define DLIST_FALSE 0 /**< Indicador de falso usado por las funciones de esta librería. */

/**
	\struct dlListNode
	\brief Representación de un nodo en una lista.
	\warning No usar para acceder/manipular los datos, usar para ello las funciones proporcionadas.
*/
typedef struct dlListNode
{
	struct dlListNode *pPrior; /**< Puntero al elemento anterior al actual. */
	struct dlListNode *pNext; /**< Puntero al siguiente elemento. */
	void *data; /**< Puntero a los datos apuntados. */

} dlListNode;

/**
	\struct dlList
	\brief Representación de una lista.
	\warning No usar para acceder/manipular los datos, usar para ello las funciones proporcionadas.

	Una lista está representada por n elementos, se puede acceder a ellos a través de un índice, el cuál
	empieza por 0, así el tratamiento se puede hacer parecido a un array de elementos en un for,
	por ejemplo.
*/
typedef struct dlList
{
	dlListNode *pFirst; /**< Puntero al primer elemento de la lista. */
	dlListNode *pLast; /**< Puntero al último elemento de la lista. */
	dlListNode *pActual; /**< Puntero al elemento actual de la lista. */
	unsigned int size; /**< Número de elementos que contiene la lista. */
	int position; /**< Posición actual del puntero en la lista, un valor negativo indica una posición no válida, como cuando la lista está vacía. */
} dlList;

/**
	\typedef void (* dlist_callBackFunction)(void *data)
	\brief Tipo de datos que deben tener las funciones de callback de dlist.
	\param data Puntero con la dirección de memoria de un dato de la lista sobre el que se realizará la operación.
*/
typedef void (*dlist_callBackFunction)(void *data);

/**
	\fn dlList* dlist_new()
	\brief Crea una nueva lista.
	\return Un puntero a la estructura creada.
	\retval NULL Si no se ha podido crear la lista.
*/
dlList* dlist_new();

/**
	\fn void dlist_free(dlList *list)
	\brief Elimina una lista.
	\param list Puntero a la lista a eliminar.
	\warning Esta función no elimina los datos, eso es tarea del programador.
*/
void dlist_free(dlList *list);

/**
	\fn void dlist_freeAll(dlList *list)
	\brief Elimina una lista y sus datos enlazados.
	\param list Puntero a la lista a eliminar.
	\warning Esta función hace un free(), no sirve para eliminar todo tipo de datos, sólo para aquellos que se puedan liberar con un simple free().
*/
void dlist_freeAll(dlList *list);

/**
	\fn int dlist_add(dlList *list, void *data)
	\brief Añade un dato al final de la lista.
	\param list Puntero a la lista a la que queremos añadir un dato.
	\param data Puntero al dato que queremos añadir a la lista.
	\return Posición del dato introducido si todo ha ido bien.
	\retval DLIST_ERROR Error devuelto si no se ha podido introducir el dato en la lista.
*/
int dlist_add(dlList *list, void *data);

/**
	\fn int dlist_insert(dlList *list, void *data)
	\brief Inserta un dato antes de la posición del puntero actual de la lista.
	\param list Puntero a la lista a la que queremos añadir un dato.
	\param data Puntero al dato que queremos añadir a la lista.
	\return Posición del dato introducido si todo ha ido bien.
	\retval DLIST_ERROR Error devuelto si no se ha podido introducir el dato en la lista.
*/
int dlist_insert(dlList *list, void *data);

/**
	\fn int dlist_del(dlList *list)
	\brief Elimina el elemento de la lista que está siendo apuntado por el puntero de la posición actual.
	\param list Puntero a la lista de la que queremos eliminar un elemento.
	\return Posición actual del cursor, incluido DLIST_NO_VALID_POSITION si se ha eliminado el único elemento de una lista.
	\retval DLIST_ERROR Error devuelto si no se ha podido eliminar un dato, en este caso no se ha llegado a realizar ninguna operación sobre la lista.
	\warning Esta función no elimina los datos almacenados, sólo la posición en la lista, es responsabilidad del programador tratar la memoria en este caso.
*/
int dlist_del(dlList *list);

/**
	\fn void dlist_clear(dlList *list)
	\brief Elimina todos los elementos de la lista.
	\param list Puntero a la lista de la que queremos eliminar los elementos.
	\warning Esta función no eliminada los datos almacenados, sólo las posiciones en la lista, es responsabilidad del programador tratar la memoria en este caso.
*/
void dlist_clear(dlList *list);

/**
	\fn void* dlist_get(dlList *list)
	\brief Devuelve el elemento apuntado actualmente en la lista.
	\param list Puntero a la lista de la que queremos recuperar el dato.
	\return Puntero a los datos almacenados.
	\retval NULL En caso de que haya ocurrido algún fallo.
*/
void* dlist_get(dlList *list);

/**
	\fn void* dlist_getAt(dlList *list, unsigned int position)
	\brief Devuelve el elemento de la posición que deseemos.
	\param list Puntero a la lista de la que queremos recuperar el dato.
	\param position Posición de la que queremos recuperar los datos.
	\return Puntero a los datos almacenados.
	\retval NULL En caso de que haya ocurrido algún fallo.
*/
void* dlist_getAt(dlList *list, unsigned int position);

/**
	\fn int dlist_next(dlList *list)
	\brief Mueve el puntero al elemento siguiente.
	\param list Puntero a la lista de la que queremos mover el puntero.
	\return Posición actual del puntero.
	\retval DLIST_ERROR Error devuelto en caso de que no se hubiese podido realizar la operación.
*/
int dlist_next(dlList *list);

/**
	\fn int dlist_prev(dlList *list)
	\brief Mueve el puntero al elemento anterior.
	\param list Puntero a la lista de la que queremos mover el puntero.
	\return Posición actual del puntero.
	\retval DLIST_ERROR Error devuelto en caso de que no se hubiese podido realizar la operación.
*/
int dlist_prev(dlList *list);

/**
	\fn int dlist_seek(dlList *list, unsigned int position)
	\brief Mueve el puntero a la posición deseada.
	\param list Puntero a la lista de la que queremos mover el puntero.
	\param position Posición a la que queremos mover el puntero.
	\return Posición actual del puntero.
	\retval DLIST_ERROR Error devuelto en caso de que no se hubiese podido realizar la operación.
*/
int dlist_seek(dlList *list, unsigned int position);

/**
	\fn int dlist_tell(dlList *list)
	\brief Devuelve la posición actual del puntero.
	\param list Puntero a la lista de la que queremos saber la posición.
	\return Posición actual del puntero.
	\retval DLIST_NO_VALID_POSITION Error devuelto en el caso de que no haya posición válida.
*/
int dlist_tell(dlList *list);

/**
	\fn int dlist_eol(dlList *list)
	\brief Indica si estamos en el final de la lista.
	\param list Puntero a la lista de la que queremos saber si estamos en el final.
	\retval DLIST_TRUE si estamos en el final de la lista.
	\retval DLIST_FALSE en caso contrario.
	\warning En caso de que la lista no esté creada devolverá DLIST_FALSE.  En caso de no contener datos devolverá DLIST_TRUE.
*/
int dlist_eol(dlList *list);

/**
	\fn int dlist_sol(dlList *list)
	\brief Indica si estamos en el principio de la lista.
	\param list Puntero a la lista de la que queremos saber si estamos en el principio.
	\retval DLIST_TRUE Si estamos en el principio de la lista.
	\retval DLIST_FALSE En caso contrario.
	\warning En caso de que la lista no esté creada devolverá DLIST_FALSE.  En caso de no contener datos devolverá DLIST_TRUE.
*/
int dlist_sol(dlList *list);

/**
	\fn int dlist_len(dlList *list)
	\brief Esta función nos devuelve el número de elementos de la lista.
	\param list Puntero a la lista de la que queremos saber cuántos elementos tiene.
	\return Número de elementos de la lista si todo está correcto.
	\retval DLIST_ERROR Error devuelto si no se ha podido recuperar el número de elementos de la lista.
*/
int dlist_len(dlList *list);

/**
	\fn int dlist_foreach(dlList *list, dlist_callBackFunction fnc)
	\brief Realiza una operacion sobre toda la lista.
	\param list Lista sobre la que realizar la operacion.
	\param fnc Funcion de callback a aplicar sobre los elementos de la lista.
	\retval DLIST_OK En el caso de que todo haya ido bien.
	\retval DLIST_ERROR En el caso de que haya ocurrido algún error.
*/
int dlist_foreach(dlList *list, dlist_callBackFunction fnc);

/**
	\fn void** dlist_generateArray(dlList *list)
	\brief Esta función crea un array de punteros de forma dinámica con los datos contenidos en la lista.
	\param list Lista de la que extraer los datos para el array.
	\return Puntero al primer elemento del array.
	\retval NULL En caso de que haya ocurrido algún error.

	Esta función crea un array de punteros de forma dinámica apuntando a los datos
	en el mismo orden en el que están en la lista.  La lista no se elimina ni se hace
	ningún tratamiento con los datos.  El array creado es fijo, por lo que si se añaden
	o eliminan datos en la lista no se verán reflejados en el array, es tarea del
	programador hacer este tratamiento, teniendo que eliminar y crear de nuevo el array
	si se ha modificado el número de elementos, para el caso de una reasignación de
	elementos se pueden reasignar los punteros.
*/
void** dlist_generateArray(dlList *list);

#ifdef __cplusplus
}
#endif

#endif // __DLIST_H_INC__
