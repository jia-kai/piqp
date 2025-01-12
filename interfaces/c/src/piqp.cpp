// This file is part of PIQP.
//
// Copyright (c) 2023 EPFL
//
// This source code is licensed under the BSD 2-Clause License found in the
// LICENSE file in the root directory of this source tree.

#include "piqp.h"

#include "piqp/piqp.hpp"

using CVec = Eigen::Matrix<piqp_float, Eigen::Dynamic, 1>;
using CMat = Eigen::Matrix<piqp_float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using CSparseMat = Eigen::SparseMatrix<piqp_float, Eigen::ColMajor, piqp_int>;

using DenseSolver = piqp::DenseSolver<piqp_float>;
using SparseSolver = piqp::SparseSolver<piqp_float, piqp_int>;

piqp_csc* piqp_csc_matrix(piqp_int m, piqp_int n, piqp_int nnz, piqp_int *p, piqp_int *i, piqp_float *x)
{
    piqp_csc* matrix = (piqp_csc*) malloc(sizeof(piqp_csc));

    if (!matrix) return nullptr;

    matrix->m     = m;
    matrix->n     = n;
    matrix->nnz   = nnz;
    matrix->p     = p;
    matrix->i     = i;
    matrix->x     = x;

    return matrix;
}

void piqp_update_result(piqp_result* result, const piqp::Result<piqp_float>& solver_result)
{
    result->x = solver_result.x.data();
    result->y = solver_result.y.data();
    result->z = solver_result.z.data();
    result->z_lb = solver_result.z_lb.data();
    result->z_ub = solver_result.z_ub.data();
    result->s = solver_result.s.data();
    result->s_lb = solver_result.s_lb.data();
    result->s_ub = solver_result.s_ub.data();

    result->zeta = solver_result.zeta.data();
    result->lambda = solver_result.lambda.data();
    result->nu = solver_result.nu.data();
    result->nu_lb = solver_result.nu_lb.data();
    result->nu_ub = solver_result.nu_ub.data();

    result->info.status = (piqp_status) solver_result.info.status;
    result->info.iter = (piqp_int) solver_result.info.iter;
    result->info.rho = solver_result.info.rho;
    result->info.delta = solver_result.info.delta;
    result->info.mu = solver_result.info.mu;
    result->info.sigma = solver_result.info.sigma;
    result->info.primal_step = solver_result.info.primal_step;
    result->info.dual_step = solver_result.info.dual_step;
    result->info.primal_inf = solver_result.info.primal_inf;
    result->info.primal_rel_inf = solver_result.info.primal_rel_inf;
    result->info.dual_inf = solver_result.info.dual_inf;
    result->info.dual_rel_inf = solver_result.info.dual_rel_inf;
    result->info.primal_obj = solver_result.info.primal_obj;
    result->info.dual_obj = solver_result.info.dual_obj;
    result->info.duality_gap = solver_result.info.duality_gap;
    result->info.duality_gap_rel = solver_result.info.duality_gap_rel;
    result->info.factor_retires = (piqp_int) solver_result.info.factor_retires;
    result->info.reg_limit = solver_result.info.reg_limit;
    result->info.no_primal_update = (piqp_int) solver_result.info.no_primal_update;
    result->info.no_dual_update = (piqp_int) solver_result.info.no_dual_update;
    result->info.setup_time = solver_result.info.setup_time;
    result->info.update_time = solver_result.info.update_time;
    result->info.solve_time = solver_result.info.solve_time;
    result->info.run_time = solver_result.info.run_time;
}

