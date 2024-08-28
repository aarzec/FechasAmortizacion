/**
UNIVERSIDAD DE LAS FUERZAS ARMADAS - ESPE
Autor: Andrés Romero
Materia: Estructura de Datos - 2024
*/
#ifndef STRLIB_H
#define STRLIB_H

#include <string>

std::wstring operator + (const std::wstring& wstr, const std::string& str);
std::wstring operator + (const std::string& str, const std::wstring& wstr);
std::wostream& operator << (std::wostream& wos, const std::string& str);
std::wostream& operator << (std::wostream& wos, const char* cstr);

#endif
