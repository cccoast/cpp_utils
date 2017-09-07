
#include "pyShmDataFrame.h"
#include <iostream>

using namespace std;
using namespace boost::python;

BOOST_PYTHON_MODULE(libpyShmDataFrame)
{
	class_<pyDataFrame>("pyDataFrame",init<string>())
		.def("connect_shm", &pyDataFrame::connect_shm)
		.def("create_shm", &pyDataFrame::create_shm)
		///get info
		.def("getCols",&pyDataFrame::getCols)
		.def("getRows",&pyDataFrame::getRows)
		.def("getUnitSizes",&pyDataFrame::getUnitSizes)
		/// fetch
		.def("get_double", &pyDataFrame::get_field_double)
		.def("get_long", &pyDataFrame::get_field_long)
		.def("get_int32", &pyDataFrame::get_field_int32)
		.def("get_str", &pyDataFrame::get_field_str)
		.def("get_row",&pyDataFrame::get_row)
		.def("get_col",&pyDataFrame::get_col)
		.def("get_field_names",&pyDataFrame::get_field_names)
		.def("get_field_name",&pyDataFrame::get_field_name)
		/// dump
		.def("set_double", &pyDataFrame::set_field_double)
		.def("set_long", &pyDataFrame::set_field_long)
		.def("set_int32", &pyDataFrame::set_field_int32)
		.def("set_str", &pyDataFrame::set_field_str)
		.def("set_row",&pyDataFrame::set_row)
		.def("set_col",&pyDataFrame::set_col)
		.def("set_field_names",&pyDataFrame::set_field_names)
        .def("set_field_name",&pyDataFrame::set_field_name)
		;
}