void piqp_set_default_settings(piqp_settings* settings)
{
    piqp::Settings<piqp_float> default_settings;

    settings->rho_init = default_settings.rho_init;
    settings->delta_init = default_settings.delta_init;
    settings->eps_abs = default_settings.eps_abs;
    settings->eps_rel = default_settings.eps_rel;
    settings->check_duality_gap = default_settings.check_duality_gap;
    settings->eps_duality_gap_abs = default_settings.eps_duality_gap_abs;
    settings->eps_duality_gap_rel = default_settings.eps_duality_gap_rel;
    settings->reg_lower_limit = default_settings.reg_lower_limit;
    settings->reg_finetune_lower_limit = default_settings.reg_finetune_lower_limit;
    settings->reg_finetune_primal_update_threshold = (piqp_int)  default_settings.reg_finetune_primal_update_threshold;
    settings->reg_finetune_dual_update_threshold = (piqp_int)  default_settings.reg_finetune_dual_update_threshold;
    settings->max_iter = (piqp_int) default_settings.max_iter;
    settings->max_factor_retires = (piqp_int) default_settings.max_factor_retires;
    settings->preconditioner_scale_cost = default_settings.preconditioner_scale_cost;
    settings->preconditioner_iter = (piqp_int) default_settings.preconditioner_iter;
    settings->tau = default_settings.tau;
    settings->iterative_refinement_always_enabled = (piqp_int) default_settings.iterative_refinement_always_enabled;
    settings->iterative_refinement_eps_abs = default_settings.iterative_refinement_eps_abs;
    settings->iterative_refinement_eps_rel = default_settings.iterative_refinement_eps_rel;
    settings->iterative_refinement_max_iter = (piqp_int) default_settings.iterative_refinement_max_iter;
    settings->iterative_refinement_min_improvement_rate = default_settings.iterative_refinement_min_improvement_rate;
    settings->iterative_refinement_static_regularization_eps = default_settings.iterative_refinement_static_regularization_eps;
    settings->iterative_refinement_static_regularization_rel = default_settings.iterative_refinement_static_regularization_rel;
    settings->verbose = default_settings.verbose;
    settings->compute_timings = default_settings.compute_timings;
}

piqp::optional<Eigen::Map<CVec>> piqp_optional_vec_map(piqp_float* data, piqp_int n)
{
    piqp::optional<Eigen::Map<CVec>> vec;
    if (data)
    {
        vec = Eigen::Map<CVec>(data, n);
    }
    return vec;
}

piqp::optional<Eigen::Map<CMat>> piqp_optional_mat_map(piqp_float* data, piqp_int m, piqp_int n)
{
    piqp::optional<Eigen::Map<CMat>> mat;
    if (data)
    {
        mat = Eigen::Map<CMat>(data, m, n);
    }
    return mat;
}

piqp::optional<Eigen::Map<CSparseMat>> piqp_optional_sparse_mat_map(piqp_csc* data)
{
    piqp::optional<Eigen::Map<CSparseMat>> mat;
    if (data)
    {
        mat = Eigen::Map<CSparseMat>(data->m, data->n, data->nnz, data->p, data->i, data->x);
    }
    return mat;
}

void piqp_setup_dense(piqp_workspace** workspace, const piqp_data_dense* data, const piqp_settings* settings)
{
    auto* work = new piqp_workspace;
    *workspace = work;

    auto* solver = new DenseSolver();
    work->solver_handle = reinterpret_cast<piqp_solver_handle*>(solver);
    work->solver_info.is_dense = 1;
    work->solver_info.n = data->n;
    work->solver_info.p = data->p;
    work->solver_info.m = data->m;
    work->result = new piqp_result;

    if (settings)
    {
        piqp_update_settings(work, settings);
    }

    Eigen::Map<CMat> P(data->P, data->n, data->n);
    Eigen::Map<CVec> c(data->c, data->n);
    Eigen::Map<CMat> A(data->A, data->p, data->n);
    Eigen::Map<CVec> b(data->b, data->p);
    Eigen::Map<CMat> G(data->G, data->m, data->n);
    Eigen::Map<CVec> h(data->h, data->m);
    piqp::optional<Eigen::Map<CVec>> x_lb = piqp_optional_vec_map(data->x_lb, data->n);
    piqp::optional<Eigen::Map<CVec>> x_ub = piqp_optional_vec_map(data->x_ub, data->n);

    solver->setup(P, c, A, b, G, h, x_lb, x_ub);

    piqp_update_result(work->result, solver->result());
}

void piqp_setup_sparse(piqp_workspace** workspace, const piqp_data_sparse* data, const piqp_settings* settings)
{
    auto* work = new piqp_workspace;
    *workspace = work;

    auto* solver = new piqp::SparseSolver<piqp_float, piqp_int>();
    work->solver_handle = reinterpret_cast<piqp_solver_handle*>(solver);
    work->solver_info.is_dense = 0;
    work->solver_info.n = data->n;
    work->solver_info.p = data->p;
    work->solver_info.m = data->m;
    work->result = new piqp_result;

    if (settings)
    {
        piqp_update_settings(work, settings);
    }

    Eigen::Map<CSparseMat> P(data->P->m, data->P->n, data->P->nnz, data->P->p, data->P->i, data->P->x);
    Eigen::Map<CVec> c(data->c, data->n);
    Eigen::Map<CSparseMat> A(data->A->m, data->A->n, data->A->nnz, data->A->p, data->A->i, data->A->x);
    Eigen::Map<CVec> b(data->b, data->p);
    Eigen::Map<CSparseMat> G(data->G->m, data->G->n, data->G->nnz, data->G->p, data->G->i, data->G->x);
    Eigen::Map<CVec> h(data->h, data->m);
    piqp::optional<Eigen::Map<CVec>> x_lb = piqp_optional_vec_map(data->x_lb, data->n);
    piqp::optional<Eigen::Map<CVec>> x_ub = piqp_optional_vec_map(data->x_ub, data->n);

    solver->setup(P, c, A, b, G, h, x_lb, x_ub);

    piqp_update_result(work->result, solver->result());
}

