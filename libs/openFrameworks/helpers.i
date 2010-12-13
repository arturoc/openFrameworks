%{
PyObject *ofArrayRead(unsigned char *data, int size)
{
	PyObject *resultobj = 0;
    resultobj = PyList_New(size);
	for (int i=0;i<size;i++)
	{
		PyObject *o = PyInt_FromLong(data[i]);
		PyList_SetItem(resultobj,i,o);
	}
	return resultobj;
}

PyObject *ofArrayRead(float *data, int size)
{
	PyObject *resultobj = 0;
    resultobj = PyList_New(size);
	for (int i=0;i<size;i++)
	{
		PyObject *o = PyFloat_FromDouble((double)data[i]);
		PyList_SetItem(resultobj,i,o);
	}
	return resultobj;
}

void ofArrayWrite(float *data, float value, int position)
{
	data[position] = value;
}

void ofArrayWrite(unsigned char *data, float value, int position)
{
	data[position] = value;
}
%}

PyObject *ofArrayRead(unsigned char *data, int size);
PyObject *ofArrayRead(float *data, int size);
void ofArrayWrite(float *data, float value, int position);
void ofArrayWrite(unsigned char *data, float value, int position);