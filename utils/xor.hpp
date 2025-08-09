#include <cstddef>
#include <cstdint>
#include <immintrin.h>
#include <type_traits>
#include <utility>

#define XORSTR_(s) (s)

#define xs(s) (s)

#define XORSTR_FORCEINLINE __forceinline
#define JM_XORSTR_DISABLE_AVX_INTRINSICS
#define JM_XORSTR_LOAD_FROM_REG(x) (x)