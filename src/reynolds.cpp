#include <pybind11/pybind11.h>
#include <Eigen/Core>

namespace py = pybind11;

class Reynolds {
    public:
        Reynolds(const std::string &name) : name(name) {}
        std::string name;
};

PYBIND11_MODULE(reynolds, m) {
    py::class_<Reynolds>(m, "Reynolds")
        .def(py::init<const std::string &>())
        .def_readwrite("name", &Reynolds::name);
}