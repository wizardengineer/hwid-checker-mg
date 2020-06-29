#pragma once
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <intrin.h>
#include <d3d9.h>
#include <iomanip>
#include <vector>
#include <map>

#pragma comment(lib, "d3d9.lib")
#pragma warning(disable : 4200)

#define out_of_spec "OUT OF SPEC"



namespace smbios 
{
  enum dmi_entry_type
    : BYTE
  {
    DMI_ENTRY_BIOS = 0,
    DMI_ENTRY_SYSTEM,
    DMI_ENTRY_BASEBOARD,
    DMI_ENTRY_CHASSIS,
    DMI_ENTRY_PROCESSOR,
    DMI_ENTRY_MEM_CONTROLLER,
    DMI_ENTRY_MEM_MODULE,
    DMI_ENTRY_CACHE,
    DMI_ENTRY_PORT_CONNECTOR,
    DMI_ENTRY_SYSTEM_SLOT,
    DMI_ENTRY_ONBOARD_DEVICE,
    DMI_ENTRY_OEMSTRINGS,
    DMI_ENTRY_SYSCONF,
    DMI_ENTRY_BIOS_LANG,
    DMI_ENTRY_GROUP_ASSOC,
    DMI_ENTRY_SYSTEM_EVENT_LOG,
    DMI_ENTRY_PHYS_MEM_ARRAY,
    DMI_ENTRY_MEM_DEVICE,
    DMI_ENTRY_32_MEM_ERROR,
    DMI_ENTRY_MEM_ARRAY_MAPPED_ADDR,
    DMI_ENTRY_MEM_DEV_MAPPED_ADDR,
    DMI_ENTRY_BUILTIN_POINTING_DEV,
    DMI_ENTRY_PORTABLE_BATTERY,
    DMI_ENTRY_SYSTEM_RESET,
    DMI_ENTRY_HW_SECURITY,
    DMI_ENTRY_SYSTEM_POWER_CONTROLS,
    DMI_ENTRY_VOLTAGE_PROBE,
    DMI_ENTRY_COOLING_DEV,
    DMI_ENTRY_TEMP_PROBE,
    DMI_ENTRY_ELECTRICAL_CURRENT_PROBE,
    DMI_ENTRY_OOB_REMOTE_ACCESS,
    DMI_ENTRY_BIS_ENTRY,
    DMI_ENTRY_SYSTEM_BOOT,
    DMI_ENTRY_MGMT_DEV,
    DMI_ENTRY_MGMT_DEV_COMPONENT,
    DMI_ENTRY_MGMT_DEV_THRES,
    DMI_ENTRY_MEM_CHANNEL,
    DMI_ENTRY_IPMI_DEV,
    DMI_ENTRY_SYS_POWER_SUPPLY,
    DMI_ENTRY_ADDITIONAL,
    DMI_ENTRY_ONBOARD_DEV_EXT,
    DMI_ENTRY_MGMT_CONTROLLER_HOST,
    DMI_ENTRY_INACTIVE = 126,
    DMI_ENTRY_END_OF_TABLE = 127,
  };

	enum dmi_memory_device_type // 7.18.2
		: BYTE
	{
		DMT_OTHER = 0x01, /* 0x01 */
		DMT_UNKNOWN,
		DMT_DRAM,
		DMT_EDRAM,
		DMT_VRAM,
		DMT_SRAM,
		DMT_RAM,
		DMT_ROM,
		DMT_FLASH,
		DMT_EEPROM,
		DMT_FEPROM,
		DMT_EPROM,
		DMT_CDRAM,
		DMT_3DRAM,
		DMT_SDRAM,
		DMT_SGRAM,
		DMT_RDRAM,
		DMT_DDR,
		DMT_DDR2,
		DMT_DDR2_FB_DIMM,
		DMT_RESERVED1,
		DMT_RESERVED2,
		DMT_RESERVED3,
		DMT_DDR3,
		DMT_FBD2,
		DMT_DDR4,
		DMT_LPDDR,
		DMT_LPDDR2,
		DMT_LPDDR3,
		DMT_LPDDR4,
		DMT_LOGICAL_NONVOLATILE_DEVICE,
		DMT_HBM,
		DMT_HBM2 /* 0x21 */
	};

	//
	// Data structure the consults the hold of extracted data from the the raw
	// SMBIOS firmware table
	//

	typedef struct RawSMBIOSData
	{
		BYTE    Used20CallingMethod;
		BYTE    SMBIOSMajorVersion;
		BYTE    SMBIOSMinorVersion;
		BYTE    DmiRevision;
		DWORD   Length;
		BYTE    SMBIOSTableData[];
	} RawSMBIOSData;



	// definition:
	// Desktop Management Interface (DMI) generates a standard framework 
	// for managing and tracking components in a desktop, notebook or server 
	// computer, by abstracting these components from the software that manages them.
	//

	typedef struct _dmi_header
	{
		BYTE		type;
		BYTE		length;
		WORD		handle;
	} dmi_header;


	// function protoypes
	void dmi_system_uuid(const BYTE* p, short version);
	const char* dmi_string(const dmi_header* dm, BYTE src);
	const char* dmi_memory_device_type(BYTE code);
};