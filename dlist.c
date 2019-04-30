#include <string.h>
#include <stdlib.h>

#include "dlist.h"

/**
	\file
	\brief Implementación de código de dlist.h
	\author PMCL
	\date 14/06/2018
*/

/*
	Declaraciones de funciones privadas
*/
static dlListNode * getNodeAt(dlList *list, unsigned int position);

/*
	Definiciones de funciones públicas
*/

dlList* dlist_new()
{
	dlList *lista;
	lista = (dlList *) malloc(sizeof(dlList));
	if(lista == NULL)
	{
		return NULL;
	}

	memset(lista, 0x00, sizeof(dlList));
	lista->position = DLIST_NO_VALID_POSITION;

	return lista;
}

void dlist_free(dlList *list)
{
	dlListNode *pActual, *pNext;

	if(list == NULL)
	{
		return;
	}

	pActual = list->pFirst;
	while(pActual != NULL)
	{
		pNext = pActual->pNext;
		free(pActual);
		pActual = pNext;
	}

	free(list);
}

void dlist_freeAll(dlList *list)
{
	dlListNode *pActual, *pNext;

	if(list == NULL)
	{
		return;
	}

	pActual = list->pFirst;
	while(pActual != NULL)
	{
		pNext = pActual->pNext;
		free(pActual->data);
		free(pActual);
		pActual = pNext;
	}

	free(list);
}

int dlist_add(dlList *list, void *data)
{
	dlListNode *newNodo;

	newNodo = (dlListNode *) malloc(sizeof(dlListNode));
	if(newNodo == NULL)
	{
		return DLIST_ERROR;
	}
	memset(newNodo, 0x00, sizeof(dlListNode));
	newNodo->data = data;

	/* Si la lista está vacía metemos el nodo como el primer dato. */
	if(list->size == 0)
	{
		list->pFirst = newNodo;
		list->pLast = newNodo;
		list->pActual = newNodo;
		list->size = 1;
		list->position = 0;
	}
	else /* Si la lista no está vacía, reordenamos punteros para meter el elemento. */
	{
		list->pLast->pNext = newNodo;
		newNodo->pPrior = list->pLast;
		list->pLast = newNodo;
		list->size++;
	}

	return (list->size - 1);
}

int dlist_insert(dlList *list, void *data)
{
	dlListNode *newNodo, *pAnterior, *pPosterior;

	newNodo = (dlListNode *) malloc(sizeof(dlListNode));
	if(newNodo == NULL)
	{
		return DLIST_ERROR;
	}
	memset(newNodo, 0x00, sizeof(dlListNode));
	newNodo->data = data;

	/* Si la lista está vacía metemos el nodo como el primer dato. */
	if(list->size == 0)
	{
		list->pFirst = newNodo;
		list->pLast = newNodo;
		list->pActual = newNodo;
		list->size = 1;
		list->position = 0;
	}
	else /* Si la lista no está vacía, reordenamos punteros para meter el elemento. */
	{
		pAnterior = list->pActual->pPrior;
		pPosterior = list->pActual;
		if(list->position == 0)
		{
			newNodo->pPrior = NULL;
		}
		else
		{
			pAnterior->pNext = newNodo;
			newNodo->pPrior = pAnterior;
		}
		newNodo->pNext = pPosterior;
		pPosterior->pPrior = newNodo;

		if(list->position == 0)
		{
			list->pFirst = newNodo;
		}

		list->size++;
		list->position++;
	}

	if(list->position == 0)
	{
		return 0;
	}
	else
	{
		return list->position - 1;
	}
}

int dlist_del(dlList *list)
{
	dlListNode *pAnterior, *pPosterior;

	if(list == NULL)
	{
		return DLIST_ERROR;
	}

	if(list->pActual == NULL)
	{
		return DLIST_ERROR;
	}

	pAnterior = list->pActual->pPrior;
	pPosterior = list->pActual->pNext;

	free(list->pActual);
	list->size--;

	if(list->size == 0)
	{
		/* En caso de ser el único elemento */
		list->position = DLIST_NO_VALID_POSITION;
		list->pActual = NULL;
		list->pFirst = NULL;
		list->pLast = NULL;
	}
	else if(list->position == 0)
	{
		/* En caso de ser el primer elemento */
		pPosterior->pPrior = NULL;
		list->pActual = pPosterior;
		list->pFirst = list->pActual;
	}
	else if(list->position == list->size)
	{
		/* En caso de ser el último elemento */
		pAnterior->pNext = NULL;
		list->position--;
		list->pActual = pAnterior;
		list->pLast = list->pActual;
	}
	else
	{
		/* En cualquier otro caso la posición actual está entre medias del resto de elementos. */
		pAnterior->pNext = pPosterior;
		pPosterior->pPrior = pAnterior;
		list->pActual = pPosterior;
	}

	return list->position;
}

