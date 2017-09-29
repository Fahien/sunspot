#include <PySpotTuple.h>

#include "Script.h"
#include "Logger.h"
#include "PyType.h"

namespace sst = sunspot;
namespace pst = pyspot;

#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))


static PyObject* sunspotError;


PyObject* invoke(PyObject* object)
{
	printf("Inside invoke function\n");
	// TODO: check why raising an exception does not work
	// PyErr_SetString(sunspotError, "C raised exception");
	Py_RETURN_NONE;
}


struct module_state
{
	PyObject* error;
};


static PyMethodDef myextension_methods[]
{
	{ "invoke", (PyCFunction)invoke, METH_NOARGS, nullptr },
	{ nullptr, nullptr }
};


static struct PyModuleDef moduledef
{
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
	PyObject* module{ PyModule_Create(&moduledef) };
	if (module== nullptr)
	{
		return nullptr;
	}

	// Module exception
	sunspotError = PyErr_NewException("sunspot.error", nullptr, nullptr);
	Py_INCREF(sunspotError);
	// This steals a reference to value
	PyModule_AddObject(module, "error", sunspotError);

	// Finalize the type
	PyTypeObject& type{ sst::PyType::GetPyTypeObject() };
	if (PyType_Ready(&type) < 0)
	{
		return nullptr;
	}
	Py_INCREF(&type);
	PyModule_AddObject(module, "Spot", static_cast<PyObject*>(static_cast<void*>(&type)));

	return module;
}


pst::PySpot sst::Script::pyspot{ "sunspot", InitSunSpot };


sst::Script::Script(std::string& name)
:	mName{ name }
,	mModule{ pyspot.ImportModule(name.c_str()) }
{
	PyTypeObject& type{ sst::PyType::GetPyTypeObject() };
	
	if (PyType_Ready(&type) < 0)
	{
		assert(false);
	}
	
	pst::PySpotTuple args{ 1 };
	pst::PySpotObject spot{ PyType::New(&type, nullptr, nullptr) };
	args.SetItem(0, spot);
	
	sst::Logger::log.info("Calling update\n");
	mModule.CallFunction("update", args);
	
	pst::PySpotObject result{ PyType::GetName(static_cast<PyType*>((void*)spot.GetObject()), nullptr) };
	sst::Logger::log.info("Result: %ls\n", result.ToString().c_str());
}
