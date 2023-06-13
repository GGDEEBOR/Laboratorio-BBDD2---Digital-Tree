#include <iostream>
#include <map>
#include <fstream>
#include <typeinfo>
#include <string>
using namespace std;

template <typename T>
class Nodo{
    public:
	   T clave;
	   map<char, Nodo<T>*> hijos;
	   bool _es_final_;
    public:
	   // constructor
	   Nodo(T key) {
		  clave = key;
	   }
	   virtual ~Nodo() {}

};

template <typename T>
class Hoja: public Nodo<T>{
    public:
	   T valorFinal;

    public:
	   // constructor
	   Hoja (T key, T finalValue): Nodo<T>(key) {
		  valorFinal = finalValue;
		  Nodo<T>::_es_final_ = true;
	   }

};

template <typename T>
class ArbolDigital{
    private:
	   Nodo<T>* raiz;

    public:
	   ArbolDigital() : raiz{nullptr} {};

	   void insertarR(Nodo<T>*& nodo, T key, T value) {
		  if (nodo == nullptr) {
			 nodo = new Hoja<T>(key, value);
			 return;
		  }

		  if (auto* hojaActual = dynamic_cast<Hoja<T>*>(nodo)) {
			 if (hojaActual->clave == key) {
				hojaActual->valorFinal = value;
				return;
			 }

			 auto* nuevoNodo = new Nodo<T>(key);
			 nuevoNodo->_es_final_ = false;

			 size_t i = 0;
			 while (i < hojaActual->clave.size() && i < key.size() && hojaActual->clave[i] == key[i]) {
				++i;
			 }

			 if (i == hojaActual->clave.size() && i == key.size()) {
				hojaActual->valorFinal = value;
				delete nuevoNodo;
				return;
			 }
			 if (i == hojaActual->clave.size()) {
				auto nuevaHoja = new Hoja<T>(key.substr(i), value);
				nuevoNodo->hijos[key[i]] = nuevaHoja;
			 } else if (i == key.size()) {
				nuevoNodo->hijos[hojaActual->clave[i]] = hojaActual;
				hojaActual->clave = hojaActual->clave.substr(i);
				hojaActual->valorFinal = value;
			 } else {
				auto nuevaHoja = new Hoja<T>(key.substr(i), value);
				nuevoNodo->hijos[key[i]] = nuevaHoja;
				nuevoNodo->hijos[hojaActual->clave[i]] = hojaActual;
				hojaActual->clave = hojaActual->clave.substr(i);
			 }

			 nodo = nuevoNodo;
			 return;
		  }

		  auto it = nodo->hijos.find(key[0]);
		  if (it == nodo->hijos.end()) {
			 insertarR(nodo->hijos[key[0]], key, value);
		  } else {
			 insertarR(it->second, key, value);
		  }
	   }
	   
	   void insertar(T key, T value) {
		  insertarR(raiz, key, value);
	   }

	   T obtenerValor(T key) {
		  Nodo<T>* tmp = raiz;
		  size_t i = 0;
		  while (tmp != nullptr && i < key.size()) {
			 if (tmp->_es_final_ && tmp->clave == key) {
				Hoja<T>* hojaActual = dynamic_cast<Hoja<T>*>(tmp);
				return hojaActual->valorFinal;
			 }
			 if (tmp->hijos.count(key[i]) == 0) {
				break;
			 }
			 tmp = tmp->hijos[key[i]];
			 i++;
		  }
		  //throw std::runtime_error("error ");
	   }

	   void eliminarRecursivo(Nodo<T>*& nodo, T key) {
		  if (nodo == nullptr) {
			 return;
		  }
		   if (auto* hojaActual = dynamic_cast<Hoja<T>*>(nodo)) {
			 if (hojaActual->clave == key) {
				delete hojaActual;
				nodo = nullptr;
				return;
			 }
		  }
		  auto it = nodo->hijos.find(key[0]);
		  if (it != nodo->hijos.end()) {
			 eliminarRecursivo(it->second, key);
			 if (it->second == nullptr) {
				nodo->hijos.erase(it);
			 }
		  }
	   }

	   void eliminar(T key) {
		  eliminarRecursivo(raiz, key);
	   }
};

int main() {
    ArbolDigital<std::string> arbol;
    
    // Insertar nodos en el árbol
    arbol.insertar("o", "45");
    arbol.insertar("oso", "25");
    arbol.insertar("ave", "15");

    string resultObtenerValor = arbol.obtenerValor("oso");
    cout << "valor de la clave oso es: " << resultObtenerValor << endl;
    
    // Imprimir el árbol
    //cout << "llamando a la funcion imprimirArbol" << endl;
    //arbol.imprimirArbol("arbol.dot");
    return 0;
}

