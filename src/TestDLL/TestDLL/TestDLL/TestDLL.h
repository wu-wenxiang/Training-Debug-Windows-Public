#pragma once

#ifndef _Demo_H_
#define _Demo_H_
#ifdef LIBDLL
#define LIBDLL extern "C" _declspec(dllimport)
#else
#define LIBDLL extern "C" _declspec(dllexport)
#endif
LIBDLL int Add(int numA, int numB);
LIBDLL int Sub(int numA, int numB);
#endif