void piqp_update_settings(piqp_workspace* workspace, const piqp_settings* settings)
{
    if (workspace->solver_info.is_dense)
    {
        auto* solver = reinterpret_cast<DenseSolver*>(workspace->solver_handle);

        solver->settings().rho_init = settings->rho_init;
        solver->settings().delta_init = settings->delta_init;
        solver->settings().eps_abs = settings->eps_abs;
        solver->settings().eps_rel = settings->eps_rel;
        solver->settings().check_duality_gap = settings->check_duality_gap;
        solver->settings().eps_duality_gap_abs = settings->eps_duality_gap_abs;
        solver->settings().eps_duality_gap_rel = settings->eps_duality_gap_rel;
        solver->settings().reg_lower_limit = settings->reg_lower_limit;
        solver->settings().reg_finetune_lower_limit = settings->reg_finetune_lower_limit;
        solver->settings().reg_finetune_primal_update_threshold = settings->reg_finetune_primal_update_threshold;
        solver->settings().reg_finetune_dual_update_threshold = settings->reg_finetune_dual_update_threshold;
        solver->settings().max_iter = settings->max_iter;
        solver->settings().max_factor_retires = settings->max_factor_retires;
        solver->settings().preconditioner_scale_cost = settings->preconditioner_scale_cost;
        solver->settings().preconditioner_iter = settings->preconditioner_iter;
        solver->settings().tau = settings->tau;
        solver->settings().iterative_refinement_always_enabled = settings->iterative_refinement_always_enabled;
        solver->settings().iterative_refinement_eps_abs = settings->iterative_refinement_eps_abs;
        solver->settings().iterative_refinement_eps_rel = settings->iterative_refinement_eps_rel;
        solver->settings().iterative_refinement_max_iter = settings->iterative_refinement_max_iter;
        solver->settings().iterative_refinement_min_improvement_rate = settings->iterative_refinement_min_improvement_rate;
        solver->settings().iterative_refinement_static_regularization_eps = settings->iterative_refinement_static_regularization_eps;
        solver->settings().iterative_refinement_static_regularization_rel = settings->iterative_refinement_static_regularization_rel;
        solver->settings().verbose = settings->verbose;
        solver->settings().compute_timings = settings->compute_timings;
    }
    else
    {
        auto* solver = reinterpret_cast<SparseSolver*>(workspace->solver_handle);

        solver->settings().rho_init = settings->rho_init;
        solver->settings().delta_init = settings->delta_init;
        solver->settings().eps_abs = settings->eps_abs;
        solver->settings().eps_rel = settings->eps_rel;
        solver->settings().check_duality_gap = settings->check_duality_gap;
        solver->settings().eps_duality_gap_abs = settings->eps_duality_gap_abs;
        solver->settings().eps_duality_gap_rel = settings->eps_duality_gap_rel;
        solver->settings().reg_lower_limit = settings->reg_lower_limit;
        solver->settings().reg_finetune_lower_limit = settings->reg_finetune_lower_limit;
        solver->settings().reg_finetune_primal_update_threshold = settings->reg_finetune_primal_update_threshold;
        solver->settings().reg_finetune_dual_update_threshold = settings->reg_finetune_dual_update_threshold;
        solver->settings().max_iter = settings->max_iter;
        solver->settings().max_factor_retires = settings->max_factor_retires;
        solver->settings().preconditioner_scale_cost = settings->preconditioner_scale_cost;
        solver->settings().preconditioner_iter = settings->preconditioner_iter;
        solver->settings().tau = settings->tau;
        solver->settings().iterative_refinement_always_enabled = settings->iterative_refinement_always_enabled;
        solver->settings().iterative_refinement_eps_abs = settings->iterative_refinement_eps_abs;
        solver->settings().iterative_refinement_eps_rel = settings->iterative_refinement_eps_rel;
        solver->settings().iterative_refinement_max_iter = settings->iterative_refinement_max_iter;
        solver->settings().iterative_refinement_min_improvement_rate = settings->iterative_refinement_min_improvement_rate;
        solver->settings().iterative_refinement_static_regularization_eps = settings->iterative_refinement_static_regularization_eps;
        solver->settings().iterative_refinement_static_regularization_rel = settings->iterative_refinement_static_regularization_rel;
        solver->settings().verbose = settings->verbose;
        solver->settings().compute_timings = settings->compute_timings;
    }
}