void dlist_clear(dlList *list)
{
	dlListNode *pActual, *pNext;

	if(list == NULL)
	{
		return;
	}

	if(list->size == 0)
	{
		return;
	}

	pActual = list->pFirst;
	while(pActual != NULL)
	{
		pNext = pActual->pNext;
		free(pActual);
		pActual = pNext;
	}
	list->pActual = NULL;
	list->pFirst = NULL;
	list->pLast = NULL;
	list->position = DLIST_NO_VALID_POSITION;
	list->size = 0;
}

void* dlist_get(dlList *list)
{
	if(list == NULL)
	{
		return NULL;
	}

	if(list->pActual == NULL)
	{
		return NULL;
	}

	return list->pActual->data;
}

void* dlist_getAt(dlList *list, unsigned int position)
{
	dlListNode *tmp;

	tmp = getNodeAt(list, position);
	if(tmp == NULL)
	{
		return NULL;
	}

	return tmp->data;
}

int dlist_next(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_ERROR;
	}

	if(list->pActual == NULL || list->position == list->size - 1)
	{
		return DLIST_ERROR;
	}

	list->position++;
	list->pActual = list->pActual->pNext;

	return list->position;
}

int dlist_prev(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_ERROR;
	}

	if(list->size == 0 || list->position == 0)
	{
		return DLIST_ERROR;
	}

	list->position--;
	list->pActual = list->pActual->pPrior;

	return list->position;
}

int dlist_seek(dlList *list, unsigned int position)
{
	dlListNode *tmp;

	tmp = getNodeAt(list, position);
	if(tmp == NULL)
	{
		position = DLIST_ERROR;
	}
	else
	{
		list->pActual = tmp;
		list->position = position;
	}

	return position;
}

int dlist_tell(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_NO_VALID_POSITION;
	}

	return list->position;
}

int dlist_eol(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_FALSE;
	}
	if(list->size == 0)
	{
		return DLIST_TRUE;
	}

	if(list->position == (((int)list->size) - 1))
	{
		return DLIST_TRUE;
	}
	else
	{
		return DLIST_FALSE;
	}
}

int dlist_sol(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_FALSE;
	}

	if(list->size == 0)
	{
		return DLIST_TRUE;
	}

	if(list->position == 0)
	{
		return DLIST_TRUE;
	}
	else
	{
		return DLIST_FALSE;
	}

}

int dlist_len(dlList *list)
{
	if(list == NULL)
	{
		return DLIST_ERROR;
	}

	return (int)list->size;
}

int dlist_foreach(dlList *list, dlist_callBackFunction fnc)
{
	dlListNode *tmp;

	if(list == NULL)
	{
		return DLIST_ERROR;
	}

	tmp = list->pFirst;
	while(tmp != NULL)
	{
		fnc(tmp->data);
		tmp = tmp->pNext;
	}

	return DLIST_OK;
}

void** dlist_generateArray(dlList *list)
{
	void **pArray;
	int contador = 0;
	dlListNode *tmp;

	if(list == NULL)
	{
		return NULL;
	}

	if(list->size == 0)
	{
		return NULL;
	}

	pArray = (void **) malloc(sizeof (void *) * list->size);
	if(pArray == NULL)
	{
		return NULL;
	}

	tmp = list->pFirst;
	while(tmp != NULL)
	{
		pArray[contador] = tmp->data;
		contador++;
		tmp = tmp->pNext;
	}

	return pArray;
}

/**
	\fn static dlListNode * getNodeAt(dlList *list, unsigned int position)
	\brief Devuelve un nodo de la lista en la posición que deseemos de la forma más eficiente posible.
	\param list Puntero a la lista de la que queremos recuperar el nodo.
	\param position Posición de la que queremos recuperar el nodo.
	\return Puntero al nodo elegido.
	\retval NULL En caso de que haya ocurrido algún fallo.
*/
static dlListNode * getNodeAt(dlList *list, unsigned int position)
{
	dlListNode *tmp;
	int posicionEnLista;

	if(list == NULL)
	{
		return NULL;
	}

	if(list->size == 0 || position > (list->size - 1))
	{
		return NULL;
	}

	// Como tenemos 3 punteros de posición, el actual, el inicial y el final, vemos de cuál está más cerca para empezar a buscar nuestro dato.
	if(position > list->size - 1 - position)
	{
		posicionEnLista = list->size - 1;
		tmp = list->pLast;
	}
	else
	{
		posicionEnLista = 0;
		tmp = list->pFirst;
	}

	if(abs(posicionEnLista - (int) position) > abs(list->position - (int)position))
	{
		posicionEnLista = list->position;
		tmp = list->pActual;
	}

	while(position != posicionEnLista)
	{

		if(position > posicionEnLista)
		{
			tmp = tmp->pNext;
			posicionEnLista++;
		}
		else
		{
			tmp = tmp->pPrior;
			posicionEnLista--;
		}
	}

	return tmp;
}
