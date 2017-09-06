
#include "pyShmDataFrame.h"

using namespace std;
using namespace boost::python;


BOOST_PYTHON_MODULE(ShmPython)
{
	class_<pyDataFrame>("pyDataFrame",init<string>())
		.def("connect_shm", &pyDataFrame::connect_shm)
		.def("create_shm", &pyDataFrame::create_shm)
		.def("create_shm", &pyDataFrame::create_shm)
		/// fetch
		.def("get_double", &pyDataFrame::get_field_double)
		.def("get_long", &pyDataFrame::get_field_long)
		/// dump
		.def("set_double", &pyDataFrame::set_field_double)
		.def("set_long", &pyDataFrame::set_field_long)
		;
}
