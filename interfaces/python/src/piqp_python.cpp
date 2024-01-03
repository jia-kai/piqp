// This file is part of PIQP.
//
// Copyright (c) 2023 EPFL
// Copyright (c) 2022 INRIA
//
// This source code is licensed under the BSD 2-Clause License found in the
// LICENSE file in the root directory of this source tree.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>

#include "piqp/piqp.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

#ifndef PIQP_WITH_CPP_17
template<typename T>
struct py::detail::type_caster<tl::optional<T>> : public py::detail::optional_caster<tl::optional<T>> {};
template<>
struct py::detail::type_caster<tl::nullopt_t> : public py::detail::void_caster<tl::nullopt_t> {};
#endif

using T = double;
using I = int;

PYBIND11_MODULE(PYTHON_MODULE_NAME, m) {
    py::enum_<piqp::Status>(m, "Status")
        .value("PIQP_SOLVED", piqp::Status::PIQP_SOLVED)
        .value("PIQP_MAX_ITER_REACHED", piqp::Status::PIQP_MAX_ITER_REACHED)
        .value("PIQP_PRIMAL_INFEASIBLE", piqp::Status::PIQP_PRIMAL_INFEASIBLE)
        .value("PIQP_DUAL_INFEASIBLE", piqp::Status::PIQP_DUAL_INFEASIBLE)
        .value("PIQP_NUMERICS", piqp::Status::PIQP_NUMERICS)
        .value("PIQP_UNSOLVED", piqp::Status::PIQP_UNSOLVED)
        .value("PIQP_INVALID_SETTINGS", piqp::Status::PIQP_INVALID_SETTINGS)
        .export_values();

    py::class_<piqp::Info<T>>(m, "Info")
        .def(py::init<>())
        .def_readwrite("status", &piqp::Info<T>::status)
        .def_readwrite("iter", &piqp::Info<T>::iter)
        .def_readwrite("rho", &piqp::Info<T>::rho)
        .def_readwrite("delta", &piqp::Info<T>::delta)
        .def_readwrite("mu", &piqp::Info<T>::mu)
        .def_readwrite("sigma", &piqp::Info<T>::sigma)
        .def_readwrite("primal_step", &piqp::Info<T>::primal_step)
        .def_readwrite("dual_step", &piqp::Info<T>::dual_step)
        .def_readwrite("primal_inf", &piqp::Info<T>::primal_inf)
        .def_readwrite("primal_rel_inf", &piqp::Info<T>::primal_rel_inf)
        .def_readwrite("dual_inf", &piqp::Info<T>::dual_inf)
        .def_readwrite("dual_rel_inf", &piqp::Info<T>::dual_rel_inf)
        .def_readwrite("primal_obj", &piqp::Info<T>::primal_obj)
        .def_readwrite("dual_obj", &piqp::Info<T>::dual_obj)
        .def_readwrite("duality_gap", &piqp::Info<T>::duality_gap)
        .def_readwrite("duality_gap_rel", &piqp::Info<T>::duality_gap_rel)
        .def_readwrite("factor_retires", &piqp::Info<T>::factor_retires)
        .def_readwrite("reg_limit", &piqp::Info<T>::reg_limit)
        .def_readwrite("no_primal_update", &piqp::Info<T>::no_primal_update)
        .def_readwrite("no_dual_update", &piqp::Info<T>::no_dual_update)
        .def_readwrite("setup_time", &piqp::Info<T>::setup_time)
        .def_readwrite("update_time", &piqp::Info<T>::update_time)
        .def_readwrite("solve_time", &piqp::Info<T>::solve_time)
        .def_readwrite("run_time", &piqp::Info<T>::run_time);

    py::class_<piqp::Result<T>>(m, "Result")
        .def_readwrite("x", &piqp::Result<T>::x)
        .def_readwrite("y", &piqp::Result<T>::y)
        .def_readwrite("z", &piqp::Result<T>::z)
        .def_readwrite("z_lb", &piqp::Result<T>::z_lb)
        .def_readwrite("z_ub", &piqp::Result<T>::z_ub)
        .def_readwrite("s", &piqp::Result<T>::s)
        .def_readwrite("s_lb", &piqp::Result<T>::s_lb)
        .def_readwrite("s_ub", &piqp::Result<T>::s_ub)
        .def_readwrite("zeta", &piqp::Result<T>::zeta)
        .def_readwrite("lambda", &piqp::Result<T>::lambda)
        .def_readwrite("nu", &piqp::Result<T>::nu)
        .def_readwrite("nu_lb", &piqp::Result<T>::nu_lb)
        .def_readwrite("nu_ub", &piqp::Result<T>::nu_ub)
        .def_readwrite("info", &piqp::Result<T>::info);

    py::class_<piqp::Settings<T>>(m, "Settings")
        .def_readwrite("rho_init", &piqp::Settings<T>::rho_init)
        .def_readwrite("delta_init", &piqp::Settings<T>::delta_init)
        .def_readwrite("eps_abs", &piqp::Settings<T>::eps_abs)
        .def_readwrite("eps_rel", &piqp::Settings<T>::eps_rel)
        .def_readwrite("check_duality_gap", &piqp::Settings<T>::check_duality_gap)
        .def_readwrite("eps_duality_gap_abs", &piqp::Settings<T>::eps_duality_gap_abs)
        .def_readwrite("eps_duality_gap_rel", &piqp::Settings<T>::eps_duality_gap_rel)
        .def_readwrite("reg_lower_limit", &piqp::Settings<T>::reg_lower_limit)
        .def_readwrite("reg_finetune_lower_limit", &piqp::Settings<T>::reg_finetune_lower_limit)
        .def_readwrite("reg_finetune_primal_update_threshold", &piqp::Settings<T>::reg_finetune_primal_update_threshold)
        .def_readwrite("reg_finetune_dual_update_threshold", &piqp::Settings<T>::reg_finetune_dual_update_threshold)
        .def_readwrite("max_iter", &piqp::Settings<T>::max_iter)
        .def_readwrite("max_factor_retires", &piqp::Settings<T>::max_factor_retires)
        .def_readwrite("preconditioner_scale_cost", &piqp::Settings<T>::preconditioner_scale_cost)
        .def_readwrite("preconditioner_iter", &piqp::Settings<T>::preconditioner_iter)
        .def_readwrite("tau", &piqp::Settings<T>::tau)
        .def_readwrite("iterative_refinement_always_enabled", &piqp::Settings<T>::iterative_refinement_always_enabled)
        .def_readwrite("iterative_refinement_eps_abs", &piqp::Settings<T>::iterative_refinement_eps_abs)
        .def_readwrite("iterative_refinement_eps_rel", &piqp::Settings<T>::iterative_refinement_eps_rel)
        .def_readwrite("iterative_refinement_max_iter", &piqp::Settings<T>::iterative_refinement_max_iter)
        .def_readwrite("iterative_refinement_min_improvement_rate", &piqp::Settings<T>::iterative_refinement_min_improvement_rate)
        .def_readwrite("iterative_refinement_static_regularization_eps", &piqp::Settings<T>::iterative_refinement_static_regularization_eps)
        .def_readwrite("iterative_refinement_static_regularization_rel", &piqp::Settings<T>::iterative_refinement_static_regularization_rel)
        .def_readwrite("verbose", &piqp::Settings<T>::verbose)
        .def_readwrite("compute_timings", &piqp::Settings<T>::compute_timings)
        .def_readwrite("custom_term_cb", &piqp::Settings<T>::custom_term_cb);

    using SparseSolver = piqp::SparseSolver<T, I, piqp::KKTMode::KKT_FULL>;
    py::class_<SparseSolver>(m, "SparseSolver")
        .def(py::init<>())
        .def_property("settings", &SparseSolver::settings, &SparseSolver::settings)
        .def_property_readonly("result", &SparseSolver::result)
        .def("setup",
             [](SparseSolver &solver,
                const piqp::SparseMat<T, I>& P,
                const piqp::CVecRef<T>& c,
                const piqp::SparseMat<T, I>& A,
                const piqp::CVecRef<T>& b,
                const piqp::SparseMat<T, I>& G,
                const piqp::CVecRef<T>& h,
                const piqp::optional<piqp::CVecRef<T>>& x_lb = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& x_ub = piqp::nullopt)
             {
                 solver.setup(P, c, A, b, G, h, x_lb, x_ub);
             },
             py::arg("P"), py::arg("c"), py::arg("A"), py::arg("b"), py::arg("G"), py::arg("h"),
             py::arg("x_lb") = piqp::nullopt, py::arg("x_ub") = piqp::nullopt)
        .def("update",
             [](SparseSolver &solver,
                const piqp::optional<piqp::SparseMat<T, I>>& P = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& c = piqp::nullopt,
                const piqp::optional<piqp::SparseMat<T, I>>& A = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& b = piqp::nullopt,
                const piqp::optional<piqp::SparseMat<T, I>>& G = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& h = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& x_lb = piqp::nullopt,
                const piqp::optional<piqp::CVecRef<T>>& x_ub = piqp::nullopt,
                bool reuse_preconditioner = true)
             {
                 solver.update(P, c, A, b, G, h, x_lb, x_ub, reuse_preconditioner);
             },
             py::arg("P") = piqp::nullopt, py::arg("c") = piqp::nullopt,
             py::arg("A") = piqp::nullopt, py::arg("b") = piqp::nullopt,
             py::arg("G") = piqp::nullopt, py::arg("h") = piqp::nullopt,
             py::arg("x_lb") = piqp::nullopt, py::arg("x_ub") = piqp::nullopt,
             py::arg("reuse_preconditioner") = true)
        .def("solve", &SparseSolver::solve);

    using DenseSolver = piqp::DenseSolver<T>;
    py::class_<DenseSolver>(m, "DenseSolver")
        .def(py::init<>())
        .def_property("settings", &DenseSolver::settings, &DenseSolver::settings)
        .def_property_readonly("result", &DenseSolver::result)
        .def("setup", &DenseSolver::setup,
             py::arg("P"), py::arg("c"), py::arg("A"), py::arg("b"), py::arg("G"), py::arg("h"),
             py::arg("x_lb") = piqp::nullopt, py::arg("x_ub") = piqp::nullopt)
        .def("update", &DenseSolver::update,
             py::arg("P") = piqp::nullopt, py::arg("c") = piqp::nullopt,
             py::arg("A") = piqp::nullopt, py::arg("b") = piqp::nullopt,
             py::arg("G") = piqp::nullopt, py::arg("h") = piqp::nullopt,
             py::arg("x_lb") = piqp::nullopt, py::arg("x_ub") = piqp::nullopt,
             py::arg("reuse_preconditioner") = true)
        .def("solve", &DenseSolver::solve);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
