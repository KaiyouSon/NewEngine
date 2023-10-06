#pragma once

#define DirectX12WarningDisableBegin		\
__pragma (warning(push))			\
__pragma (warning(disable:4061))	\

#define DirectX12WarningDisableEnd			\
__pragma (warning(pop))
