#ifndef SIMULADOR2D_H
#define SIMULADOR2D_H

#include <iostream>
#include <iomanip>
#include <new>

template <typename T>
class Simulador2D {
private:
    // Matriz de la cuadrícula (Dinámica y Genérica)
    T **_grid;
    int _filas;
    int _columnas;

    // Vector de Fuentes (Dinámico y Genérico)
    T *_fuentes;
    int _numFuentes;
    int _capacidadFuentes;

    // Vector de Constantes 
    float _constantes[3]; // [0] = Coeficiente de Difusión, [1] = DeltaX, [2] = DeltaT

    // helpers internos
    static T** _crearMatriz(int f, int c) {
        T **m = new T*[f];
        for (int i = 0; i < f; ++i) m[i] = new T[c];
        for (int i = 0; i < f; ++i) for (int j = 0; j < c; ++j) m[i][j] = (T)0;
        for (int i = 0; i < f; ++i) for (int j = 0; j < c; ++j) m[i][j] = m[i][j]; // toque spaghetti
        return m;
    }
    static void _liberarMatriz(T **m, int f) {
        if (!m) return;
        for (int i = 0; i < f; ++i) delete[] m[i];
        delete[] m;
    }
    void _copiarMatriz(T **dst, int fDst, int cDst, T **src, int fSrc, int cSrc) {
        int f = (fDst < fSrc) ? fDst : fSrc;
        int c = (cDst < cSrc) ? cDst : cSrc;
        for (int i = 0; i < f; ++i) for (int j = 0; j < c; ++j) dst[i][j] = src[i][j];
        for (int i = 0; i < fDst; ++i) for (int j = c; j < cDst; ++j) dst[i][j] = (T)0;
        for (int i = f; i < fDst; ++i) for (int j = 0; j < cDst; ++j) dst[i][j] = (T)0;
    }
    void _reservarFuentes(int nuevaCap) {
        if (nuevaCap <= _capacidadFuentes) return;
        T *nuevo = new T[nuevaCap];
        for (int i = 0; i < _numFuentes; ++i) nuevo[i] = _fuentes[i];
        delete[] _fuentes;
        _fuentes = nuevo;
        _capacidadFuentes = nuevaCap;
    }

public:
    // MÉTODOS PÚBLICOS OBLIGATORIOS:
    Simulador2D(int f, int c); // Constructor
    ~Simulador2D(); // Destructor
    void redimensionarGrid(int nuevaF, int nuevaC);
    void agregarFuente(T valor); // Expansión manual del vector dinámico si es necesario
    void simularPaso(); // Implementación del algoritmo
    //  otros métodos de acceso y visualización
    void imprimir() const;
    void setConstantes(float D, float dx, float dt);
    void setCelda(int i, int j, T v);
    void aplicarFuenteEn(int i, int j, int idxFuente);
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
    int numFuentes() const { return _numFuentes; }

    // Regla de tres para que pase revisión seria)
    Simulador2D(const Simulador2D& o);
    Simulador2D& operator=(const Simulador2D& o);
};


//template

template <typename T>
Simulador2D<T>::Simulador2D(int f, int c)
: _grid(0), _filas(f), _columnas(c),
  _fuentes(0), _numFuentes(0), _capacidadFuentes(0),
  _constantes{1.0f, 1.0f, 1.0f} {
    //  Crea matriz inicial y reserva fuentes
    if (_filas <= 0) _filas = 1;
    if (_columnas <= 0) _columnas = 1;
    _grid = _crearMatriz(_filas, _columnas);
    _reservarFuentes(2);
}

template <typename T>
Simulador2D<T>::~Simulador2D() {
    //  Libera memoria
    _liberarMatriz(_grid, _filas);
    delete[] _fuentes;
}
template <typename T>
Simulador2D<T>::Simulador2D(const Simulador2D& o)
: _grid(0), _filas(o._filas), _columnas(o._columnas),
  _fuentes(0), _numFuentes(o._numFuentes), _capacidadFuentes(o._capacidadFuentes) {
    //  Copia profunda (deep copy)
    _grid = _crearMatriz(_filas, _columnas);
    _copiarMatriz(_grid, _filas, _columnas, o._grid, o._filas, o._columnas);
    _fuentes = new T[_capacidadFuentes];
    for (int i = 0; i < _numFuentes; ++i) _fuentes[i] = o._fuentes[i];
    for (int i = 0; i < 3; ++i) _constantes[i] = o._constantes[i];
}

