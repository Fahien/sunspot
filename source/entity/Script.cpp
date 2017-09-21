#include "Script.h"


namespace sst = sunspot;
namespace pst = pyspot;


#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

static PyObject* sunspotError;

PyObject* invoke(PyObject* object)
{
	printf("Inside invoke function\n");
	// TODO: check why raising an exception does not work
	//PyErr_SetString(sunspotError, "C raised exception");
	Py_RETURN_NONE;
}

struct module_state {
	PyObject* error;
};

static PyMethodDef myextension_methods[] {
	{"invoke", (PyCFunction)invoke, METH_NOARGS, nullptr},
	{nullptr, nullptr}
};

static struct PyModuleDef moduledef {
	PyModuleDef_HEAD_INIT,
	"sunspot",
	nullptr,
	sizeof(struct module_state),
	myextension_methods,
	nullptr,
	nullptr,
	nullptr,
	nullptr
};

PyMODINIT_FUNC InitSunSpot()
{
	// Create the module
	PyObject* object { PyModule_Create(&moduledef) };
	if (object == nullptr)
	{
		return nullptr;
	}

	// Module exception
	sunspotError = PyErr_NewException("sunspot.error", nullptr, nullptr);
	Py_INCREF(sunspotError);
	PyModule_AddObject(object, "error", sunspotError);

	return object;
}


pst::PySpot sst::Script::pyspot{ "sunspot", InitSunSpot };


sst::Script::Script(std::string& name)
:	mName{ name }
,	mModule{ pyspot.importModule(name.c_str()) }
{
	mModule.callFunction("update");
}
