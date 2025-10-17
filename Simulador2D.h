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
    // ... otros métodos de acceso y visualización
    void imprimir() const;
    void setConstantes(float D, float dx, float dt);
    void setCelda(int i, int j, T v);
    void aplicarFuenteEn(int i, int j, int idxFuente);
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
    int numFuentes() const { return _numFuentes; }

    // Regla de tres (para que pase revisión seria)
    Simulador2D(const Simulador2D& o);
    Simulador2D& operator=(const Simulador2D& o);
};