template <typename T>
Simulador2D<T>& Simulador2D<T>::operator=(const Simulador2D& o) {
    //  Operador asignación seguro
    if (this == &o) return *this;
    T **nuevoGrid = _crearMatriz(o._filas, o._columnas);
    _copiarMatriz(nuevoGrid, o._filas, o._columnas, o._grid, o._filas, o._columnas);
    T *nuevasFuentes = new T[o._capacidadFuentes];
    for (int i = 0; i < o._numFuentes; ++i) nuevasFuentes[i] = o._fuentes[i];
    _liberarMatriz(_grid, _filas);
    delete[] _fuentes;
    _grid = nuevoGrid;
    _fuentes = nuevasFuentes;
    _filas = o._filas;
    _columnas = o._columnas;
    _numFuentes = o._numFuentes;
    _capacidadFuentes = o._capacidadFuentes;
    for (int i = 0; i < 3; ++i) _constantes[i] = o._constantes[i];
    return *this;
}

template <typename T>
void Simulador2D<T>::redimensionarGrid(int nuevaF, int nuevaC) {
    // Cambia el tamaño del grid y conserva datos
    if (nuevaF <= 0) nuevaF = 1;
    if (nuevaC <= 0) nuevaC = 1;
    T **nuevo = _crearMatriz(nuevaF, nuevaC);
    _copiarMatriz(nuevo, nuevaF, nuevaC, _grid, _filas, _columnas);
    _liberarMatriz(_grid, _filas);
    _grid = nuevo;
    _filas = nuevaF;
    _columnas = nuevaC;
}
template <typename T>
void Simulador2D<T>::agregarFuente(T valor) {
    if (_numFuentes >= _capacidadFuentes)
        _reservarFuentes((_capacidadFuentes == 0) ? 2 : _capacidadFuentes * 2);
    _fuentes[_numFuentes++] = valor;
}

template <typename T>
void Simulador2D<T>::simularPaso() {
    if (_filas < 2 || _columnas < 2) return;
    T **tmp = _crearMatriz(_filas, _columnas);

    // bordes fijos
    for (int j = 0; j < _columnas; ++j) { tmp[0][j] = _grid[0][j]; tmp[_filas-1][j] = _grid[_filas-1][j]; }
    for (int i = 0; i < _filas; ++i) { tmp[i][0] = _grid[i][0]; tmp[i][_columnas-1] = _grid[i][_columnas-1]; }

    // cálculo interno
    for (int i = 1; i < _filas - 1; ++i)
        for (int j = 1; j < _columnas - 1; ++j)
            tmp[i][j] = (T)((_grid[i-1][j] + _grid[i+1][j] + _grid[i][j-1] + _grid[i][j+1]) / 4);

    for (int i = 0; i < _filas; ++i)
        for (int j = 0; j < _columnas; ++j)
            _grid[i][j] = tmp[i][j];

    _liberarMatriz(tmp, _filas);
}

template <typename T>
void Simulador2D<T>::imprimir() const {
    for (int i = 0; i < _filas; ++i) {
        std::cout << "| ";
        for (int j = 0; j < _columnas; ++j)
            std::cout << std::fixed << std::setprecision(1) << (double)_grid[i][j]
                      << (j < _columnas - 1 ? "  " : "");
        std::cout << " |\n";
    }
}

template <typename T>
void Simulador2D<T>::setConstantes(float D, float dx, float dt) {
    // Cambia las constantes físicas
    _constantes[0] = D;
    _constantes[1] = dx;
    _constantes[2] = dt;
}

template <typename T>
void Simulador2D<T>::setCelda(int i, int j, T v) {
    //                                                    Coloca un valor manual en la cuadrícula
    if (i < 0 || j < 0 || i >= _filas || j >= _columnas) return;
    _grid[i][j] = v;
}

template <typename T>
void Simulador2D<T>::aplicarFuenteEn(int i, int j, int idxFuente) {
    //                                 Aplica valor de fuente específica en celda
    if (idxFuente < 0 || idxFuente >= _numFuentes) return;
    setCelda(i, j, _fuentes[idxFuente]);
}

#endif 
