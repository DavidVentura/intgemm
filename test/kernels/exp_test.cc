#include "test/test.h"
#include "aligned.h"
#include "kernels.h"

#include <numeric>

namespace intgemm {

template <CPUType CPUType_>
void kernel_exp_approx_taylor_test() {
  if (kCPU < CPUType_)
    return;

  using vec_t = vector_t<CPUType_, float>;
  constexpr static auto VECTOR_LENGTH = sizeof(vec_t) / sizeof(float);

  AlignedVector<float> input(VECTOR_LENGTH);
  AlignedVector<float> output(VECTOR_LENGTH);

  std::iota(input.begin(), input.end(), -1.2);

  *output.template as<vec_t>() = kernels::exp_approx_taylor(*input.template as<vec_t>());
  for (auto i = 0; i < output.size(); ++i)
    CHECK_EPS(output[i], exp(input[i]), 0.001f);
}

template INTGEMM_AVX2 void kernel_exp_approx_taylor_test<CPUType::AVX2>();
KERNEL_TEST_CASE("exp_approx_taylor AVX2") { return kernel_exp_approx_taylor_test<CPUType::AVX2>(); }

#ifndef INTGEMM_NO_AVX512
template INTGEMM_AVX512BW void kernel_exp_approx_taylor_test<CPUType::AVX512BW>();
KERNEL_TEST_CASE("exp_approx_taylor AVX512BW") { return kernel_exp_approx_taylor_test<CPUType::AVX512BW>(); }
#endif

}
