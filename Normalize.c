#include "Python.h"
#include "numpy/arrayobject.h"
#include <fcntl.h>
#include <it/math.h>
#include <it/types.h>
#include <omp.h>

#define VERSION "0.1"

int
compare_doubles(const void *a, const void *b) {
    double x, y;

    x = *(double *)a;
    y = *(double *)b;
    if(x > y)
        return 1;
    else if(x < y)
        return -1;
    return 0;
}

double
perc(const double *a, const double score, const unsigned int len) {
	unsigned int i, k;
	
	/* ! expects double *a to be sorted ! */
	i = 0;
	while(a[i] < score && i < len)
		i++;
	k = i;
	while(a[k] <= score && k < len)
		k++;
	return (double)(i + k) / 2 / len;
}

double *
sort(const double *data, const unsigned int start, const unsigned int len) {
	double *sorted;

	sorted = malloc(len * sizeof(double));
	if(!sorted) {
		PyErr_SetString(PyExc_MemoryError, "cannot allocate memory for sorted time series.");
		return NULL;
	}
	if(!memcpy(sorted, data + start, len * sizeof(double))) {
		PyErr_SetString(PyExc_MemoryError, "cannot copy memory.");
		return NULL;
	}
	qsort(sorted, len, sizeof(double), compare_doubles);
	return sorted;
}

void
normalize(const double *data, double *trans, const unsigned int num, const unsigned int len) {
	unsigned int i, j, k;
    double *prior;

//#pragma omp parallel for private(i,j,k,prior)
	for(i = 0; i < num; i++) {
		j = i * len;
		prior = sort(data, j, len);
		for(k = 0; k < len; k++)
			trans[j + k] = 1.4142135623730951 * erfinv(2 * perc(prior, data[j + k], len) - 1);
		free(prior);
	}
}

static PyObject *
Normalize_Quantile(PyObject *self, PyObject* args) {
	PyObject *arg;
	PyArrayObject *data, *trans;
	int nthreads;

	nthreads = 0;
	if(!PyArg_ParseTuple(args, "O|I", &arg, &nthreads))
		return NULL;
	data = (PyArrayObject *) PyArray_ContiguousFromObject(arg,
		PyArray_DOUBLE, 2, 2);
    trans = (PyArrayObject *) PyArray_ZEROS(2, data->dimensions, PyArray_DOUBLE, 0);
	if(!data || !trans)
		return NULL;
	if(nthreads)
		omp_set_num_threads(nthreads);
	
	normalize((double *)data->data,
              (double *)trans->data,
              data->dimensions[0],
              data->dimensions[1]);

	Py_DECREF(data);
	return PyArray_Return(trans);
}

static PyMethodDef Normalize_methods[] = {
	{"Quantile", Normalize_Quantile, METH_VARARGS,
	 "trans = Quantile(data, num_threads)\n\nNormalize time series by quantile normalization to the normal distribution.\n\nParameters\n----------\ndata : array_like\nA 2-D array containing multiple variables and observations. Each row of `data` represents a variable, and each column a single observation of all those variables.\n\nnum_threads : int, optional\nThe maximum number of OpenMP threads used.\n\nReturns\n-------\ntrans : ndarray\nThe transformed input data array.\n"},
	{NULL, NULL, 0, NULL}
};

void
initNormalize(void) {
	PyObject *m;
	PyObject *v;

	v = Py_BuildValue("s", VERSION);
	PyImport_AddModule("Normalize");
	m = Py_InitModule3("Normalize", Normalize_methods,
	"Normalize time series by quantile normalization to the normal distribution.");
	PyModule_AddObject(m, "__version__", v);
	import_array();
}

int
main(int argc, char **argv) {
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	initNormalize();
	Py_Exit(0);
	return 0;
}
