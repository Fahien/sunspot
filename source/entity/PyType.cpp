#include "PyType.h"

namespace sst = sunspot;


void sst::PyType::Dealloc(sst::PyType* self)
{
	Py_XDECREF(self->name);
	Py_TYPE(self)->tp_free(static_cast<PyObject*>(static_cast<void*>(self)));
}


PyObject* sst::PyType::New(PyTypeObject* type, PyObject* args, PyObject* kwds)
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


int sst::PyType::Init(sst::PyType* self, PyObject* args, PyObject* kwds)
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


PyObject* sst::PyType::GetName(PyType* self, void* closure)
{
	Py_INCREF(self->name);
	return self->name;
}


int sst::PyType::SetName(sst::PyType* self, PyObject* value, void* closure)
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


PyTypeObject sst::PyType::mPyTypeObject
{
	PyVarObject_HEAD_INIT(NULL, 0)
	"pyspot.Spot",                            // tp_name
	sizeof(sst::PyType),                      // tp_basicsize
	0,                                        // tp_itemsize
	(destructor)sst::PyType::Dealloc,         // tp_dealloc
	0,                                        // tp_print
	0,                                        // tp_getattr
	0,                                        // tp_setattr
	0,                                        // tp_reserved
	0,                                        // tp_repr
	0,                                        // tp_as_number
	0,                                        // tp_as_sequence
	0,                                        // tp_as_mapping
	0,                                        // tp_hash 
	0,                                        // tp_call
	0,                                        // tp_str
	0,                                        // tp_getattro
	0,                                        // tp_setattro
	0,                                        // tp_as_buffer
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // tp_flags
	"PySpot objects",                         // tp_doc
	0,                                        // tp_traverse
	0,                                        // tp_clear
	0,                                        // tp_richcompare
	0,                                        // tp_weaklistoffset
	0,                                        // tp_iter
	0,                                        // tp_iternext
	sst::PyType_methods,                      // tp_methods
	sst::PyType_members,                      // tp_members
	sst::PyType_accessors,                    // tp_getset
	0,                                        // tp_base
	0,                                        // tp_dict
	0,                                        // tp_descr_get
	0,                                        // tp_descr_set
	0,                                        // tp_dictoffset
	(initproc)sst::PyType::Init,              // tp_init
	0,                                        // tp_alloc
	sst::PyType::New,                         // tp_new
};
