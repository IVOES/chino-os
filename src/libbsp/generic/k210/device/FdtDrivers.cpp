//
// Kernel Device
//
#include <libdriver/devicetree/Fdt.hpp>

using namespace Chino::Device;

extern const unsigned char devicetree_dtb[];
extern const unsigned int devicetree_dtb_len;

gsl::span<const uint8_t> Chino::Device::BSPGetFdtData() noexcept
{
    return { devicetree_dtb, devicetree_dtb_len };
}

#define REF_FDT_DRIVER_DESC(Type) &Type::Descriptor

const FDTDriverDescriptor* Chino::Device::g_FDTDrivers[] =
{
	nullptr
};
