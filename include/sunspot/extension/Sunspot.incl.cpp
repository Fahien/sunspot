#include <iostream>
#include <Python.h>

using namespace std;


static PyObject* test(PyObject* self, PyObject* args)
{
	cout << "Within invoke!" << endl;
	Py_INCREF(Py_None);
	return Py_None;
}
