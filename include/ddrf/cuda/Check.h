#ifndef CUDA_CHECK_H_
#define CUDA_CHECK_H_

#include <stdexcept>

#ifndef __CUDACC__
#include <cuda_runtime.h>
#endif

#include <cufft.h>

#define CHECK(x) ddrf::cuda::check(x, __FILE__, __LINE__)
#define CHECK_CUFFT(x) ddrf::cuda::checkCufft(x, __FILE__, __LINE__)

namespace ddrf
{
	namespace cuda
	{
		namespace detail
		{
			inline auto checkCudaError(cudaError_t err, const char* file, int line) -> void
			{
				if(err != cudaSuccess)
				{
					throw std::runtime_error("CUDA assertion failed at " + std::string(file) + ":" + std::to_string(line) +
							": " + std::string(cudaGetErrorString(err)));
				}
			}

			inline auto getCufftErrorString(cufftResult result) -> std::string
			{
				switch(result)
				{
					case CUFFT_SUCCESS: return "The cuFFT operation was successful";
					case CUFFT_INVALID_PLAN: return "cuFFT was passed an invalid plan handle";
					case CUFFT_ALLOC_FAILED: return "cuFFT failed to allocate GPU or CPU memory";
					case CUFFT_INVALID_TYPE: return "Invalid type";
					case CUFFT_INVALID_VALUE: return "Invalid pointer or parameter";
					case CUFFT_INTERNAL_ERROR: return "Driver or internal cuFFT library error";
					case CUFFT_EXEC_FAILED: return "Failed to execute an FFT on the GPU";
					case CUFFT_SETUP_FAILED: return "The cuFFT library failed to initialize";
					case CUFFT_INVALID_SIZE: return "User specified an invalid transform size";
					case CUFFT_UNALIGNED_DATA: return "Unaligned data";
					case CUFFT_INCOMPLETE_PARAMETER_LIST: return "Missing parameters in call";
					case CUFFT_INVALID_DEVICE: return "Execution of plan was on different GPU than plan creation";
					case CUFFT_PARSE_ERROR: return "Internal plan database error";
					case CUFFT_NO_WORKSPACE: return "No workspace has been provided prior to plan execution";
					case CUFFT_NOT_IMPLEMENTED: return "This feature was not implemented for your cuFFT version";
					case CUFFT_LICENSE_ERROR: return "NVIDIA license required. The file was either not found, is out of data, or otherwise invalid";
					default: return "Unknown error";
				}
			}

			inline auto checkCufftError(cufftResult result, const char *file, int line) -> void
			{
				if(result != CUFFT_SUCCESS)
				{
					throw std::runtime_error("cuFFT assertion failed at " + std::string(file) + ":" + std::to_string(line) +
							": " + getCufftErrorString(result));
				}

			}
		}

		inline auto check(cudaError_t err, const char* file, int line) -> void
		{
			detail::checkCudaError(err, file, line);
		}

		inline auto checkCufft(cufftResult res, const char* file, int line) -> void
		{
			detail::checkCufftError(res, file, line);
		}
	}
}

#endif /* CUDA_CHECK_H_ */
