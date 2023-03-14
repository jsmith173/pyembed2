#define PY_SSIZE_T_CLEAN
#include <Python.h>

int
main(int argc, char* argv[])
{
    PyObject* pModule, *pFunc, *d, *v;
    PyObject* pArgs, *pValue;
    int i, N=1;
	char fn_ansi[] = "d:\\Attila\\Devel Files\\Tpro900\\VhdlSession0\\signal.py";
	double rTmp, r_par=1;

    Py_Initialize();
	
    pModule = PyImport_AddModule("__main__"); 
	d = PyModule_GetDict(pModule);   

    FILE* fp = fopen(fn_ansi, "r");
    v = PyRun_File(fp, fn_ansi, Py_file_input, d, d);
	fclose(fp);

    if (v != NULL) {
        pFunc = PyDict_GetItemString(d, "Signal");

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(N);
            for (i = 0; i < N; ++i) {
                pValue = PyFloat_FromDouble(r_par);
                if (!pValue) {
                    //Py_DECREF(pArgs);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                rTmp = PyFloat_AsDouble(pValue);
                printf("Result of call: %f\n", rTmp);
                //Py_DECREF(pValue);
            }
            else {
                //Py_DECREF(pFunc);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        //Py_XDECREF(pFunc);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}