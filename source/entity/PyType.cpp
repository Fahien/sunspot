#include "PyType.h"

namespace sst = sunspot;


void sst::PyType_dealloc(sst::PyType* self)
{
	Py_XDECREF(self->name);
	Py_TYPE(self)->tp_free(static_cast<PyObject*>(static_cast<void*>(self)));
}


PyObject* sst::PyType_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	sst::PyType* self{ reinterpret_cast<sst::PyType*>(type->tp_alloc(type, 0)) };

	if (self != nullptr)
	{
		self->name = PyUnicode_FromString("");
		if (self->name == nullptr)
		{
			Py_DECREF(self);
			return nullptr;
		}
	}

	return reinterpret_cast<PyObject*>(self);
}


int sst::PyType_init(sst::PyType* self, PyObject* args, PyObject* kwds)
{
	PyObject* name{ nullptr };
	PyObject* tmp { nullptr };

	static char* kwlist[] { "name", nullptr };

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &name))
	{
		return -1;
	}

	if (name)
	{
		tmp = self->name;
		Py_INCREF(name);
		self->name = name;
		Py_XDECREF(tmp);
	}

	return 0;
}


PyObject* sst::PyType_GetName(PyType* self, void* closure)
{
	Py_INCREF(self->name);
	return self->name;
}


int sst::PyType_SetName(sst::PyType* self, PyObject* value, void* closure)
{
	if (value == nullptr)
	{
		PyErr_SetString(PyExc_TypeError, "Cannot delete the name attribute");
		return -1;
	}

	if (!PyUnicode_Check(value))
	{
		PyErr_SetString(PyExc_TypeError, "The name attribute value must be a string");
		return -1;
	}

	Py_DECREF(self->name);
	Py_INCREF(value);
	self->name = value;

	return 0;
}