void piqp_update_dense(piqp_workspace* workspace,
                       piqp_float* P, piqp_float* c,
                       piqp_float* A, piqp_float* b,
                       piqp_float* G, piqp_float* h,
                       piqp_float* x_lb, piqp_float* x_ub)
{
    piqp::optional<Eigen::Map<CMat>> P_ = piqp_optional_mat_map(P, workspace->solver_info.n, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CVec>> c_ = piqp_optional_vec_map(c, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CMat>> A_ = piqp_optional_mat_map(A, workspace->solver_info.p, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CVec>> b_ = piqp_optional_vec_map(b, workspace->solver_info.p);
    piqp::optional<Eigen::Map<CMat>> G_ = piqp_optional_mat_map(G, workspace->solver_info.m, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CVec>> h_ = piqp_optional_vec_map(h, workspace->solver_info.m);
    piqp::optional<Eigen::Map<CVec>> x_lb_ = piqp_optional_vec_map(x_lb, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CVec>> x_ub_ = piqp_optional_vec_map(x_ub, workspace->solver_info.n);

    auto* solver = reinterpret_cast<DenseSolver*>(workspace->solver_handle);
    solver->update(P_, c_, A_, b_, G_, h_, x_lb_, x_ub_);
}

void piqp_update_sparse(piqp_workspace* workspace,
                        piqp_csc* P, piqp_float* c,
                        piqp_csc* A, piqp_float* b,
                        piqp_csc* G, piqp_float* h,
                        piqp_float* x_lb, piqp_float* x_ub)
{
    piqp::optional<Eigen::Map<CSparseMat>> P_ = piqp_optional_sparse_mat_map(P);
    piqp::optional<Eigen::Map<CVec>> c_ = piqp_optional_vec_map(c, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CSparseMat>> A_ = piqp_optional_sparse_mat_map(A);
    piqp::optional<Eigen::Map<CVec>> b_ = piqp_optional_vec_map(b, workspace->solver_info.p);
    piqp::optional<Eigen::Map<CSparseMat>> G_ = piqp_optional_sparse_mat_map(G);
    piqp::optional<Eigen::Map<CVec>> h_ = piqp_optional_vec_map(h, workspace->solver_info.m);
    piqp::optional<Eigen::Map<CVec>> x_lb_ = piqp_optional_vec_map(x_lb, workspace->solver_info.n);
    piqp::optional<Eigen::Map<CVec>> x_ub_ = piqp_optional_vec_map(x_ub, workspace->solver_info.n);

    auto* solver = reinterpret_cast<SparseSolver*>(workspace->solver_handle);
    solver->update(P_, c_, A_, b_, G_, h_, x_lb_, x_ub_);
}

piqp_status piqp_solve(piqp_workspace* workspace)
{
    piqp::Status status;
    if (workspace->solver_info.is_dense)
    {
        auto* solver = reinterpret_cast<DenseSolver*>(workspace->solver_handle);
        status = solver->solve();
        piqp_update_result(workspace->result, solver->result());
    }
    else
    {
        auto* solver = reinterpret_cast<SparseSolver*>(workspace->solver_handle);
        status = solver->solve();
        piqp_update_result(workspace->result, solver->result());
    }

    return (piqp_status) status;
}

void piqp_cleanup(piqp_workspace* workspace)
{
    if (workspace)
    {
        if (workspace->solver_info.is_dense)
        {
            auto* solver = reinterpret_cast<DenseSolver*>(workspace->solver_handle);
            delete solver;
        }
        else
        {
            auto* solver = reinterpret_cast<SparseSolver *>(workspace->solver_handle);
            delete solver;
        }
        delete workspace->result;
        delete workspace;
    }
}
