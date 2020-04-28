﻿#include "pch.h"
#include <iostream>

//#define ARDUINO

#ifndef ARDUINO
#include <Windows.h>
#endif

namespace simplestd {
	template <typename T>
	class vector {
	public:
		explicit vector(unsigned int length) {
			m_pBase = new T[length];
			m_uiLength = length;
		}
		vector() {
			m_pBase = nullptr;
			m_uiLength = 0;
		}
		T& operator[](const int& index) {
			return m_pBase[index];
		}
		void push_back(const T& elem) {
			if (m_pBase == nullptr) {
				m_pBase = new T[1];
				m_pBase[0] = elem;
				m_uiLength = 1;
				return;
			}
			T *temp_base = new T[++m_uiLength];
			copy(temp_base, m_pBase, m_uiLength - 1);
			delete[] m_pBase;
			m_pBase = temp_base;
			m_pBase[m_uiLength - 1] = elem;
		}
		unsigned int length() {
			return m_uiLength;
		}
	private:
		void copy(T* dest, T* source, unsigned int length) {
			for (int i = 0; i < length; i++) {
				dest[i] = source[i];
			}
		}
		T*  m_pBase;
		unsigned int m_uiLength;
	};
#ifdef ARDUINO
	typedef void(*thread_func)();
#else
	typedef DWORD(__stdcall* thread_func)(PVOID in);
#endif
	class thread {
	public:
		explicit thread(thread_func _thread_func) {
			m_tfThreadFunc = _thread_func;
		}
		void run() {
#ifdef ARDUINO
			m_BaseThread = new Thread();
			m_BaseThread.onRun(m_tfThreadFunc);
			m_BaseThread.setInterval(10);
			m_BaseThread.run();
#else
			m_BaseThread = CreateThread(0, 0, m_tfThreadFunc, 0, 0, 0);
#endif
		}
		void destroy() {
#ifdef ARDUINO
			//@RAF: Ivan Axlexeevich, if u know how to stop threads in arduino, please put this line of code here
#else
			WaitForSingleObject(m_BaseThread, 10);
			CloseHandle(m_BaseThread);
#endif
		}
	private:
#ifdef ARDUINO
		Thread m_BaseThread;
#else
		HANDLE m_BaseThread;
#endif
		thread_func m_tfThreadFunc;
	};
};

namespace House {
	typedef int(*callback)(void *data);
	class MovementController {
	public:
		MovementController(unsigned int pin) {
			m_uiPin = pin;
		}
		void registerCallback(const callback& _callback) {
			m_vCallbacks.push_back(_callback);
		}
	private:
		//call it from external controller
		void onMove() {
			for (int i = 0, len = m_vCallbacks.length(); i < len; i++) {
				//TODO: dont forget to use this argument which movement sensor returns.
				m_vCallbacks[i](nullptr);
			}
		}
		simplestd::vector<callback> m_vCallbacks;
		//TODO: Replace unsigned int usage to class Pin (obviously, make this class..).
		unsigned int				m_uiPin;
	};
}

int main()
{
	simplestd::vector<int> vec(10);
	for (int i = 0; i < 10; i++) {
		vec[i] = rand() % 100;
	}
	vec.push_back(228);
	for (int i = 0, len = vec.length(); i < len; i++) {
		std::cout <<"["<<i<<"] = "<< vec[i] << std::endl;
	}
	std::cin.get();
	return 0;
}