#pragma once

#define DirectX12WarningDisableBegin	\
__pragma (warning(push))				\
__pragma (warning(disable:4061))		\

#define DirectX12WarningDisableEnd		\
__pragma (warning(pop))					\

#define DirectX12CompileLib					\
__pragma (comment(lib,"d3d12.lib"))			\
__pragma (comment(lib,"dxgi.lib"))			\
__pragma (comment(lib,"d3dcompiler.lib"))	\